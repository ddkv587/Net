RM					:= rm -rf
MKDIR				:= mkdir -p
COPY				:= cp -rf

CXX					:= g++
CPP_FLAGS			:= -Wall -std=c++11 -Os -g
CPP_FLAGS_DEBUG		:= -Wall -std=c++11 -O0 -g

LIBS				:= -pthread
LIBS_DIR			:=
