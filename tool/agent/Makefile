######################################
#
#######################################
TOP_PATH = $(shell pwd)

#source file
#源文件，自动找所有.c和.cpp文件，并将目标定义为同名.o文件
SOURCE  := $(wildcard ./src/*.c) $(wildcard ./src/*.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
  
#target you can change test to what you want
#目标文件名，输入任意你想要的执行文件名
TARGET  := ./bin/log_kafka_agent
  
#compile and lib parameter
#编译参数
CC      := g++
INCLUDE :=	-I$(TOP_PATH)/import/include \
			-I$(TOP_PATH)/import/include/google \
			-I$(TOP_PATH)/common/uds_datagram/export/include

LIBS    :=	$(TOP_PATH)/common/uds_datagram/export/lib/libudsdatagram.a \
			$(TOP_PATH)/import/lib/libiniparser.a \
			$(TOP_PATH)/import/lib/liblogcollect.a \
			$(TOP_PATH)/import/lib/libprotobuf.a \
			$(TOP_PATH)/import/lib/libglog.a \
			-L$(TOP_PATH)/import/lib \
			-lcurl \
			-lpthread
LDFLAGS :=
DEFINES :=
CFLAGS  := -g -Wall -O0 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS)
  
.PHONY : clean distclean rebuild
	  
all : $(TARGET)
	  
objs : $(OBJS)
	  
rebuild: distclean all
	                
clean :
	rm -fr src/*.o
			    
distclean : clean
	rm -fr $(TARGET)
		  
$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
