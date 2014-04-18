#include <iostream>
#include <v8.h>

using namespace std;
using namespace v8;

void Multiply(const v8::FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Local<Object> self = info.This();

  int x = self->Get(String::NewFromUtf8(isolate, "x"))->Int32Value();
  int y = self->Get(String::NewFromUtf8(isolate, "y"))->Int32Value();

  int factor = info[0]->Int32Value();

  self->Set(String::NewFromUtf8(isolate, "x"), Number::New(isolate, x * factor));
  self->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, y * factor));

  // support chaining
  info.GetReturnValue().Set(self);
}

void AddFunction(Isolate* isolate, Handle<Object> global, const char* name, v8::FunctionCallback callback) {
  HandleScope handle_scope(isolate);
  global->Set(String::NewFromUtf8(isolate, name), FunctionTemplate::New(isolate, callback)->GetFunction());
}

Handle<String> GetScript(Isolate* isolate) {
  string src =
    "'use strict';"
    ""
    "var p = new Point(1, 2);"
    "p.multiply(4).multiply(2);"
    "'x: ' + p.x + ' y: ' + p.y;";

  const char *js = src.c_str();
  return String::NewFromUtf8(isolate, js);
}

// dynamically create properties on the Point object when ctor is invoked from js
void PointCtor(const v8::FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Handle<ObjectTemplate> t = ObjectTemplate::New();

  Handle<Number> x = info[0]->IsNumber() ? info[0]->ToNumber() : Number::New(isolate, 0);
  Handle<Number> y = info[1]->IsNumber() ? info[1]->ToNumber() : Number::New(isolate, 0);

  t->Set(String::NewFromUtf8(isolate, "x"), x);
  t->Set(String::NewFromUtf8(isolate, "y"), y);

  t->Set(String::NewFromUtf8(isolate, "multiply"), FunctionTemplate::New(isolate, Multiply));

  info.GetReturnValue().Set(t->NewInstance());
}

int main(int argc, const char *argv[]) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);
  Handle<Context> context = Context::New(isolate);
  Context::Scope context_scope(context);

  // add functions to global context
  Handle<Object> global = context->Global();
  AddFunction(isolate, global, "Point", PointCtor);

  // compile and run js
  Handle<String> source = GetScript(isolate);
  Handle<Value> result = Script::Compile(source)->Run();

  cout << "Running script returned: " << *String::Utf8Value(result);

  return 0;
}
