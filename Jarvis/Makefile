PROJECT 	:= $(shell pwd)
INCLUDE 	:= -I$(PROJECT)/speech
SRC 		:= $(wildcard *.cpp)
BIN 		:= $(PROJECT)/bin
CXX 		:= g++
CC 			:= gcc
TARGET 		:= Jarvis
FLAGS		:= -O3 -Wall -g -std=c++11
LIB 		:= -ljsoncpp -lcurl -lcrypto -lpthread

all : $(BIN)/$(TARGET)

$(BIN)/$(TARGET) : $(SRC)
	[ -e $(BIN) ] || mkdir -p $(BIN)
	$(CXX) -o $@ $^ $(LIB) $(FLAGS) $(INCLUDE)

.PHONY :

clean :
	rm -rf $(BIN)/$(TARGET) version

help :
	@echo "make : ...create this project..."
	@echo "make clean : ...clean this project workdir..."
	@echo "target will create to current dir bin ..."
