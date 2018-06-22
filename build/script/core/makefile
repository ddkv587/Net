include $(CLEAR_VARS)

TARGET_MODULE := Core

CPP_SRCDIR		:= $(SRC_ROOT_PATH)/core
ifeq ($(SYSTEM), OS_BSD)
	CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp" ! -name "MultiEpoll.cpp" ! -name "MultiSelect.cpp")
else
	CPP_SRC			:= $(shell find $(CPP_SRCDIR) -name "*.cpp" ! -name "MultiKqueue.cpp")
endif
CPP_INCLUDE		:= -I$(CPP_SRCDIR)

LIBS			+= -lglog 
LIBS_DIR		+= 

include $(BUILD_SHARED_LIBRARY)
