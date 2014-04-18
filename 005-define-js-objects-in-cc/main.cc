#include <iostream>
#include <v8.h>

using namespace std;
using namespace v8;

Handle<Value> Multiply(const Arguments& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope;

  Local<Object> self = info.This();

  int x = self->Get(String::New("x"))->Int32Value();
  int y = self->Get(String::New("y"))->Int32Value();

  int factor = info[0]->Int32Value();

  self->Set(String::New("x"), Number::New(x * factor));
  self->Set(String::New("y"), Number::New(y * factor));

  // support chaining
  return self;
}

typedef Handle<Value> (*FunctionCallback)(const Arguments& info);
void AddFunction(Handle<Object> global, const char* name, FunctionCallback callback) {
    global->Set(String::New(name), FunctionTemplate::New(callback)->GetFunction());
}

Handle<String> GetScript(Isolate* isolate) {
  string src =
    "'use strict';"
    ""
    "var p = new Point(1, 2);"
    "p.multiply(4).multiply(2);"
    "'x: ' + p.x + ' y: ' + p.y;";

  const char *js = src.c_str();
  return String::New(js);
}

// dynamically create properties on the Point object when ctor is invoked from js
Handle<Value> PointCtor(const Arguments& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope;

  Handle<ObjectTemplate> t = ObjectTemplate::New();

  Handle<Number> x = info[0]->IsNumber() ? info[0]->ToNumber() : Number::New(0);
  Handle<Number> y = info[1]->IsNumber() ? info[1]->ToNumber() : Number::New(0);

  t->Set(String::New("x"), x);
  t->Set(String::New("y"), y);

  t->Set(String::New("multiply"), FunctionTemplate::New(Multiply));

  return t->NewInstance();
}

int main(int argc, const char *argv[]) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope;
  Handle<Context> context = Context::New();
  Context::Scope context_scope(context);

  // add functions to global context
  Handle<Object> global = context->Global();
  AddFunction(global, "Point", PointCtor);

  // compile and run js
  Handle<String> source = GetScript(isolate);
  Handle<Value> result = Script::Compile(source)->Run();

  cout << "Running script returned: " << *String::Utf8Value(result);

  return 0;
}
