#include <iostream>
#include <v8.h>

using namespace std;
using namespace v8;

typedef Handle<Value> (*FunctionCallback)(const Arguments& info);
void AddFunction(Handle<Object> global, const char* name, FunctionCallback callback) {
  global->Set(String::New(name), FunctionTemplate::New(callback)->GetFunction());
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
  return String::New(js);
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
  HandleScope handle_scope;

  Local<Object> self = info.Holder();
  Local<External> external = Local<External>::Cast(self->GetInternalField(0));

  return static_cast<T*>(external->Value());
}

Handle<Value> Multiply(const Arguments& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope;

  Local<Object> self = info.This();

  int factor = info[0]->Int32Value();
  Point* point = Unwrap<Point>(info);

  point->multiply(factor);

  // support chaining
  return self;
}

Handle<Value> GetPointX(Local<String> property, const AccessorInfo& info) {
  HandleScope handle_scope;
  Point* p = Unwrap<Point>(info);
  return Number::New(p->_x);
}

Handle<Value> SetPointX(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope handle_scope;
  Point* p = Unwrap<Point>(info);
  p->_x = value->Int32Value();
  return Undefined();
}

Handle<Value> GetPointY(Local<String> property, const AccessorInfo& info) {
  HandleScope handle_scope;
  Point* p = Unwrap<Point>(info);
  return Number::New(p->_y);
}

Handle<Value> SetPointY(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope handle_scope;
  Point* p = Unwrap<Point>(info);
  p->_y = value->Int32Value();
  return Undefined();
}

// expose accessors to be used from js
Handle<Value> PointCtor(const Arguments& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope handle_scope;

  Handle<ObjectTemplate> t = ObjectTemplate::New();

  // js point object represents one C++ object
  t->SetInternalFieldCount(1);
  t->SetAccessor(String::New("x"), GetPointX, (AccessorSetter) SetPointX);
  t->SetAccessor(String::New("y"), GetPointY, (AccessorSetter) SetPointY);

  t->Set(String::New("multiply"), FunctionTemplate::New(Multiply));

  // initialize point with passed info or (0, 0)
  int x = info[0]->IsNumber() ? info[0]->Int32Value() : 0;
  int y = info[1]->IsNumber() ? info[1]->Int32Value() : 0;

  Point *p = new Point(x, y);
  Local<Object> wrap = t->NewInstance();
  wrap->SetInternalField(0, External::New(p));

  return wrap;
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
