include $(CLEAR_VARS)

TARGET_MODULE := Native

CPP_INCLUDE		:= -I$(SRC_ROOT_PATH) \
				   -I$(SRC_ROOT_PATH)/core \
				   -I$(SRC_ROOT_PATH)/listener \
				   -I$(SRC_ROOT_PATH)/worker \
				   -I$(SRC_ROOT_PATH)/protocol \
				   -I$(SRC_ROOT_PATH)/update \
				   -I$(SRC_ROOT_PATH)/native

CPP_SRCDIR		:= $(SRC_ROOT_PATH)/native
CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")

LIBS			+= -ldl -lglog -lCore -lListener -lWorker -lProtocol
LIBS_DIR		+= -L$(TARGET_PATH) 

include $(BUILD_SHARED_LIBRARY)
