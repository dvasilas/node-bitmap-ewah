#include <v8.h>
#include <nan.h>
#include <node.h>
#include "EWAHBitMap.h"
#include "src/EWAHBoolArray/headers/ewah.h"

using namespace v8;
using namespace std;
using namespace node;

Nan::Persistent<Function> EWAHBitMap::constructor;
Nan::Persistent<FunctionTemplate> EWAHBitMap::constructor_template;

void EWAHBitMap::Init() {
    Nan::HandleScope scope;

    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("EWAHBitMap").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "push", Push);
    Nan::SetPrototypeMethod(tpl, "set", Set);
    Nan::SetPrototypeMethod(tpl, "unset", Unset);
    Nan::SetPrototypeMethod(tpl, "toString", ToString);
    Nan::SetPrototypeMethod(tpl, "length", Length);
    Nan::SetPrototypeMethod(tpl, "numberOfOnes", NumberOfOnes);
    Nan::SetPrototypeMethod(tpl, "map", Map);
    Nan::SetPrototypeMethod(tpl, "or", Or);
    Nan::SetPrototypeMethod(tpl, "and", And);
    Nan::SetPrototypeMethod(tpl, "xor", Xor);
    Nan::SetPrototypeMethod(tpl, "not", Not);
    Nan::SetPrototypeMethod(tpl, "read", Read);
    Nan::SetPrototypeMethod(tpl, "write", Write);

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
        Nan::ThrowTypeError("Wrong number of arguments");
        return ;
    }
    if (!info[0]->IsNumber()) {
        Nan::ThrowTypeError("Arguments must be a bit position");
        return ;
    }
    ewahbitmap->set(info[0]->NumberValue());
}

NAN_METHOD(EWAHBitMap::Set) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return ;
    }
    if (!info[0]->IsNumber()) {
        Nan::ThrowTypeError("Arguments must be a bit position");
        return ;
    }

    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());

    Handle<Value> tmpInst = EWAHBitMap::NewInstance(info[0]);
    EWAHBitMap* tmpObject = Nan::ObjectWrap::Unwrap<EWAHBitMap>(tmpInst->ToObject());
    tmpObject->set(info[0]->NumberValue());

    Handle<Value> resultInst = EWAHBitMap::NewInstance(info[0]);
    EWAHBitMap* resultObject = Nan::ObjectWrap::Unwrap<EWAHBitMap>(resultInst->ToObject());

    that->getMutableArray().logicalor(tmpObject->getMutableArray(), resultObject->getMutableArray());

    that->setArray(resultObject->getMutableArray());
}

NAN_METHOD(EWAHBitMap::Unset) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return ;
    }
    if (!info[0]->IsNumber()) {
        Nan::ThrowTypeError("Arguments must be a bit position");
        return ;
    }

    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());

    Handle<Value> tmpInst = EWAHBitMap::NewInstance(info[0]);
    EWAHBitMap* tmpObject = Nan::ObjectWrap::Unwrap<EWAHBitMap>(tmpInst->ToObject());
    tmpObject->set(info[0]->NumberValue());

    Handle<Value> resultInst = EWAHBitMap::NewInstance(info[0]);
    EWAHBitMap* resultObject = Nan::ObjectWrap::Unwrap<EWAHBitMap>(resultInst->ToObject());

    if (!that->getImmutableArray().intersects(tmpObject->getImmutableArray())) {
        that->getMutableArray().logicalor(that->getMutableArray(), resultObject->getMutableArray());
    }
    else {
        that->getMutableArray().logicalxor(tmpObject->getMutableArray(), resultObject->getMutableArray());
    }
    that->setArray(resultObject->getMutableArray());
}

NAN_METHOD(EWAHBitMap::ToString) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return ;
    }
    if ((!info[0]->IsString()) || ((info[0]->ToString())->Length() < 1)) {
        Nan::ThrowTypeError("Argument should be a non-empty string");
        return ;
    }

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

    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return ;
    }
    if (!info[0]->IsFunction()) {
        Nan::ThrowTypeError("Wrong type of arguments (argument shoudld be a function)");
        return ;
    }

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

