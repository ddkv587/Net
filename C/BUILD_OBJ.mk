#build temp obj

BUILD_MIDDLE_PATH 	:= $(IM_PATH)/$(TARGET_MODULE)

CPP_APP_OBJECT		:= $(foreach filename, $(CPP_SRC:%.cpp=%.o), $(subst $(CPP_SRCDIR), $(BUILD_MIDDLE_PATH), $(filename)))

$(CPP_APP_OBJECT): $(BUILD_MIDDLE_PATH)%.o: $(CPP_SRCDIR)%.cpp
	@echo $(notdir $<)
	@$(MKDIR) $(dir $@)
	$(CXX) -c $(CPP_INCLUDE) $(CPPFLAGS) -o $@ $<

.PHONY: all clean debug release
