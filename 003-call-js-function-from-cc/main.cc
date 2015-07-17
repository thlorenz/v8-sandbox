#include "include/v8.h"
#include "libplatform/libplatform.h"
#include <iostream>
#include <cassert>

using namespace v8;
using namespace std;

void SetArg(Isolate* isolate, string str, unsigned int idx, Handle<Value> argv[]) {
  argv[idx] = String::NewFromUtf8(isolate, str.c_str());
}

void SetArg(Isolate* isolate, double num, unsigned int idx, Handle<Value> argv[]) {
  argv[idx] = Number::New(isolate, num);
}

void SetArg(Isolate* isolate, bool flag, unsigned int idx, Handle<Value> argv[]) {
  argv[idx] = Boolean::New(isolate, flag);
}

Handle<Value> CallFunction(Isolate* isolate, string name, Handle<Object> global, unsigned int argc, Handle<Value> argv[]) {
  Handle<Value> wrappedFn = global->Get(String::NewFromUtf8(isolate, name.c_str()));
  Handle<Function> fn = Handle<Function>::Cast(wrappedFn);

  Handle<Value> result = fn->Call(global, argc, argv);
  assert(!result.IsEmpty() && "calling function failed");

  // Close doesn't exist anymore, so we don't create a local handle scope
  // Instead we clean up all values we don't return manually (not sure if this is the correct solution)
  wrappedFn.~Handle();
  fn.~Handle();

  return result;
}

Handle<String> GetScript(Isolate* isolate) {
  string src =
    "'use strict';"
    ""
    "function equal(uno, dos) {"
    "  return uno === dos;"
    "}"
    ""
    "function hello() {"
    "  return 'world';"
    "}"
    ""
    "function multi(s, times) {"
    "  var res = '';"
    "  for (var i = 0; i < times; i++) res = res + s + ' ';"
    "  return res;"
    "}";

  const char *js = src.c_str();
  return String::NewFromUtf8(isolate, js);
}

int main(int argc, const char *argv[]) {
  V8::InitializeICU();
  Platform* platform = platform::CreateDefaultPlatform();
  V8::InitializePlatform(platform);
  V8::Initialize();

  Isolate* isolate = Isolate::New();
  {
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);
    Local<Context> context = Context::New(isolate);
    Context::Scope context_scope(context);

    // compile and run js
    Handle<String> source = GetScript(isolate);
    Script::Compile(source)->Run();

    // call functions
    Handle<Object> global = context->Global();

    // -- hello
    Handle<Value> result = CallFunction(isolate, "hello", global, 0, NULL);
    printf("hello returned %s\n", *String::Utf8Value(result));

    // -- multi
    Handle<Value> args[2];
    SetArg(isolate, string("hi"), 0, args);
    SetArg(isolate, 5.0, 1, args);

    result = CallFunction(isolate, "multi", global, 2, args);
    printf("multi returned %s\n", *String::Utf8Value(result));

    // -- equal(1.0, 1.0)
    SetArg(isolate, 1.0, 0, args);
    SetArg(isolate, 1.0, 1, args);

    result = CallFunction(isolate, "equal", global, 2, args);
    printf("equal(1.0, 1.0) returned %s\n", *String::Utf8Value(result));

    // -- equal(1.0, 2.0)
    SetArg(isolate, 1.0, 0, args);
    SetArg(isolate, 2.0, 1, args);

    result = CallFunction(isolate, "equal", global, 2, args);
    printf("equal(1.0, 2.0) returned %s\n", *String::Utf8Value(result));
  }

  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;


  return 0;
}
