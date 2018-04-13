include $(CLEAR_VARS)

TARGET_MODULE := Log

CPP_INCLUDE		:= -I$(ROOT_PATH) \
				   -I.. \
				   -I.
CPP_SRCDIR		:= $(shell pwd)
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")

LIBS			+= -lglog
LIBS_DIR		+=

include $(BUILD_OBJ)
