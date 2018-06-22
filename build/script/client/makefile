include $(CLEAR_VARS)

TARGET_MODULE := Client

CPP_INCLUDE		:= -I$(SRC_ROOT_PATH) \
				   -I$(SRC_ROOT_PATH)/client \

CPP_SRCDIR		:= $(SRC_ROOT_PATH)/client
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")

LIBS			+= -ldl
LIBS_DIR		+= -L$(TARGET_PATH) 

include $(BUILD_EXECUTE)
