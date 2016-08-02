#include <nan.h>
#include "EWAHBitMap.h"

using namespace v8;

void CreateObject(const Nan::FunctionCallbackInfo<Value>& info) {
    info.GetReturnValue().Set(EWAHBitMap::NewInstance(info[0]));
}

void InitAll(Local<Object> exports) {
    EWAHBitMap::Init();
    exports->Set(Nan::New("createObject").ToLocalChecked(),Nan::New<FunctionTemplate>(CreateObject)->GetFunction());
}

NODE_MODULE(addon, InitAll)
