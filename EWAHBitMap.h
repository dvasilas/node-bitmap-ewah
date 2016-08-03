#ifndef EWAHBITMAP_H
#define EWAHBITMAP_H

#include <nan.h>
#include <node.h>
#include "src/EWAHBoolArray/headers/ewah.h"

class EWAHBitMap : public Nan::ObjectWrap {
private:
    typedef EWAHBoolArray<> ewahboolarray;
    typedef ewahboolarray::const_iterator ewahboolarray_const_iterator;
    ewahboolarray boolArray;

    void set(unsigned i) {
        boolArray.set(i);
    }

    const ewahboolarray& getImmutableArray() {
        return boolArray;
    }

    ewahboolarray& getMutableArray() {
        return boolArray;
    }

public:
    static void Init();
    static Nan::Persistent<v8::Function> constructor;
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
    static NAN_METHOD(Push);
    static NAN_METHOD(ToString);
};

#endif
