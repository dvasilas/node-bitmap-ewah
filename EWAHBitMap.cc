#include <nan.h>
#include <node.h>
#include "EWAHBitMap.h"
#include "src/EWAHBoolArray/headers/ewah.h"

using namespace v8;
using namespace std;

Nan::Persistent<Function> EWAHBitMap::constructor;
Nan::Persistent<FunctionTemplate> EWAHBitMap::constructor_template;

void EWAHBitMap::Init() {
    Nan::HandleScope scope;

    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("EWAHBitMap").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "push", Push);
    Nan::SetPrototypeMethod(tpl, "toString", ToString);
    Nan::SetPrototypeMethod(tpl, "length", Length);
    Nan::SetPrototypeMethod(tpl, "numberOfOnes", NumberOfOnes);
    Nan::SetPrototypeMethod(tpl, "map", Map);

    constructor_template.Reset(tpl);
    constructor.Reset(tpl->GetFunction());
}

void EWAHBitMap::New(const Nan::FunctionCallbackInfo<Value>& info) {
    Nan::HandleScope scope;

    EWAHBitMap *ewahbitmap = new EWAHBitMap();
    ewahbitmap->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

Local<Object> EWAHBitMap::NewInstance(Local<Value> arg) {
    Nan::EscapableHandleScope scope;
    Local<Value> argv[1] = { arg };
    Local<Function> cons = Nan::New<Function>(constructor);
    Local<Object> instance = cons->NewInstance(1, argv);
    return scope.Escape(instance);
}

NAN_METHOD(EWAHBitMap::Push) {
    Nan::HandleScope scope;

    EWAHBitMap *ewahbitmap = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());
    if (info.Length() < 1) {
        Nan::ThrowError("Wrong number of arguments");
    }
    if (!info[0]->IsNumber()) {
        Nan::ThrowError("Arguments must be a bit position");
    }
    ewahbitmap->set(info[0]->NumberValue());
}

NAN_METHOD(EWAHBitMap::ToString) {
    Nan::HandleScope scope;

    if (info.Length() != 1)
        Nan::ThrowError("Wrong number of arguments");
    if ((!info[0]->IsString()) || ((info[0]->ToString())->Length() < 1))
        Nan::ThrowError("Argument should be a non-empty string");

    Handle<String> delimiter = info[0]->ToString();
    char *strDelimiter = new char[delimiter->Utf8Length()];
    delimiter->WriteUtf8(strDelimiter);

    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());
    const ewahboolarray& ewahBoolArray = that->getImmutableArray();

    stringstream resultStream;
    ewahboolarray_const_iterator it = ewahBoolArray.begin();
    for (resultStream <<  *it++; it != ewahBoolArray.end(); ++it) {
        resultStream << strDelimiter;
        resultStream << *it;
    }
    delete strDelimiter;
    Handle<String> strResult = Nan::New<String>(resultStream.str()).ToLocalChecked();
    info.GetReturnValue().Set(strResult);
}

NAN_METHOD(EWAHBitMap::Length) {
    Nan::HandleScope scope;

    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(that->getImmutableArray().sizeInBits()));
}

NAN_METHOD(EWAHBitMap::NumberOfOnes) {
    Nan::HandleScope scope;

    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(that->getImmutableArray().numberOfOnes()));
}

NAN_METHOD(EWAHBitMap::Map) {
    Nan::HandleScope scope;

    if (info.Length()<1)
        Nan::ThrowError("Wrong number of arguments");
    if (!info[0]->IsFunction())
        Nan::ThrowError("Wrong type of arguments (argument shoudld be a function)");

    Handle<Array> resultArray = Nan::New<Array>();
    Local<Function> callback = Local<Function>::Cast(info[0]);
    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());
    const ewahboolarray& ewahBoolArray = that->getImmutableArray();
	unsigned i = 0;
    for (ewahboolarray_const_iterator it = ewahBoolArray.begin(); it != ewahBoolArray.end(); ++it) {
        Local<Value> info[1] = { Nan::New<Number>(*it) };
        Local<Value> elem = callback->Call(v8::Isolate::GetCurrent()->GetCurrentContext()->Global(), 1, info);
        resultArray->Set(i++, elem);
    }
    info.GetReturnValue().Set(resultArray);
}
