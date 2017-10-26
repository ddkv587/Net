CC 			:= g++
CFLAGS		:= -g -o3 -std=c++11

RM 			:= rm -rf
MKDIR		:= mkdir -p

CUR_DIR					:= $(shell pwd)
OBJECT_CORE_DIR			:= $(CUR_DIR)/Core
OBJECT_PROTOCOL_DIR		:= $(CUR_DIR)/Protocol
OBJECT_SERVER_DIR		:= $(CUR_DIR)/Server
OBJECT_THREADPOOL_DIR	:= $(CUR_DIR)/ThreadPool
CONFIG_DIR				:= $(CUR_DIR)/Menuconfig

TARGET_OBJECT_MIDDLE	:= $(CUR_DIR)/im_Net/
TARGET_OBJECT_DIR		:= $(CUR_DIR)/target/
