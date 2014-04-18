#include <iostream>
#include <v8.h>

using namespace std;
using namespace v8;

Handle<Value> Print(const Arguments& argv) {

  cout << "printing ";

  for (int i = 0; i < argv.Length(); i++) {
    String::Utf8Value arg(argv[i]);
    cout << *arg << " ";
  }

  cout << endl;
  return Undefined();
}

Handle<Value> Add(const Arguments& argv) {
  cout << "adding ";
  int sum = 0;
  for (int i = 0; i < argv.Length(); i++) {
    if (argv[i]->IsInt32()) {
      String::Utf8Value arg(argv[i]);
      cout << *arg << " ";
      sum += argv[i]->Int32Value();
    } else {
      String::Utf8Value arg(argv[i]);
      cout << endl << "WARN: can only add integers, therefore ignoring " << *arg;
    }
  }

  cout << "sum: " <<  sum << endl;
  return Number::New(sum);
}

Handle<String> GetScript(Isolate* isolate) {
  string src =
    "'use strict';"
    ""
    "print('Hello', 'World');"
    ""
    "var sum1 = add(1, 2, 3, 4, 4.5, 'four', 5)"
    "  , sum2 = add(11, 22, 33);"
    "add(sum1, sum2);";

  const char *js = src.c_str();
  return String::New(js);
}

typedef Handle<Value> (*FunctionCallback)(const Arguments& info);
void AddFunction(Handle<Object> global, const char* name, FunctionCallback callback) {
  global->Set(String::New(name), FunctionTemplate::New(callback)->GetFunction());
}

int main(int argc, const char *argv[]) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope;
  Handle<Context> context = Context::New();
  Context::Scope context_scope(context);

  // add functions to global context
  Handle<Object> global = context->Global();
  AddFunction(global, "print", Print);
  AddFunction(global, "add", Add);

  // compile and run js
  Handle<String> source = GetScript(isolate);
  Handle<Value> result = Script::Compile(source)->Run();

  cout << "Running script returned: " << *String::Utf8Value(result);

  return 0;
}
