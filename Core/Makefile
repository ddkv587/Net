include ../DECLARE.mk

OBJECT 		:= Core
CFLAGS 		+= -fPIC

LIBS		:=
LIBS_DIR	:=

SRCS		:= $(wildcard *.cpp)
CPP_OBJECT	:= $(patsubst %.cpp, %.o, $(SRCS))

$(OBJECT): $(CPP_OBJECT)
	$(CC) $(CFLAGS) $^ --share -o lib$(OBJECT).so 

$(CPP_OBJECT):$(SRCS)
	$(CC) -c $(CFLAGS) $(@:.o=.cpp) -o $@

clean:
	$(RM) *.o
	$(RM) *.so

.PHONY:clean
