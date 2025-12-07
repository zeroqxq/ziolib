files := $(shell find . -name '*.hpp')
BUILD_DIR = ./build

CXXFLAGS= -fPIC -shared

$(BUILD_DIR)/libzio.so: $(files) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(files) -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
