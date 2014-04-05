#include <v8.h>

using namespace v8;

int main(int argc, const char *argv[]) {
  Isolate* isolate = Isolate::GetCurrent();

  HandleScope handle_scope(isolate);
  Handle<Context> context = Context::New(isolate);
  Context::Scope context_scope(context);

  return 0;
}
