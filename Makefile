CXX       := clang++
CXXFLAGS  := -std=c++23 -Wall -Wextra -Wpedantic -Iinclude -Ivendor/eigen
LDFLAGS   := -fuse-ld=lld
AR        := llvm-ar
ARFLAGS   := rcs

BUILD_DIR := build
LIB_DIR   := lib
TARGET    := $(LIB_DIR)/libRFsim.a
TEST_DIR  := tests
TEST_BIN  := $(BUILD_DIR)/tests/run_tests

# Recursively discover implementation files under src/.
SOURCES   := $(shell find src -type f \( -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' \))
OBJECTS   := $(patsubst %.cc,$(BUILD_DIR)/%.o,$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(patsubst %.cxx,$(BUILD_DIR)/%.o,$(SOURCES))))
DEPS      := $(OBJECTS:.o=.d)
TEST_SOURCES := $(shell find $(TEST_DIR) -type f \( -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' \))
TEST_OBJECTS := $(patsubst %.cc,$(BUILD_DIR)/%.o,$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(patsubst %.cxx,$(BUILD_DIR)/%.o,$(TEST_SOURCES))))
TEST_DEPS := $(TEST_OBJECTS:.o=.d)

.PHONY: all test clean
.NOTPARALLEL: all

all: $(TARGET) compile_commands.json

test: $(TEST_BIN)
	$(TEST_BIN)

compile_commands.json: $(SOURCES) $(TEST_SOURCES) Makefile
	bear -- $(MAKE) clean test

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

$(TEST_BIN): $(TEST_OBJECTS) $(TARGET)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# Preserve the src/ subdirectory structure inside build/.
$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: %.cxx
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)
-include $(TEST_DEPS)

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)
