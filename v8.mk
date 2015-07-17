ROOT         = $(dir $(lastword $(MAKEFILE_LIST)))
V8           = $(ROOT)v8/
V8_REPO      = $(V8).gitignore
V8_LIB_FILES = libv8_{base,libbase,libplatform,snapshot}.a
V8_LIB       = $(V8)out/Debug/$(V8_LIB_FILES)
V8_TOOLS_DIR = $(ROOT)depot_tools
V8_TOOLS     = $(ROOT)depot_tools/gclient

# force clang++ since g++ thinks certain things like FunctionCallbackInfo are ambiguous whereas clang++ resolves them just fine
CXX         = clang++
CC          = clang
CPP         = clang
LINK        = clang++
CXX_host    = clang++
CC_host     = clang
CPP_host    = clang
LINK_host   = clang++
GYP_DEFINES = "clang=1"

v8: $(V8_LIB)

$(V8_LIB): $(V8_REPO)
	(cd $(V8) && GYP_DEFINES=$(GYP_DEFINES) ./build/gyp_v8 -f ninja && ninja -C out/Debug)

$(V8_REPO): $(V8_TOOLS)
	export PATH=$(V8_TOOLS_DIR):$$PATH
	cd $(ROOT) && fetch v8

$(V8_TOOLS):
	git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git $(V8_TOOLS_DIR)

.PHONY: v8
