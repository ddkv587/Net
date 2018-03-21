include $(CLEAR_VARS)

TARGET_MODULE := Demo

CPP_INCLUDE		:= -I$(ROOT_PATH) \
				   -I$(ROOT_PATH)/listener \
				   -I$(ROOT_PATH)/multiplex \
				   -I$(ROOT_PATH)/processor \
				   -I.
CPP_SRCDIR		:= $(shell pwd)
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")

LIBS			+=
LIBS_DIR		+=

OBJ_DIR			:= $(IM_PATH)
OBJ_MODULE		:= $(shell find $(OBJ_DIR) -name "*.o")

include $(BUILD_EXECUTE)
