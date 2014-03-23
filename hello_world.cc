#include "../node/deps/v8/include/v8.h"

using namespace v8;

int main(int argc, const char *argv[]) {
  Isolate* isolate = Isolate::GetCurrent();

  HandleScope handle_scope(isolate);

  Handle<Context> context = Context::New(isolate);

  Context::Scope context_scope(context);

  Handle<String> source = String::NewFromUtf8(isolate, "'Hello' + ', World!'");

  Handle<Script> script = Script::Compile(source);


  return 0;
}
