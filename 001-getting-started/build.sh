#!/usr/bin/env bash

g++ -I../deps/v8/include main.cc -o main -Wl,\
--start-group ../deps/v8/out/native/obj.target/{tools/gyp/libv8_{base.x64,snapshot},third_party/icu/libicu{uc,i18n,data}}.a -Wl,\
--end-group -lrt
