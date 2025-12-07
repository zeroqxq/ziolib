BUILD_DIR = ./build

CXXFLAGS= -fPIC -c

$(BUILD_DIR)/libzio.so: ziolib.hpp $(BUILD_DIR)
	cp $< ziolib.cpp
	$(CXX) $(CXXFLAGS) ziolib.cpp -o $(BUILD_DIR)/lib.o
	$(CXX) -shared -o $@ $(BUILD_DIR)/lib.o
	rm ziolib.cpp

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
