#include "pch.h"
#include "JavaScriptTypedArray.h"
#include "JavaScriptEngine.h"
#include "JavaScriptObject.h"
#include "JavaScriptPrimitiveValue.h"
#include "JavaScriptConverter.h"

using namespace Microsoft::Scripting::JavaScript;

JavaScriptTypedArray::JavaScriptTypedArray(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object) :
    primitive_(primitive),
    object_(object),
    handle_(GetHandleFromVar(primitive)),
    engine_(primitive->Engine)
{

}

JavaScriptArrayBuffer^ JavaScriptTypedArray::Buffer::get()
{
    return safe_cast<JavaScriptArrayBuffer^>(GetPropertyByName(L"buffer"));
}

uint32 JavaScriptTypedArray::ByteLength::get()
{
    auto val = GetPropertyByName(L"byteLength");
    return (uint32)engine_->Converter->ToDouble(val);
}

uint32 JavaScriptTypedArray::ByteOffset::get()
{
    auto val = GetPropertyByName(L"byteOffset");
    return (uint32)engine_->Converter->ToDouble(val);
}

uint32 JavaScriptTypedArray::Length::get()
{
    auto val = GetPropertyByName(L"length");
    return (uint32)engine_->Converter->ToDouble(val);
}

JavaScriptTypedArrayType JavaScriptTypedArray::ArrayType::get()
{
    std::call_once(gotType_, [&]() {
        BYTE* buf;
        uint32 len;
        int32 elemSize;
        JsTypedArrayType type;
        ObjCheckForFailure1(JsGetTypedArrayStorage(handle_, &buf, &len, &type, &elemSize));

        type_ = (JavaScriptTypedArrayType)type;
    });

    return type_;
}

DECLARE_JAVASCRIPT_OBJECT_IMPLEMENTATION(JavaScriptTypedArray)
