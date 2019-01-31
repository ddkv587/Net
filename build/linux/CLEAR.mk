RM					:= rm -rf
MKDIR				:= mkdir -p
COPY				:= cp -rf
SYSTEM				:= 

CXX					:= g++
CPP_FLAGS			:= -Wall -std=c++11 -Os -fpermissive -g3
CPP_FLAGS_DEBUG		:= -Wall -std=c++11 -O0 -fpermissive -g3

LIBS				:= -pthread -lglog
LIBS_DIR			:=
