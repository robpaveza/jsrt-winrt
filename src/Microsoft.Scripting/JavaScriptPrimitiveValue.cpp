#include "pch.h"
#include "JavaScriptPrimitiveValue.h"
#include "JavaScriptEngine.h"
#include "JavaScriptConverter.h"

using namespace Microsoft::Scripting::JavaScript;

JavaScriptPrimitiveValue::JavaScriptPrimitiveValue(JsValueRef jsRef, JavaScriptEngine^ engine) :
    handle_(jsRef),
    engine_(engine)
{
    ObjCheckForFailure1(JsAddRef(handle_, nullptr));
}

JavaScriptPrimitiveValue::~JavaScriptPrimitiveValue()
{
    if (handle_ != JS_INVALID_REFERENCE)
    {
        engine_->QueueRelease(handle_);
        handle_ = JS_INVALID_REFERENCE;
    }
}

JavaScriptValueType JavaScriptPrimitiveValue::Type::get()
{
    engine_->ClaimContext();

    JsValueType type;
    ObjCheckForFailure1(JsGetValueType(handle_, &type));

    switch (type)
    {
    case JsArray:
        return JavaScriptValueType::Array;
    case JsBoolean:
        return JavaScriptValueType::Boolean;
    case JsUndefined:
        return JavaScriptValueType::Undefined;
    case JsNumber:
        return JavaScriptValueType::Number;
    case JsString:
        return JavaScriptValueType::String;
    case JsFunction:
        return JavaScriptValueType::Function;

    case JsObject:
    case JsNull:
    case JsError:
    default:
        return JavaScriptValueType::Object;
    }
}

bool JavaScriptPrimitiveValue::IsTruthy::get()
{
    engine_->ClaimContext();
    return engine_->Converter->ToBoolean(this);
}

JavaScriptEngine^ JavaScriptPrimitiveValue::Engine::get()
{
    return engine_;
}

String^ JavaScriptPrimitiveValue::ToString()
{
    engine_->ClaimContext();
    return engine_->Converter->ToString(this);
}

bool JavaScriptPrimitiveValue::SimpleEquals(IJavaScriptValue^ other)
{
    engine_->ClaimContext();

    auto otherRef = GetHandleFromVar(other);
    bool result;
    ObjCheckForFailure1(JsEquals(handle_, otherRef, &result));

    return result;
}

bool JavaScriptPrimitiveValue::StrictEquals(IJavaScriptValue^ other)
{
    engine_->ClaimContext();
    auto otherRef = GetHandleFromVar(other);

    bool result;
    ObjCheckForFailure1(JsStrictEquals(handle_, otherRef, &result));

    return result;
}
