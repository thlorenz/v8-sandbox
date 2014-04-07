#include <v8.h>
#include <iostream>
#include <cassert>

using namespace v8;
using namespace std;

void SetArg(string str, unsigned int idx, Handle<Value> argv[]) {
  argv[idx] = String::New(str.c_str());
}

void SetArg(double num, unsigned int idx, Handle<Value> argv[]) {
  argv[idx] = Number::New(num);
}

void SetArg(bool flag, unsigned int idx, Handle<Value> argv[]) {
  argv[idx] = Boolean::New(flag);
}

Handle<Value> CallFunction(string name, Handle<Object> global, unsigned int argc, Handle<Value> argv[]) {
  Handle<Value> wrappedFn = global->Get(String::New(name.c_str()));
  Handle<Function> fn = Handle<Function>::Cast(wrappedFn);

  Handle<Value> result = fn->Call(global, argc, argv);
  assert(!result.IsEmpty() && "calling function failed");
  return result;
}

Handle<String> GetScript() {
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
  return String::New(js, strlen(js));
}

int main(int argc, const char *argv[]) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);

  Handle<Context> context = Context::New(isolate);
  Context::Scope context_scope(context);

  // compile and run js
  Handle<String> source = GetScript();
  Script::Compile(source)->Run();

  // call functions
  Handle<Object> global = context->Global();

  // -- hello
  Handle<Value> result = CallFunction("hello", global, 0, NULL);
  printf("hello returned %s\n", *String::Utf8Value(result));

  // -- multi
  Handle<Value> args[2];
  SetArg(string("hi"), 0, args);
  SetArg(5.0, 1, args);

  result = CallFunction("multi", global, 2, args);
  printf("multi returned %s\n", *String::Utf8Value(result));

  // -- equal(1.0, 1.0)
  SetArg(1.0, 0, args);
  SetArg(1.0, 1, args);

  result = CallFunction("equal", global, 2, args);
  printf("equal(1.0, 1.0) returned %s\n", *String::Utf8Value(result));

  // -- equal(1.0, 2.0)
  SetArg(1.0, 0, args);
  SetArg(2.0, 1, args);

  result = CallFunction("equal", global, 2, args);
  printf("equal(1.0, 2.0) returned %s\n", *String::Utf8Value(result));


  return 0;
}
