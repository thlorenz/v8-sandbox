CXX         = g++
LINK        = g++
GYP_DEFINES = 

ROOT	  = $(dir $(lastword $(MAKEFILE_LIST)))
V8      = $(ROOT)deps/v8
V8_REPO = $(V8)/.gitignore
V8_LIB  = $(V8)/out/native/obj.target/tools/gyp/libv8_{base.x64,snapshot}.a
V8_GYP  = $(V8)/build/gyp/gyp
V8_ARCH ?= native

v8: $(V8_LIB)
	
$(V8_LIB): $(V8_REPO) $(V8_GYP)
	cd $(V8)  && \
	CXX=$(CXX) LINK=$(LINK) GYP_DEFINES=$(GYP_DEFINES) $(MAKE) $(V8_ARCH) -j4 console=readline

$(V8_REPO):
	cd $(ROOT)deps && \
	curl -L https://github.com/v8/v8/archive/3.26.19.tar.gz | tar xvzf - && \
	mv v8-3.26.19 v8

$(V8_GYP):
	cd $(V8) && CXX=$(CXX) LINK=$(LINK) GYP_DEFINES=$(GYP_DEFINES) $(MAKE) dependencies

.PHONY: v8
