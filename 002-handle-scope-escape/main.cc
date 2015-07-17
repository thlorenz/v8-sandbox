#include <iostream>
#include "include/v8.h"
#include "libplatform/libplatform.h"

using namespace v8;

Handle<Array> getItems(int x, int y, int z) {
  Isolate* isolate = Isolate::GetCurrent();
  EscapableHandleScope handle_scope(isolate);

  Local<Array> items = Array::New(isolate, 3);

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
  return handle_scope.Escape(items);
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

    Handle<Array> items = getItems(3, 4, 2);
    fprintf(stderr, "Got %d items\n", items->Length());
  }

  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;
  return 0;
}
