include $(CLEAR_VARS)

TARGET_MODULE := Core

CPP_INCLUDE   := -I$(SOURCE_ROOT_SERVER)/Core

CPP_SRCDIR    := $(SOURCE_ROOT_SERVER)/Core
PCH_H         := $(SOURCE_ROOT_SERVER)/Core/NET_Core.hpp

CPP_SRC       := $(shell find $(CPP_SRCDIR) -name "*.cpp")

include $(BUILD_SHARED_LIBRARY)