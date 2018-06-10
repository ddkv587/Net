include $(CLEAR_VARS)

TARGET_MODULE := Demo

CPP_INCLUDE		:= -I$(SRC_ROOT_PATH) \
				   -I$(SRC_ROOT_PATH)/core \
				   -I$(SRC_ROOT_PATH)/listener \
				   -I$(SRC_ROOT_PATH)/processor \
				   -I$(SRC_ROOT_PATH)/protocol \
				   -I$(SRC_ROOT_PATH)/native 

CPP_SRCDIR		:= $(SRC_ROOT_PATH)/demo
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")

LIBS			+= -ldl -lglog -lCore -lListener -lProcessor -lProtocol -lNative
LIBS_DIR		+= -L$(TARGET_PATH) 

include $(BUILD_EXECUTE)
