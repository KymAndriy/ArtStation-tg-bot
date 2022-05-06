// #include <iostream>
// #include <string>
// #include <fstream>
// #include <vector>
// #include <csignal>
// #include <cstdio>
// #include <cstdlib>
// #include <exception>
// #include <string>
// #include <vector>
// #include <algorithm>

#include <tgbot/tgbot.h>
#include <boost/algorithm/string.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/spsc_queue.hpp>

#include "JsonParser.h"
#include "UrlGrabber.h"
#include "JsonReceiver.hpp"
#include "StringManager.h"
#include "thread_pool.h"
#include "common_defs.h"

using namespace TgBot;

void createKeyboard(const std::map<std::string,
                    std::string> &buttonsNameMap,
                    InlineKeyboardMarkup::Ptr &keyboard,
                    size_t column_size, size_t max_word_len)
{
    std::vector<InlineKeyboardButton::Ptr> row;

    size_t i = 0;
    for (auto it = buttonsNameMap.begin(); it != buttonsNameMap.end(); it++)
    {
        InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
        checkButton->text = it->first;
        StringManager::replaceCharacters(checkButton->text, {'_'}, ' ');
        checkButton->callbackData = boost::algorithm::to_lower_copy(it->first);
        ;
        StringManager::replaceCharacters(checkButton->callbackData,
                                         {' ', '&', '-', });

        /// TODO: change keyboard build algorithm
        if (((i != 0) && (i % column_size) == 0) && row.size() != 0)
        {
            keyboard->inlineKeyboard.push_back(row);
            row.clear();
        }
        row.push_back(checkButton);
        if (it->first.size() > max_word_len && row.size() != 0)
        {
            keyboard->inlineKeyboard.push_back(row);
            row.clear();
        }
        i++;
    }
}

void task(boost::lockfree::spsc_queue<std::string> &pool, std::string url, JsonParser &p)
{
    std::string str;
    UrlGrabber grab;
    JsonReceiver::get(url, str);

    grab.setParseStr(str);
    grab.setRegexExpression(p.getFinalUrlRegex());
    auto urls = grab.getUrls();
    for (auto i : urls)
    {
        pool.push(i);
    }
    // pool.pop()
}


void taskOneThread(boost::lockfree::spsc_queue<std::string> &pool, std::string url, JsonParser &p)
{
    std::string str;
    UrlGrabber grab;
    JsonReceiver::get(url, str);

    grab.setParseStr(str);
    grab.setRegexExpression(p.getFinalUrlRegex());
    auto urls = grab.getUrls();
    for (auto i : urls)
    {
        pool.push(i);
    }
    // pool.pop()
}

/// TODO: add (int argc, char*[] argv) for json path
int main(int argc, char *argv[])
{
    curl_global_init(CURL_GLOBAL_ALL);
    JsonParser parser("/home/andrii/temp_project/ArtStation-tg-bot/bot_configs.json");
    parser.parse();

    Bot bot(parser.getToken());

    std::map<std::string, std::string> buttonsNameMap = parser.getNameAndUrls();

    InlineKeyboardMarkup::Ptr menu_keyboard(new InlineKeyboardMarkup);
    {
        InlineKeyboardButton::Ptr menu_button(new InlineKeyboardButton);
        menu_button->text = "Menu";
        menu_button->callbackData = "menu";
        menu_keyboard->inlineKeyboard.push_back(std::vector<InlineKeyboardButton::Ptr>({menu_button}));
    }

    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    createKeyboard(buttonsNameMap, keyboard, 3, 14);

    /// if wrap this 'for' in function it will be 'error: std::bad_alloc'
    for (auto it = buttonsNameMap.begin(); it != buttonsNameMap.end(); it++)
    {
        std::string callback_command = boost::algorithm::to_lower_copy(it->first);
        StringManager::replaceCharacters(callback_command, {' ', '&', '-',});

        bot.getEvents().onCallbackQuery(
            [&bot, callback_command, it, &parser, &menu_keyboard](CallbackQuery::Ptr query)
            {
                std::thread t([&bot, callback_command, it, &parser, &menu_keyboard, &query]()
                              {
                if (StringTools::startsWith(query->data, callback_command)) {
                    bot.getApi().sendChatAction(query->message->chat->id,"upload_photo");

                    std::string str;
                    JsonReceiver::get(it->second, str);
                    
                    UrlGrabber grab;
                    grab.setParseStr(str);
                    grab.setRegexExpression(parser.getHashImageRegex());
                    auto hash_urls = grab.getUrls();
                    StringManager::extractHash(hash_urls);
                    boost::lockfree::spsc_queue<std::string> data(1000);
                    std::vector<std::thread> pool;
                    //thread_pool pool(8);
                    for(auto iter : hash_urls)
                    {
                        pool.push_back(std::thread(task,std::ref(data),iter, std::ref(parser)));
                        // pool.add_task(task, std::ref(data),iter, std::ref(parser));
                        // bot.getApi().sendMessage(query->message->chat->id, 
                        //     iter, false, 0,0,"HTML");
                    }
                    // pool.wait_all();
                    std::string img;
                    // using namespace std::literals;
                    // std::this_thread::sleep_for(10ns);
                    bot.getApi().sendChatAction(query->message->chat->id,"upload_photo");

                    for(int i = 0; !data.empty() || i < 100; i++)
                    {
                        try{
                            data.pop(img);
                            // bot.getApi().sendMessage(query->message->chat->id, 
                            //     img, false, 0,0,"HTML");
                            bot.getApi().sendPhoto(query->message->chat->id, img);
                            // bot.getApi().sendDocument(query->message->chat->id, img);
                            img.clear();
                        }catch(...){}
                    }
                    // void collect(queue_thread<int> &arts,std::string artwork_url, std::string artwork_regex);
                    /*
                    ///TODO: add sending photoes utility
                    ///TODO: Add Thread poll that created once and only receive url, return
                    
                        struct art
                        {
                            photo_url,
                            artist,
                            number of artist work
                        }
                    */
                    std::string msg = it->first;
                    std::replace( msg.begin(), msg.end(), '_', ' ');
                    ///TODO: Add link to artist and Name 
                    // msg.append(", <a href=\"").append(it->second).append("\">Artist</a>\n");
                    bot.getApi().sendMessage(query->message->chat->id, msg, true, 0, menu_keyboard,"HTML");
                    for(auto& i: pool)
                    {
                        i.join();
                    }
            } });
                t.detach();
            });
    }

    /// TODO: write bot description at start
    bot.getEvents().onCommand("start", [&bot, &menu_keyboard](Message::Ptr message)
                              { 
                // std::thread t([&bot, &menu_keyboard, &message](){
                bot.getApi().sendMessage(message->chat->id, "Show:", false, 0, menu_keyboard); 
                // });
                // t.detach();
                 });
    bot.getEvents().onCommand("menu", [&bot, &keyboard](Message::Ptr message)
            { 
            // std::thread t([&bot, &keyboard, &message](){
                    bot.getApi().sendMessage(message->chat->id, "Choose theme:", false, 0, keyboard); 
                // });
            // t.detach(); 
            });
    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query)
                                    {
            if (StringTools::startsWith(query->data, "menu")) {
                bot.getApi().sendMessage(query->message->chat->id, "Choose theme:", false, 0, keyboard);
            } });

    signal(SIGINT, [](int s)
           {
        printf("SIGINT got\n");
        exit(0); 
        curl_global_cleanup(); });

    try
    {
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true)
        {
            longPoll.start();
        }
    }
    catch (std::exception &e)
    {
        printf("error: %s\n", e.what());
    }

    return 0;
}