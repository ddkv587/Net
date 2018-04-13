include $(CLEAR_VARS)

TARGET_MODULE := Processor

CPP_INCLUDE		:= -I$(ROOT_PATH) \
				   -I.
CPP_SRCDIR		:= $(shell pwd)
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")

LIBS			+= -lCore -lListener
LIBS_DIR		+= -L$(TARGET_PATH)

include $(BUILD_SHARED_LIBRARY)
