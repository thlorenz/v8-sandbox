#include <iostream>
#include <v8.h>

using namespace std;
using namespace v8;

void Multiply(const FunctionCallbackInfo<Value>& argv) {
  Local<Object> self = argv.This();

  int x = self->Get(String::New("x"))->Int32Value();
  int y = self->Get(String::New("y"))->Int32Value();

  int factor = argv[0]->Int32Value();

  self->Set(String::New("x"), Number::New(x * factor));
  self->Set(String::New("y"), Number::New(y * factor));

  // support chaining
  argv.GetReturnValue().Set(self);
}

void PointCtor(const FunctionCallbackInfo<Value>& argv) {
  Handle<ObjectTemplate> t = ObjectTemplate::New();

  Handle<Number> x = argv[0]->IsNumber() ? argv[0]->ToNumber() : Number::New(0);
  Handle<Number> y = argv[1]->IsNumber() ? argv[1]->ToNumber() : Number::New(0);

  t->Set(String::New("x"), x);
  t->Set(String::New("y"), y);

  t->Set(String::New("multiply"), FunctionTemplate::New(Multiply));

  argv.GetReturnValue().Set(t->NewInstance());
}

Handle<String> GetScript() {
  string src =
    "'use strict';"
    ""
    "var p = new Point(1, 2);"
    "p.multiply(4).multiply(2);"
    "'x: ' + p.x + ' y: ' + p.y;";

  const char *js = src.c_str();
  return String::New(js, strlen(js));
}

void AddFunction(Handle<Object> global, const char* name, FunctionCallback callback) {
  global->Set(String::New(name), FunctionTemplate::New(callback)->GetFunction());
}

int main(int argc, const char *argv[]) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);
  Handle<Context> context = Context::New(isolate);
  Context::Scope context_scope(context);

  // add functions to global context
  Handle<Object> global = context->Global();
  AddFunction(global, "Point", PointCtor);

  // compile and run js
  Handle<String> source = GetScript();
  Handle<Value> result = Script::Compile(source)->Run();

  cout << "Running script returned: " << *String::Utf8Value(result);

  return 0;
}