NAN_METHOD(EWAHBitMap::Or) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return ;
    }
    if (!info[0]->IsObject()) {
        Nan::ThrowTypeError("Wrong type of argument");
        return ;
    }

    EWAHBitMap* rightOperand = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info[0]->ToObject());
    if (rightOperand == NULL)
        Nan::ThrowError("Cannot cast arguments to ");

    Handle<Value> resultInst = EWAHBitMap::NewInstance(info[0]);
    EWAHBitMap* resultObject = Nan::ObjectWrap::Unwrap<EWAHBitMap>(resultInst->ToObject());
    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());

    that->getMutableArray().logicalor(rightOperand->getMutableArray(), resultObject->getMutableArray());

    info.GetReturnValue().Set(resultInst);
}

NAN_METHOD(EWAHBitMap::And) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return ;
    }
    if (!info[0]->IsObject()) {
        Nan::ThrowTypeError("Wrong type of argument");
        return ;
    }

    EWAHBitMap* rightOperand = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info[0]->ToObject());
    if (rightOperand == NULL)
        Nan::ThrowError("Cannot cast arguments to ");

    Handle<Value> resultInst = EWAHBitMap::NewInstance(info[0]);
    EWAHBitMap* resultObject = Nan::ObjectWrap::Unwrap<EWAHBitMap>(resultInst->ToObject());
    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());

    that->getMutableArray().logicaland(rightOperand->getMutableArray(), resultObject->getMutableArray());

    info.GetReturnValue().Set(resultInst);
}

NAN_METHOD(EWAHBitMap::Xor) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return ;
    }
    if (!info[0]->IsObject()) {
        Nan::ThrowTypeError("Wrong type of argument");
        return ;
    }

    EWAHBitMap* rightOperand = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info[0]->ToObject());
    if (rightOperand == NULL)
        Nan::ThrowError("Cannot cast arguments to ");

    Handle<Value> resultInst = EWAHBitMap::NewInstance(info[0]);
    EWAHBitMap* resultObject = Nan::ObjectWrap::Unwrap<EWAHBitMap>(resultInst->ToObject());
    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());

    that->getMutableArray().logicalxor(rightOperand->getMutableArray(), resultObject->getMutableArray());

    info.GetReturnValue().Set(resultInst);
}

NAN_METHOD(EWAHBitMap::Not) {
    Nan::HandleScope scope;

    Handle<Value> resultInst = EWAHBitMap::NewInstance(info[0]);
    EWAHBitMap* resultObject = Nan::ObjectWrap::Unwrap<EWAHBitMap>(resultInst->ToObject());
    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());

    that->getMutableArray().logicalnot(resultObject->getMutableArray());

    info.GetReturnValue().Set(resultInst);
}

NAN_METHOD(EWAHBitMap::Write) {
    Nan::HandleScope scope;

    stringstream writebuf;
    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());
    Handle<Value> sizeInBits = Nan::New<Number>(that->getMutableArray().sizeInBits());
    Handle<Value> bufferSize = Nan::New<Number>(that->getMutableArray().bufferSize());
    that->getMutableArray().writeBuffer(writebuf);

    string str = writebuf.str();
    char *binaryBuff = new char[str.length()+1];
    for (size_t i=0; i<str.length(); i++)
        binaryBuff[i] = str[i];

    Local<Value> buffer = Nan::CopyBuffer(binaryBuff, writebuf.str().length()).ToLocalChecked();
    Handle<Array> resultArray = Nan::New<Array>();
    resultArray->Set(0, sizeInBits);
    resultArray->Set(1, bufferSize);
    resultArray->Set(2, buffer);

    info.GetReturnValue().Set(resultArray);
}

NAN_METHOD(EWAHBitMap::Read) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return ;
    }
    if (!info[0]->IsObject()) {
        Nan::ThrowTypeError("Wrong type of argument");
        return ;
    }

    Local<Array> input = Local<Array>::Cast(info[0]);
    size_t sizeInBits = Local<Value>::Cast(input->Get(0))->NumberValue();
    size_t bufferSize = Local<Value>::Cast(input->Get(1))->NumberValue();
    Local<Object> bufferObject = Local<Object>::Cast(input->Get(2));
    char* bufferData = Buffer::Data(bufferObject);
    int bufferLength = Buffer::Length(bufferObject);

    stringstream readbuf;
    for (int i=0; i<bufferLength; i++)
        readbuf << bufferData[i];
    EWAHBitMap* that = Nan::ObjectWrap::Unwrap<EWAHBitMap>(info.This());
    that->getMutableArray().readBuffer(readbuf,bufferSize);
    that->getMutableArray().setSizeInBits(sizeInBits);
}
