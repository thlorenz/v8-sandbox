#include <iostream>
#include <v8.h>

using namespace std;
using namespace v8;

void AddFunction(Isolate* isolate, Handle<Object> global, const char* name, v8::FunctionCallback callback) {
  HandleScope handle_scope(isolate);
  global->Set(String::NewFromUtf8(isolate, name), FunctionTemplate::New(isolate, callback)->GetFunction());
}

Handle<String> GetScript(Isolate* isolate) {
  string src =
    "'use strict';"
    ""
    "var p = new Point(1, 2);"
    "p.y = p.y + 1;"
    "p.multiply(4).multiply(2);"
    "'x: ' + p.x + ' y: ' + p.y;";

  const char *js = src.c_str();
  return String::NewFromUtf8(isolate, js);
}

// Here we define all Point properties and functions in C++
class Point {

public:
  int _x;
  int _y;
  Point(int x = 0, int y = 0) : _x(x), _y(y) {}

  inline void multiply(const double factor) {
    _x *= factor;
    _y *= factor;
  }
};

template <typename T, typename CallbackInfo>
T* Unwrap(const CallbackInfo& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Local<Object> self = info.Holder();
  Local<External> external = Local<External>::Cast(self->GetInternalField(0));

  return static_cast<T*>(external->Value());
}

void Multiply(const v8::FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Local<Object> self = info.This();

  int factor = info[0]->Int32Value();
  Point* point = Unwrap<Point>(info);

  point->multiply(factor);

  // support chaining
  info.GetReturnValue().Set(self);
}

void GetPointX(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  HandleScope handle_scope(info.GetIsolate());
  Point* p = Unwrap<Point>(info);
  info.GetReturnValue().Set(Number::New(info.GetIsolate(), p->_x));
}

void SetPointX(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  HandleScope handle_scope(info.GetIsolate());
  Point* p = Unwrap<Point>(info);
  p->_x = value->Int32Value();
}

void GetPointY(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  HandleScope handle_scope(info.GetIsolate());
  Point* p = Unwrap<Point>(info);
  info.GetReturnValue().Set(Number::New(info.GetIsolate(), p->_y));
}

void SetPointY(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  HandleScope handle_scope(info.GetIsolate());
  Point* p = Unwrap<Point>(info);
  p->_y = value->Int32Value();
}

// expose accessors to be used from js
void PointCtor(const v8::FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Handle<ObjectTemplate> t = ObjectTemplate::New();

  // js point object represents one C++ object
  t->SetInternalFieldCount(1);
  t->SetAccessor(String::NewFromUtf8(isolate, "x"), GetPointX, (AccessorSetterCallback) SetPointX);
  t->SetAccessor(String::NewFromUtf8(isolate, "y"), GetPointY, (AccessorSetterCallback) SetPointY);

  t->Set(String::NewFromUtf8(isolate, "multiply"), FunctionTemplate::New(isolate, Multiply));

  // initialize point with passed info or (0, 0)
  int x = info[0]->IsNumber() ? info[0]->Int32Value() : 0;
  int y = info[1]->IsNumber() ? info[1]->Int32Value() : 0;

  Point *p = new Point(x, y);
  Local<Object> wrap = t->NewInstance();
  wrap->SetInternalField(0, External::New(isolate, p));

  info.GetReturnValue().Set(wrap);
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
