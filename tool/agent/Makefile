#makefile for those who have many excutable files

.PHONY: all strip clean sync install mem_check
.SUFFIXES: .sh .h .hpp .c .cc .cpp

OSNAME      := $(shell uname)

CC			= gcc
CFLAGS		= -O0 -ggdb3
CX			= g++
#CXFLAGS	    = -O0 -g -ggdb3
#CFLAGS		= -O3 -Werror -std=c++11
CFLAGS		= -O2 -Werror

TAR         = ar
TARFLAGS    = crs

# curl      google    iniparser log_kafka
INC_DIR		= ./inc
SRC_DIR		= ./src
EXAM_DIR	= ./exam
OBJ_DIR		= ./obj
LIB_DIR     = ./lib
BIN_DIR		= ./bin
LIB_BIN     = liblogcollect$(VERSION).a
BIN			= log_kafka_gent

INC			= -I$(INC_DIR) \
              -I./third/protobuf/include/google \
              -I./third/glog/include/ \
              -I./third/client_sdk/include/ \
              -I./third/iniparser/include/ \
              -I./third/uds_datagram/inc

LIB			= -lpthread \
              ./third/protobuf/linux_lib/libprotobuf.3.4.1.a \
              ./third/curl/linux_lib/libcurl.a \
              ./third/glog/linux_lib/libglog.a \
              ./third/iniparser/linux_lib/libiniparser.a \
              ./third/uds_datagram/linux_lib/libuds1.0.0.a \
              ./third/client_sdk/linux_lib/libclient_sdk1.0.0.a

ifeq ($(OSNAME), Darwin)
    LIB     += -L./third/curl/mac_lib -lcurl
endif
ifeq ($(OSNAME), Linux)
	LIB     += -L./third/curl/linux_lib -lcurl
endif

ifeq ($(OSNAME), Darwin)
    LIB     += -L./third/protobuf/mac_lib
endif
ifeq ($(OSNAME), Linux)
    LIB     += -Wl,-rpath,./third/protobuf/linux_lib -L./third/protobuf/linux_lib
endif


all: dir $(addprefix $(BIN_DIR)/,$(BIN))

#@ do not outoupt this command - ingnore its error and continue to make
dir:
	@-mkdir -p $(OBJ_DIR)
	# @-mkdir -p $(LIB_DIR)
	@-mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CX) $(CXFLAGS) $(INC) -o $@ -c $<

$(BIN_DIR)/$(BIN):$(addprefix $(OBJ_DIR)/, agent_server.o config.o main.o)
	$(CX) $(CXFLAGS) -o $@ $^ $(LIB)

strip:
	strip $(addprefix $(BIN_DIR)/,$(BIN))

clean:
	@rm -f $(addprefix $(BIN_DIR)/,$(BIN)) $(OBJ_DIR)/*.o ./valgrind_info
	@rm -rf $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR)

mem_check:
	valgrind --log-file=valgrind_info --track-origins=yes --leak-check=full --show-reachable=yes -v bin/tellist_fb
