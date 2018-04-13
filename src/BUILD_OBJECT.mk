#build im object

BUILD_MIDDLE_PATH 	:= $(IM_PATH)/$(TARGET_MODULE)

CPP_APP_OBJECT		:= $(foreach filename, $(CPP_SRC:%.cpp=%.o), $(subst $(CPP_SRCDIR), $(BUILD_MIDDLE_PATH), $(filename)))

all: $(CPP_APP_OBJECT)

$(CPP_APP_OBJECT): $(BUILD_MIDDLE_PATH)%.o: $(CPP_SRCDIR)%.cpp
	@echo $(notdir $<)
	@$(MKDIR) $(dir $@)
	$(CXX) -c $(CPP_INCLUDE) $(CPP_FLAGS) -o $@ $<

.PHONY: all clean debug release
