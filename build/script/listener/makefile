include $(CLEAR_VARS)

TARGET_MODULE := Listener

CPP_SRCDIR		:= $(SRC_ROOT_PATH)/listener
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")
CPP_INCLUDE		:= -I$(SRC_ROOT_PATH) \
				   -I$(CPP_SRCDIR)

LIBS			+= -lCore -lglog
LIBS_DIR		+= -L$(TARGET_PATH)

include $(BUILD_SHARED_LIBRARY)
