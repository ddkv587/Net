include ../DECLARE.mk

OBJECT 		:= ThreadPool
CFLAGS 		+= 

LIBS		:=
LIBS_DIR	:=

SRCS		:= $(wildcard *.cpp)
CPP_OBJECT	:= $(patsubst %.cpp, %.o, $(SRCS))

$(OBJECT): $(CPP_OBJECT)
	$(CC) $(CFLAGS) $^ -shared -o $(TARGET_OBJECT_DIR)/lib$@.so 

$(CPP_OBJECT):$(SRCS)
	$(CC) -c $(CFLAGS) -fPIC $(@:.o=.cpp) -o $@

clean:
	$(RM) *.o
	$(RM) *.so

.PHONY:clean
