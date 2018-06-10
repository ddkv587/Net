RM					:= rm -rf
MKDIR				:= mkdir -p
COPY				:= cp -rf

CXX					:= g++
CPP_FLAGS			:= -DOS_BSD -Wall -std=c++11 -Os -fpermissive -g3
CPP_FLAGS_DEBUG		:= -DOS_BSD -Wall -std=c++11 -O0 -fpermissive -g3

LIBS				:= -pthread -lglog
LIBS_DIR			:=
