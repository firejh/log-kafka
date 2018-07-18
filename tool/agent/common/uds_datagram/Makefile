######################################
#
######################################
  
#target you can change test to what you want
#共享库文件名，lib*.a
TARGET_STATIC	:= ./export/lib/libudsdatagram.a
TARGET_DYNAMIC	:= ./export/lib/libudsdatagram.so
  
#compile and lib parameter
#编译参数
AR      = ar
LIBS    :=
LDFLAGS :=
DEFINES :=
INCLUDE := -I./export/include
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)
#CXXFLAGS:= -fPIC $(CFLAGS)
CXXFLAGS:= $(CFLAGS)
SHARE   := -shared -o
  
#source file
#源文件，自动找所有.c和.cpp文件，并将目标定义为同名.o文件
SOURCE  := $(wildcard ./src/*.c) $(wildcard ./src/*.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
 
.PHONY : clean distclean rebuild static dynamic
  
all : $(TARGET_STATIC)
	
rebuild : distclean all
	
clean :
	rm -fr ./src/*.o
		    
distclean : clean
	rm -fr $(TARGET_STATIC) $(TARGET_DYNAMIC)
		  
static : $(TARGET_STATIC)

$(TARGET_STATIC) : $(OBJS)
	$(AR) cru $(TARGET_STATIC) $(OBJS)

dynamic : $(TARGET_DYNAMIC)

$(TARGET_DYNAMIC) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(SHARE) $@ $(OBJS) $(LDFLAGS) $(LIBS)
