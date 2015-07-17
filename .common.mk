# linked to from dir one level below
include ../v8.mk

build:
	$(CXX) -I $(V8) -I$(V8)include                                                    \
	-L$(V8)out/Debug/                                                                 \
	-lv8_base -lv8_libbase -lv8_libplatform -lv8_snapshot -licudata -licuuc -licui18n \
	-stdlib=libstdc++ -std=c++11                                                      \
	main.cc -o main

run: build
	./main

clean:
	rm -f ./main

.PHONY: clean build run
