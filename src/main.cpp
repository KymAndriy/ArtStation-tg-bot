#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include <algorithm>
#include <tgbot/tgbot.h>

#include "JsonParser.h"
#include "UrlGrabber.h"
#include <boost/algorithm/string.hpp>
// #include

using namespace TgBot;

void createKeyboard(const std::map<std::string, std::string> &buttonStringsMap, InlineKeyboardMarkup::Ptr &keyboard)
{
    std::vector<InlineKeyboardButton::Ptr> row;
    int i = 0;
    for (auto it = buttonStringsMap.begin(); it != buttonStringsMap.end(); it++)
    {
        std::string button_text = it->first;// = boost::algorithm::to_lower_copy(it->first);
        std::string button_com = boost::algorithm::to_lower_copy(button_text);
        std::replace( button_com.begin(), button_com.end(), '_', ' ');
        InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
        checkButton->text = button_text;
        // checkButton->
        checkButton->callbackData = button_com;
        row.push_back(checkButton);

        if ((i % 3) == 0)
        {
            keyboard->inlineKeyboard.push_back(row);
            row.clear();
        }
        i++;
    }
}

void createComands(const std::map<std::string, std::string> &buttonStringsMap, Bot &bot, InlineKeyboardMarkup::Ptr &keyboard)
{
    // int i = 0;

    for (auto it = buttonStringsMap.begin(); it != buttonStringsMap.end(); it++)
    {
        std::string com = boost::algorithm::to_lower_copy(it->first);
        std::replace( com.begin(), com.end(), ' ', '_');
        // com.append(std::to_string(i++));

        bot.getEvents().onCommand(com, [&bot, &keyboard, it](Message::Ptr message)
                                  { bot.getApi().sendMessage(message->chat->id, it->second, false, 0, keyboard, "Markdown"); });
        
        bot.getEvents().onCallbackQuery([&bot, &keyboard,com, it](CallbackQuery::Ptr query) {
            if (StringTools::startsWith(query->data,com)) {
                bot.getApi().sendMessage(query->message->chat->id, it->second, false, 0, keyboard, "Markdown");
            }
        });
    }
}

int main()
{
    JsonParser p("/home/andrii/temp_project/ArtStation-tg-bot/bot_configs.json");
    p.parse();

    // std::string str;
    // std::fstream fst("/home/andrii/temp_project/ArtStation-tg-bot/artwork.json", std::ios::in);
    // std::string temp;
    // while(!fst.eof())
    // {
    //     fst >> temp;
    //     str.append(temp);
    // // }
    // UrlGrabber grab;//(p.getImageUrlRegex(), str);
    // grab.setParseStr(str);
    // grab.setRegexExpression(p.getImageRegex());
    // std::vector<std::string> _urls = grab.getUrls();

    Bot bot(p.getToken());

    std::map<std::string, std::string> buttonStringsMap = p.getNameAndUrls();
    // Thanks Pietro Falessi for code
    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    createKeyboard(p.getNameAndUrls(), keyboard);

    // createComands(p.getNameAndUrls(), bot, keyboard);

    // int i = 0;
    for (auto it = buttonStringsMap.begin(); it != buttonStringsMap.end(); it++)
    {
        std::string com = boost::algorithm::to_lower_copy(it->first);
        std::replace( com.begin(), com.end(), ' ', '_');
        bot.getEvents().onCommand(com, [&bot, it](Message::Ptr message) {
            bot.getApi().sendMessage(message->chat->id, it->second);//, false, 0, , "Markdown");
        });

        //  bot.getEvents().onCallbackQuery([&bot, &keyboard,com, it](CallbackQuery::Ptr query) {
        //     if (StringTools::startsWith(query->data, com)) {
        //         bot.getApi().sendMessage(query->message->chat->id, it->second);//, false, 0, 0, "Markdown");
        //     }
        // });
    }
    bot.getEvents().onCommand("start", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, 0, false, 0, keyboard);
    });

    // bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
    //     if (StringTools::startsWith(query->data, "check")) {
    //         std::string response = "ok";
    //         bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
    //     }
    // });

    signal(SIGINT, [](int s)
           {
        printf("SIGINT got\n");
        exit(0); });

    // try {
    printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
    bot.getApi().deleteWebhook();

    TgLongPoll longPoll(bot);
    while (true)
    {
        printf("Long poll started\n");
        longPoll.start();
    }
    // } catch (std::exception& e) {
    //     printf("error: %s\n", e.what());
    // }
    //
    // return 0;

    // for(size_t i = 0; i < _urls.size(); i++)
    // {
    //     std::cout << _urls[i] << std::endl;
    // }

    // fst.close();
    return 0;
}

/*
    #include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <vector>

#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

void createOneColumnKeyboard(const vector<string>& buttonStrings, ReplyKeyboardMarkup::Ptr& kb)
{
  for (size_t i = 0; i < buttonStrings.size(); ++i) {
    vector<KeyboardButton::Ptr> row;
    KeyboardButton::Ptr button(new KeyboardButton);
    button->text = buttonStrings[i];
    row.push_back(button);
    kb->keyboard.push_back(row);
  }
}

void createKeyboard(const vector<vector<string>>& buttonLayout, ReplyKeyboardMarkup::Ptr& kb)
{
  for (size_t i = 0; i < buttonLayout.size(); ++i) {
    vector<KeyboardButton::Ptr> row;
    for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
      KeyboardButton::Ptr button(new KeyboardButton);
      button->text = buttonLayout[i][j];
      row.push_back(button);
    }
    kb->keyboard.push_back(row);
  }
}


int main() {
    string token(getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());

    Bot bot(token);

    ReplyKeyboardMarkup::Ptr keyboardOneCol(new ReplyKeyboardMarkup);
    createOneColumnKeyboard({"Option 1", "Option 2", "Option 3"}, keyboardOneCol);

    ReplyKeyboardMarkup::Ptr keyboardWithLayout(new ReplyKeyboardMarkup);
    createKeyboard({
      {"Dog", "Cat", "Mouse"},
      {"Green", "White", "Red"},
      {"On", "Off"},
      {"Back"},
      {"Info", "About", "Map", "Etc"}
    }, keyboardWithLayout);

    bot.getEvents().onCommand("start", [&bot, &keyboardOneCol](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "/start for one column keyboard\n/layout for a more complex keyboard", false, 0, keyboardOneCol);
    });
    bot.getEvents().onCommand("layout", [&bot, &keyboardWithLayout](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "/start for one column keyboard\n/layout for a more complex keyboard", false, 0, keyboardWithLayout);
    });
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start") || StringTools::startsWith(message->text, "/layout")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}

*/