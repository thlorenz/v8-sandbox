#include <iostream>
#include <v8.h>

using namespace std;
using namespace v8;

void AddFunction(Handle<Object> global, const char* name, FunctionCallback callback) {
  global->Set(String::New(name), FunctionTemplate::New(callback)->GetFunction());
}

Handle<String> GetScript() {
  string src =
    "'use strict';"
    ""
    "var p = new Point(1, 2);"
    "p.y = p.y + 1;"
    "p.multiply(4).multiply(2);"
    "'x: ' + p.x + ' y: ' + p.y;";

  const char *js = src.c_str();
  return String::New(js, strlen(js));
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
  Local<Object> self = info.Holder();
  Local<External> external = Local<External>::Cast(self->GetInternalField(0));

  return static_cast<T*>(external->Value());
}

void Multiply(const FunctionCallbackInfo<Value>& args) {
  Local<Object> self = args.This();

  int factor = args[0]->Int32Value();
  Point* point = Unwrap<Point>(args);

  point->multiply(factor);

  // support chaining
  args.GetReturnValue().Set(self);
}

void GetPointX(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Point* p = Unwrap<Point>(info);
  info.GetReturnValue().Set(Number::New(p->_x));
}

void SetPointX(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  Point* p = Unwrap<Point>(info);
  p->_x = value->Int32Value();
}

void GetPointY(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Point* p = Unwrap<Point>(info);
  info.GetReturnValue().Set(Number::New(p->_y));
}

void SetPointY(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  Point* p = Unwrap<Point>(info);
  p->_y = value->Int32Value();
}

// expose accessors to be used from js
void PointCtor(const FunctionCallbackInfo<Value>& args) {
  Handle<ObjectTemplate> t = ObjectTemplate::New();

  // js point object represents one C++ object
  t->SetInternalFieldCount(1);
  t->SetAccessor(String::New("x"), (AccessorGetterCallback) GetPointX, (AccessorSetterCallback) SetPointX);
  t->SetAccessor(String::New("y"), (AccessorGetterCallback) GetPointY, (AccessorSetterCallback) SetPointY);

  t->Set(String::New("multiply"), FunctionTemplate::New(Multiply));

  // initialize point with passed args or (0, 0)
  int x = args[0]->IsNumber() ? args[0]->Int32Value() : 0;
  int y = args[1]->IsNumber() ? args[1]->Int32Value() : 0;

  Point *p = new Point(x, y);
  Local<Object> wrap = t->NewInstance();
  wrap->SetInternalField(0, External::New(p));

  args.GetReturnValue().Set(wrap);
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
