g++ -I../deps/v8/include \
  main.cc -o main \
  ../deps/v8/out/x64.release/lib.target/libicuuc.so \
  ../deps/v8/out/x64.release/lib.target/libicui18n.so \
  ../deps/v8/out/x64.release/lib.target/libv8.so

#  `ls ../deps/v8/out/native/obj.target/{tools/gyp/libv8_{base.x64,snapshot},third_party/icu/libicudata}.a`
  #../deps/v8/out/native/obj.target/third_party/icu/libicudata.a  \
    # `find ../deps/v8 -name *.a`

  #../deps/v8/out/x64.release/obj.target/third_party/icu/libicudata.a \
  #../deps/v8/out/x64.release/obj.target/tools/gyp/libv8_base.x64.a \
  #../deps/v8/out/x64.release/obj.target/tools/gyp/libv8_snapshot.a
