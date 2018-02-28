RM					:= rm -rf
MKDIR				:= mkdir -p
COPY				:= cp -rf

CXX					:= g++
CPPFLAGS			:= -Wall -std=c++11 -j4 -Os
CPPFLAGS_DEBUG		:= -Wall -std=c++11 -j4 -O0 -g

LIBS				:= -lpthread
LIBS_DIR			:=
