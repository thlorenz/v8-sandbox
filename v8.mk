CXX ?= clang++

ROOT	  = $(dir $(lastword $(MAKEFILE_LIST)))
V8      = $(ROOT)/deps/v8
V8_REPO = $(V8)/.gitignore
V8_LIB  = $(V8)/out/native/libv8_{base.x64,snapshot}.a
V8_GYP  = $(V8)/build/gyp/gyp

v8: $(V8_LIB)
	
$(V8_LIB): $(V8_REPO) $(V8_GYP)
	cd ./deps/v8 && \
	CXX=$(CXX) werror=no $(MAKE) native -j8 library=shared snapshot=on console=readline i18nsupport=off

$(V8_REPO):
	cd deps && curl -L https://github.com/v8/v8/archive/3.24.35.28.tar.gz | tar xvzf - &&\
	mv v8-3.24.35.28 v8

# `make dependencies` pulls in ICU, but all we need is gyp
$(V8_GYP):
	cd ./deps/v8 && \
	test -d ./build/gyp || (mkdir -p ./build && git clone https://git.chromium.org/external/gyp.git ./build/gyp)

.PHONY: v8
