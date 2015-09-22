#include "pch.h"
#include "JavaScriptConverter.h"
#include "JavaScriptEngine.h"
#include "JavaScriptPrimitiveValue.h"

using namespace Microsoft::Scripting::JavaScript;

JavaScriptConverter::JavaScriptConverter(JavaScriptEngine^ engine):
    engine_(engine)
{

}

bool JavaScriptConverter::ToBoolean(IJavaScriptValue^ value)
{
    if (value == nullptr)
        throw ref new NullReferenceException();
    JsErrorCode jsErr;
    engine_->ClaimContext();

    JsValueRef boolRef;
    bool result;
    if (value->Type == JavaScriptValueType::Boolean)
    {
        boolRef = GetHandleFromVar(value);
        ObjCheckForFailure(JsBooleanToBool(boolRef, &result));
    }
    else
    {
        ObjCheckForFailure(JsConvertValueToBoolean(GetHandleFromVar(value), &boolRef));
        ObjCheckForFailure(JsBooleanToBool(boolRef, &result));
        JsRelease(boolRef, nullptr);
    }

    return result;
}

IJavaScriptValue^ JavaScriptConverter::FromBoolean(bool value)
{
    // TODO: Investigate whether it is faster to just pass out a reference
    // to engine_->TrueValue or engine_->FalseValue.
    engine_->ClaimContext();
    JsErrorCode jsErr;

    JsValueRef boolRef;
    ObjCheckForFailure(JsBoolToBoolean(value, &boolRef));

    return engine_->CreateValueFromHandle(boolRef);
}

double JavaScriptConverter::ToDouble(IJavaScriptValue^ value)
{
    if (value == nullptr)
        throw ref new NullReferenceException();
    engine_->ClaimContext();

    JsValueRef dblRef;
    double result;
    JsErrorCode jsErr;
    if (value->Type == JavaScriptValueType::Number)
    {
        dblRef = GetHandleFromVar(value);
        ObjCheckForFailure(JsNumberToDouble(dblRef, &result));
    }
    else
    {
        ObjCheckForFailure(JsConvertValueToNumber(GetHandleFromVar(value), &dblRef));
        ObjCheckForFailure(JsNumberToDouble(dblRef, &result));
        JsRelease(dblRef, nullptr);
    }

    return result;
}

IJavaScriptValue^ JavaScriptConverter::FromDouble(double value)
{
    engine_->ClaimContext();

    JsValueRef dblRef;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsDoubleToNumber(value, &dblRef));

    return engine_->CreateValueFromHandle(dblRef);
}

String^ JavaScriptConverter::ToString(IJavaScriptValue^ value)
{
    if (value == nullptr)
        throw ref new NullReferenceException();
    engine_->ClaimContext();

    JsValueRef strRef;
    JsErrorCode jsErr;
    String^ result;
    const wchar_t* str;
    size_t strLen;
    if (value->Type == JavaScriptValueType::String)
    {
        strRef = GetHandleFromVar(value);
        ObjCheckForFailure(JsStringToPointer(strRef, &str, &strLen));
        result = ref new String(str, strLen);
    }
    else if (value->Type == JavaScriptValueType::Symbol)
    {
        result = L"(Symbol)";
    }
    else
    {
        ObjCheckForFailure(JsConvertValueToString(GetHandleFromVar(value), &strRef));
        ObjCheckForFailure(JsStringToPointer(strRef, &str, &strLen));
        result = ref new String(str, strLen);
        JsRelease(strRef, nullptr);
    }

    return result;
}

IJavaScriptValue^ JavaScriptConverter::FromString(String^ value)
{
    engine_->ClaimContext();

    JsValueRef strRef;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsPointerToString(value->Begin(), value->Length(), &strRef));

    return engine_->CreateValueFromHandle(strRef);
}
