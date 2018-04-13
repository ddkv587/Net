#build execute

BUILD_MIDDLE_PATH 	:= $(IM_PATH)/$(TARGET_MODULE)

CPP_APP_OBJECT		:= $(foreach filename, $(CPP_SRC:%.cpp=%.o), $(subst $(CPP_SRCDIR), $(BUILD_MIDDLE_PATH), $(filename)))

all: $(TARGET_MODULE)

$(CPP_APP_OBJECT): $(BUILD_MIDDLE_PATH)%.o: $(CPP_SRCDIR)%.cpp
	@echo $(notdir $<)
	@$(MKDIR) $(dir $@)
	$(CXX) -c $(CPP_INCLUDE) $(CPP_FLAGS) -fPIC -o $@ $<

$(TARGET_MODULE): $(CPP_APP_OBJECT) 
	@$(CXX) $(CPP_APP_OBJECT) $(CPP_FLAGS) -Wl,-rpath,. $(LIBS_DIR) $(LIBS) $(CPP_INCLUDE) -o $(TARGET_PATH)/$@

clean:
	@$(RM) $(BUILD_MIDDLE_PATH)
	@$(RM) $(TARGET_MODULE)

.PHONY: all clean debug release
