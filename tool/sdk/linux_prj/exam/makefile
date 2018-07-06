#makefile for those who have many excutable files

.PHONY: all strip clean sync install mem_check
.SUFFIXES: .sh .h .hpp .c .cc .cpp

OSNAME      := $(shell uname)

CC			= gcc
CFLAGS		= -O0 -ggdb3
CX			= g++
CXFLAGS	    = -O0 -g -ggdb3
#CFLAGS		= -O3 -Werror -std=c++11

TAR         = ar
TARFLAGS    = crs

INC_DIR		= ./inc -I./misc -I./common -I../../third/protobuf/include/ -I../../third/curl/include/ -I../../third -I/usr/local/protobuf/include/  -I./third
SRC_DIR		= ./src
EXAM_DIR	= ./exam
OBJ_DIR		= ./obj
LIB_DIR     = ./lib
BIN_DIR		= ./bin
LIB_BIN     = liblogcollect.a
#BIN			= simple example
BIN			= example

INC			= -I$(INC_DIR)
LIB			= -lpthread -L$(LIB_DIR) -llogcollect

ifeq ($(OSNAME), Darwin)
    LIB     += -L../../third/curl/mac_lib -lcurl
endif
ifeq ($(OSNAME), Linux)
	LIB     += -L../../third/curl/linux_lib -lcurl
endif

ifeq ($(OSNAME), Darwin)
    LIB     += -L../../third/protobuf/mac_lib -lprotobuf
endif
ifeq ($(OSNAME), Linux)
    LIB     += -Wl,-rpath,../../third/protobuf/linux_lib -L../../third/protobuf/linux_lib -lprotobuf
endif


all: debug release
#strip

debug release: dir $(addprefix $(LIB_DIR)/,$(LIB_BIN)) $(addprefix $(BIN_DIR)/,$(BIN))

#@ do not outoupt this command - ingnore its error and continue to make
dir:
	@-mkdir -p $(OBJ_DIR)
	@-mkdir -p $(LIB_DIR)
	@-mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cc
	$(CX) $(CXFLAGS) $(INC) -o $@ -c $<

$(OBJ_DIR)/%.o:$(EXAM_DIR)/%.cc
	$(CX) $(CXFLAGS) $(INC) -o $@ -c $<

$(LIB_DIR)/liblogcollect.a:$(addprefix $(OBJ_DIR)/, log_collect_sdk.o log_data.o pbjson.o service.pb.o log_server.o)
	$(TAR) $(TARFLAGS) $@ $^

# $(BIN_DIR)/simple:$(addprefix $(OBJ_DIR)/, simple.o)
#	$(CX) $(CXFLAGS) -o $@ $^ $(LIB)

$(BIN_DIR)/example:$(addprefix $(OBJ_DIR)/, example.o)
	$(CX) $(CXFLAGS) -o $@ $^ $(LIB)

strip:
	strip $(addprefix $(BIN_DIR)/,$(BIN))

clean:
	@rm -f $(addprefix $(BIN_DIR)/,$(BIN)) $(OBJ_DIR)/*.o ./valgrind_info
	@rm -rf $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR)

mem_check:
	valgrind --log-file=valgrind_info --track-origins=yes --leak-check=full --show-reachable=yes -v bin/tellist_fb
