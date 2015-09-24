#include "pch.h"
#include "JavaScriptDataView.h"
#include "JavaScriptEngine.h"
#include "JavaScriptConverter.h"
#include "JavaScriptPrimitiveValue.h"
#include "JavaScriptObject.h"
#include "JavaScriptArrayBuffer.h"
#include "JavaScriptFunction.h"

using namespace Microsoft::Scripting::JavaScript;
using namespace Platform::Collections;

JavaScriptDataView::JavaScriptDataView(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object) :
    handle_(GetHandleFromVar(primitive)),
    engine_(primitive->Engine),
    object_(object),
    primitive_(primitive)
{

}

JavaScriptArrayBuffer^ JavaScriptDataView::Buffer::get()
{
    return safe_cast<JavaScriptArrayBuffer^>(GetPropertyByName(L"buffer"));
}

uint32 JavaScriptDataView::ByteLength::get()
{
    auto val = GetPropertyByName(L"byteLength");
    return (uint32)engine_->Converter->ToDouble(val);
}

uint32 JavaScriptDataView::ByteOffset::get()
{
    auto val = GetPropertyByName(L"byteOffset");
    return (uint32)engine_->Converter->ToDouble(val);
}

int16 JavaScriptDataView::GetInt8(uint32 byteOffset)
{
    auto fn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"getInt8"));
    auto args = ref new Vector<IJavaScriptValue^>(2);
    args->Append(this);
    args->Append(engine_->Converter->FromDouble((double)byteOffset));
    return (int8)engine_->Converter->ToDouble(fn->Invoke(args));
}

void JavaScriptDataView::SetInt8(uint32 byteOffset, int16 value)
{
    if (value < -128 || value > 127)
        throw ref new InvalidArgumentException(L"Value must be in the range of a signed byte, -128 to 127 inclusive.");

    auto fn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"setInt8"));
    auto args = ref new Vector<IJavaScriptValue^>(3);
    args->Append(this);
    auto converter = engine_->Converter;
    args->Append(converter->FromDouble((double)byteOffset));
    args->Append(converter->FromDouble((double)value));
    fn->Invoke(args);
}

uint8 JavaScriptDataView::GetUint8(uint32 byteOffset)
{
    auto fn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"getUint8"));
    auto args = ref new Vector<IJavaScriptValue^>(2);
    args->Append(this);
    args->Append(engine_->Converter->FromDouble((double)byteOffset));
    return (uint8)engine_->Converter->ToDouble(fn->Invoke(args));
}

void JavaScriptDataView::SetUint8(uint32 byteOffset, uint8 value)
{
    auto fn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"setUint8"));
    auto args = ref new Vector<IJavaScriptValue^>(3);
    args->Append(this);
    auto converter = engine_->Converter;
    args->Append(converter->FromDouble((double)byteOffset));
    args->Append(converter->FromDouble((double)value));
    fn->Invoke(args);
}

#define DEFINE_JSDV_IMPL(API_NAME, TYPE_NAME) \
TYPE_NAME JavaScriptDataView::Get##API_NAME##(uint32 byteOffset) \
{ \
    return Get##API_NAME##(byteOffset, false); \
} \
TYPE_NAME JavaScriptDataView::Get##API_NAME##(uint32 byteOffset, bool littleEndian) \
{ \
    auto fn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"get##API_NAME##")); \
    auto args = ref new Vector<IJavaScriptValue^>(3); \
    args->Append(this); \
    auto converter = engine_->Converter; \
    args->Append(converter->FromDouble((double)byteOffset)); \
    args->Append(converter->FromBoolean(littleEndian)); \
    return (TYPE_NAME)engine_->Converter->ToDouble(fn->Invoke(args)); \
} \
\
void JavaScriptDataView::Set##API_NAME##(uint32 byteOffset, TYPE_NAME value) \
{ \
    Set##API_NAME##(byteOffset, value, false); \
} \
\
void JavaScriptDataView::Set##API_NAME##(uint32 byteOffset, TYPE_NAME value, bool littleEndian) \
{ \
    auto fn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"set##API_NAME##")); \
    auto args = ref new Vector<IJavaScriptValue^>(4); \
    args->Append(this); \
    auto converter = engine_->Converter; \
    args->Append(converter->FromDouble((double)byteOffset)); \
    args->Append(converter->FromDouble((double)value)); \
    args->Append(converter->FromBoolean(littleEndian)); \
    fn->Invoke(args); \
} 

DEFINE_JSDV_IMPL(Int16, int16)

DEFINE_JSDV_IMPL(Uint16, uint16)

DEFINE_JSDV_IMPL(Int32, int32)

DEFINE_JSDV_IMPL(Uint32, uint32)

DEFINE_JSDV_IMPL(Float32, float32)

DEFINE_JSDV_IMPL(Float64, float64)

DECLARE_JAVASCRIPT_OBJECT_IMPLEMENTATION(JavaScriptDataView)
