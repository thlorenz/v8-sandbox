CXX         = g++
LINK        = g++
GYP_DEFINES = 

ROOT	  = $(dir $(lastword $(MAKEFILE_LIST)))
V8      = $(ROOT)deps/v8/
V8_REPO = $(V8).gitignore
V8_LIB  = $(V8)out/native/obj.target/tools/gyp/libv8_{base.x64,snapshot}.a
V8_GYP  = $(V8)build/gyp/gyp
V8_ARCH ?= native
V8_VERSION ?= 3.21.17

v8: $(V8_LIB)
	
$(V8_LIB): $(V8_REPO) $(V8_GYP)
	cd $(V8)  && \
	CXX=$(CXX) LINK=$(LINK) GYP_DEFINES=$(GYP_DEFINES) $(MAKE) $(V8_ARCH) -j4 console=readline library=shared snapshot=on i18support=off

$(V8_REPO):
	cd $(ROOT)deps && \
	curl -L https://github.com/v8/v8/archive/$(V8_VERSION).tar.gz | tar xvzf - && \
	mv v8-$(V8_VERSION) v8

# instructions say to `make dependencies`, but all we need is gyp
$(V8_GYP):
	test -d $(V8)build/gyp || (mkdir -p $(V8)build && git clone https://git.chromium.org/external/gyp.git $(V8)build/gyp)

.PHONY: v8
