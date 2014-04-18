#include <iostream>
#include <v8.h>

using namespace v8;

Handle<Array> getItems(int x, int y, int z) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);

  Handle<Array> items = Array::New(isolate, 3);

  // did we encounter an error? -> bail
  if (items.IsEmpty()){
    std::cerr << "failed to create proper array" << std::endl;
    return Handle<Array>();
  }

  items->Set(0, Integer::New(isolate, x));
  items->Set(1, Integer::New(isolate, y));
  items->Set(2, Integer::New(isolate, z));

  // Close method copies the value of its argument into the enclosing scope,
  // deletes all its local handles and then gives back the new handle copy
  //
  // handle_scope.Close doesn't exist anymore and I'm not sure what it got replaced with
  // return handle_scope.Close(items);
  return items;
}


int main(int argc, const char *argv[]) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);
  Handle<Context> context = Context::New(isolate);
  Context::Scope context_scope(context);

  Handle<Array> items = getItems(3, 4, 2);

  return 0;
}
