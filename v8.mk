CXX         = g++
LINK        = g++
GYP_DEFINES = 

ROOT	  = $(dir $(lastword $(MAKEFILE_LIST)))
V8      = $(ROOT)/deps/v8
V8_REPO = $(V8)/.gitignore
V8_LIB  = $(V8)/out/native/obj.target/tools/gyp/libv8_{base.x64,snapshot}.a
V8_GYP  = $(V8)/build/gyp/gyp

v8: $(V8_LIB)
	
$(V8_LIB): $(V8_REPO) $(V8_GYP)
	cd $(V8) && \
	CXX=$(CXX) LINK=$(LINK) GYP_DEFINES=$(GYP_DEFINES) werror=no $(MAKE) native -j4 library=shared snapshot=on console=readline 
	#i18nsupport=off

$(V8_REPO):
	cd $(ROOT)/deps &&\
		curl -L https://github.com/v8/v8/archive/3.26.19.tar.gz | tar xvzf - && \
		mv v8-3.26.19 v8

# `make dependencies` pulls in ICU, but all we need is gyp
$(V8_GYP):
	cd $(V8) && \
	CXX=$(CXX) LINK=$(LINK) GYP_DEFINES=$(GYP_DEFINES) werror=no $(MAKE) dependencies
	#test -d ./build/gyp || (mkdir -p ./build && git clone https://git.chromium.org/external/gyp.git ./build/gyp)


.PHONY: v8
