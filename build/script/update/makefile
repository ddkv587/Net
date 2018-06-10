include $(CLEAR_VARS)

TARGET_MODULE := Update

CPP_SRCDIR		:= $(SRC_ROOT_PATH)/update
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")
CPP_INCLUDE		:= -I$(SRC_ROOT_PATH) \
				   -I$(CPP_SRCDIR)

LIBS			+= -lCore
LIBS_DIR		+= -L$(TARGET_PATH)

include $(BUILD_SHARED_LIBRARY)
