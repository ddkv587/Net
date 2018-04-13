include $(CLEAR_VARS)

TARGET_MODULE := Demo

CPP_INCLUDE		:= -I$(ROOT_PATH) \
				   -I$(ROOT_PATH)/core \
				   -I$(ROOT_PATH)/listener \
				   -I$(ROOT_PATH)/processor \
				   -I$(ROOT_PATH)/protocol \
				   -I.

CPP_SRCDIR		:= $(shell pwd)
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")

LIBS			+= -lCore -lListener -lProcessor -lProtocol
LIBS_DIR		+= -L$(TARGET_PATH) 

include $(BUILD_EXECUTE)
