include $(CLEAR_VARS)

TARGET_MODULE := Core

CPP_INCLUDE		:= -I.
CPP_SRCDIR		:= $(shell pwd)
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")

LIBS			+= -lglog 
LIBS_DIR		+= 

include $(BUILD_SHARED_LIBRARY)
