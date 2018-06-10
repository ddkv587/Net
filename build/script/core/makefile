include $(CLEAR_VARS)

TARGET_MODULE := Core

CPP_SRCDIR		:= $(SRC_ROOT_PATH)/core
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")
CPP_INCLUDE		:= -I$(CPP_SRCDIR)

LIBS			+= -lglog 
LIBS_DIR		+= 

include $(BUILD_SHARED_LIBRARY)
