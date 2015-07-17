#include <v8.h>
#include "include/v8.h"
#include "libplatform/libplatform.h"

using namespace v8;

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

    Handle<String> json = String::NewFromUtf8(isolate, "{ \"foo\": \"bar\" }");
    Handle<Value> result = v8::JSON::Parse(json);

    if (result.IsEmpty() || result->IsNull() || !result->IsObject()) {
      fprintf(stderr, "Error: JSON parse didn't return an object");
    } else {
      Local<Object> obj = result.As<Object>();
      Local<Value> foo_key = String::NewFromUtf8(isolate, "foo");
      Local<Value> foo = obj->Get(foo_key);
      String::Utf8Value utf8(foo);
      fprintf(stderr, "foo: %s\n", *utf8);
    }
  }

  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;
  return 0;
}
