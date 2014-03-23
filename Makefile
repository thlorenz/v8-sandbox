CXX ?= clang++

V8_LIB=/usr/local/Cellar/v8/3.21.17/lib/libv8_{base.x64,snapshot}.a

build:
	$(CXX) -I/usr/local/Cellar/v8/3.21.17/include $(V8_LIB) hello_world.cc  -o hello_world  

run: build
	./hello_world
