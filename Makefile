TG_FLAGS = --std=c++17 -g -Wall -Iinclude/ -I/usr/local/include -lTgBot -lcurl -lboost_system -lboost_json -lssl -lcrypto -lpthread
# SCR = src
OBJ_FLAGS = -c $(TG_FLAGS)
DEBUG = -g

BUILD = build

.PHONY: all

all: build_dir bot_main

build_dir: 
	mkdir -p $(BUILD)

JsonParser.o:
	g++ src/JsonParser.cpp $(OBJ_FLAGS)
	mv JsonParser.o $(BUILD)

thread_pool.o:
	g++ src/thread_pool.cpp $(OBJ_FLAGS)
	mv thread_pool.o $(BUILD)

UrlGrabber.o:
	g++ src/UrlGrabber.cpp $(OBJ_FLAGS) 
	mv UrlGrabber.o $(BUILD)


bot_main: UrlGrabber.o JsonParser.o thread_pool.o src/main.cpp
	g++ src/main.cpp -o main \
	$(BUILD)/UrlGrabber.o $(BUILD)/JsonParser.o $(BUILD)/thread_pool.o $(BUILD)/JsonReceiver.o \
	$(TG_FLAGS)
	mv main $(BUILD)