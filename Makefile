TG_FLAGS = --std=c++11 -Wall -Iinclude/ -I/usr/local/include -lTgBot -lboost_system -lboost_json -lssl -lcrypto -lpthread
# SCR = src
OBJ_FLAGS = -c $(TG_FLAGS)
DEBUG = -g

BUILD = build

.PHONY: all

all: build_dir bot_main

build_dir: 
	mkdir -p $(BUILD)

JsonParser.o:
	g++ src/JsonParser.cpp $(OBJ_FLAGS) $(DEBUG)
	mv JsonParser.o $(BUILD)


UrlGrabber.o:
	g++ src/UrlGrabber.cpp $(OBJ_FLAGS) $(DEBUG)
	mv UrlGrabber.o $(BUILD)


bot_main: UrlGrabber.o JsonParser.o src/main.cpp
	g++ src/main.cpp -o main \
	$(BUILD)/UrlGrabber.o $(BUILD)/JsonParser.o $(TG_FLAGS) $(DEBUG)
	mv main $(BUILD)