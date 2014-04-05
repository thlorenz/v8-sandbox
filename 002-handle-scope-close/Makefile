# linked to from dir one level below
include ../v8.mk

CXX ?= clang++

build: 
	$(CXX) -I$(V8)/include $(V8_LIB) main.cc -o main  

run: build
	echo $(V8_LIB)
	./main

clean:
	rm -f ./main

.PHONY: clean build run
