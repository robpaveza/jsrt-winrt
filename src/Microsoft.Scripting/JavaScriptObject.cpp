#include "pch.h"
#include "Common.h"
#include "JavaScriptPrimitiveValue.h"
#include "JavaScriptObject.h"
#include "JavaScriptEngine.h"
#include "JavaScriptFunction.h"
#include "JavaScriptConverter.h"
#include "JavaScriptSymbol.h"

using namespace Platform::Collections;
using namespace Microsoft::Scripting::JavaScript;

#pragma region JavaScriptObject
JavaScriptObject::JavaScriptObject(JavaScriptPrimitiveValue^ delegatedPrimitive) :
    primitive_(delegatedPrimitive),
    engine_(delegatedPrimitive->Engine)
{
    handle_ = GetHandleFromVar(delegatedPrimitive);
}

bool JavaScriptObject::IsPrototypeOf(IJavaScriptObject^ other)
{
    if (other == nullptr)
        throw ref new NullReferenceException();
    engine_->ClaimContext();

    auto fn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"isPrototypeOf"));
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(this);
    args->Append(other);
    auto result = fn->Invoke(args);

    return engine_->Converter->ToBoolean(result);
}

bool JavaScriptObject::PropertyIsEnumerable(String^ propertyName)
{
    engine_->ClaimContext();

    IJavaScriptValue^ jsPropName = engine_->Converter->FromString(propertyName);

    auto fn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"propertyIsEnumerable"));
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(this);
    args->Append(jsPropName);
    auto result = fn->Invoke(args);

    return engine_->Converter->ToBoolean(result);
}

IJavaScriptValue^ JavaScriptObject::GetPropertyByName(String^ propertyName)
{
    engine_->ClaimContext();

    JsPropertyIdRef propertyId;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsGetPropertyIdFromName(propertyName->Begin(), &propertyId));

    JsValueRef resultRef;
    jsErr = JsGetProperty(handle_, propertyId, &resultRef);
    ObjCheckForFailure(jsErr);
    return engine_->CreateValueFromHandle(resultRef);
}

void JavaScriptObject::SetPropertyByName(String^ propertyName, IJavaScriptValue^ value)
{
    engine_->ClaimContext();
    if (value == nullptr)
        value = engine_->NullValue;

    JsValueRef valueToSet = GetHandleFromVar(value);
    JsPropertyIdRef propertyId;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsGetPropertyIdFromName(propertyName->Begin(), &propertyId));
    ObjCheckForFailure(JsSetProperty(handle_, propertyId, valueToSet, false));
}

void JavaScriptObject::DeletePropertyByName(String^ propertyName)
{
    engine_->ClaimContext();

    JsPropertyIdRef propertyId;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsGetPropertyIdFromName(propertyName->Begin(), &propertyId));
    JsValueRef tmpResult;
    ObjCheckForFailure(JsDeleteProperty(handle_, propertyId, false, &tmpResult));
}

#ifdef USE_EDGEMODE_JSRT
IJavaScriptValue^ JavaScriptObject::GetPropertyBySymbol(JavaScriptSymbol^ symbol)
{
    engine_->ClaimContext();

    JsPropertyIdRef propertyId;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsGetPropertyIdFromSymbol(GetHandleFromVar(symbol), &propertyId));

    JsValueRef resultRef;
    ObjCheckForFailure(JsGetProperty(handle_, propertyId, &resultRef));
    return engine_->CreateValueFromHandle(resultRef);
}

void JavaScriptObject::SetPropertyBySymbol(JavaScriptSymbol^ symbol, IJavaScriptValue^ value)
{
    engine_->ClaimContext();
    if (value == nullptr)
        value = engine_->NullValue;

    JsValueRef valueToSet = GetHandleFromVar(value);
    JsPropertyIdRef propertyId;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsGetPropertyIdFromSymbol(GetHandleFromVar(symbol), &propertyId));
    ObjCheckForFailure(JsSetProperty(handle_, propertyId, valueToSet, false));
}

void JavaScriptObject::DeletePropertyBySymbol(JavaScriptSymbol^ symbol)
{
    engine_->ClaimContext();

    JsPropertyIdRef propertyId;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsGetPropertyIdFromSymbol(GetHandleFromVar(symbol), &propertyId));
    JsValueRef tmpResult;
    ObjCheckForFailure(JsDeleteProperty(handle_, propertyId, false, &tmpResult));
}
#endif // USE_EDGEMODE_JSRT

IJavaScriptValue^ JavaScriptObject::GetValueAtIndex(IJavaScriptValue^ index)
{
    if (index == nullptr)
        throw ref new NullReferenceException();
    engine_->ClaimContext();

    JsValueRef indexRef = GetHandleFromVar(index);
    JsValueRef resultRef;
    ObjCheckForFailure1(JsGetIndexedProperty(handle_, indexRef, &resultRef));
    return engine_->CreateValueFromHandle(resultRef);
}

void JavaScriptObject::SetValueAtIndex(IJavaScriptValue^ index, IJavaScriptValue^ value)
{
    if (index == nullptr)
        throw ref new NullReferenceException();
    engine_->ClaimContext();

    if (value == nullptr)
        value = engine_->NullValue;

    auto indexRef = GetHandleFromVar(index);
    auto valueRef = GetHandleFromVar(value);
    ObjCheckForFailure1(JsSetIndexedProperty(handle_, indexRef, valueRef));
}

void JavaScriptObject::DeleteValueAtIndex(IJavaScriptValue^ index)
{
    if (index == nullptr)
        throw ref new NullReferenceException();
    engine_->ClaimContext();

    auto indexRef = GetHandleFromVar(index);
    ObjCheckForFailure1(JsDeleteIndexedProperty(handle_, indexRef));
}

bool JavaScriptObject::HasOwnProperty(String^ propertyName)
{
    engine_->ClaimContext();
    
    auto fn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"hasOwnProperty"));
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(this);
    args->Append(engine_->Converter->FromString(propertyName));

    return engine_->Converter->ToBoolean(fn->Invoke(args));
}

bool JavaScriptObject::HasProperty(String^ propertyName)
{
    engine_->ClaimContext();

    JsPropertyIdRef propertyId;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsGetPropertyIdFromName(propertyName->Begin(), &propertyId));
    bool has;
    ObjCheckForFailure(JsHasProperty(handle_, propertyId, &has));

    return has;
}

JavaScriptObject^ JavaScriptObject::GetOwnPropertyDescriptor(String^ propertyName)
{
    engine_->ClaimContext();

    JsPropertyIdRef propertyId;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsGetPropertyIdFromName(propertyName->Begin(), &propertyId));
    JsValueRef descriptorRef;
    ObjCheckForFailure(JsGetOwnPropertyDescriptor(handle_, propertyId, &descriptorRef));

    return safe_cast<JavaScriptObject^>(engine_->CreateObjectFromHandle(descriptorRef));
}

void JavaScriptObject::DefineProperty(String^ propertyName, IJavaScriptObject^ descriptor)
{
    if (descriptor == nullptr)
        throw ref new NullReferenceException();
    JsValueRef descriptorRef = GetHandleFromVar(descriptor);
    engine_->ClaimContext();

    JsPropertyIdRef propertyId;
    JsErrorCode jsErr;
    ObjCheckForFailure(JsGetPropertyIdFromName(propertyName->Begin(), &propertyId));

    bool set;
    ObjCheckForFailure(JsDefineProperty(handle_, propertyId, descriptorRef, &set));
}

void JavaScriptObject::DefineProperties(IJavaScriptObject^ propertiesContainer)
{
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(engine_->UndefinedValue);
    args->Append(propertiesContainer);

    engine_->Object_defineProperties_->Invoke(args);
}

JavaScriptArray^ JavaScriptObject::GetOwnPropertyNames()
{
    engine_->ClaimContext();

    JsValueRef resultRef;
    ObjCheckForFailure1(JsGetOwnPropertyNames(handle_, &resultRef));

    return engine_->CreateArrayFromHandle(resultRef);
}

JavaScriptArray^ JavaScriptObject::GetOwnPropertySymbols()
{
    engine_->ClaimContext();

    JsValueRef resultRef;
    ObjCheckForFailure1(JsGetOwnPropertySymbols(handle_, &resultRef));

    return engine_->CreateArrayFromHandle(resultRef);
}

void JavaScriptObject::PreventExtensions()
{
    engine_->ClaimContext();

    ObjCheckForFailure1(JsPreventExtension(handle_));
}

void JavaScriptObject::Seal()
{
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(engine_->UndefinedValue);
    args->Append(this);

    engine_->Object_seal_->Invoke(args);
}

void JavaScriptObject::Freeze()
{
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(engine_->UndefinedValue);
    args->Append(this);

    engine_->Object_freeze_->Invoke(args);
}
#pragma endregion 

#pragma region Properties

JavaScriptArray^ JavaScriptObject::Keys::get()
{
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(engine_->UndefinedValue);
    args->Append(this);

    return safe_cast<JavaScriptArray^>(engine_->Object_keys_->Invoke(args));
}

bool JavaScriptObject::IsExtensible::get()
{
    engine_->ClaimContext();
    bool result;
    
    ObjCheckForFailure1(JsGetExtensionAllowed(handle_, &result));
    return result;
}

IJavaScriptObject^ JavaScriptObject::Prototype::get()
{
    engine_->ClaimContext();

    JsValueRef prototypeRef;
    ObjCheckForFailure1(JsGetPrototype(handle_, &prototypeRef));

    return engine_->CreateObjectFromHandle(prototypeRef);
}

void JavaScriptObject::Prototype::set(IJavaScriptObject^ value)
{
    engine_->ClaimContext();

    JsValueRef prototypeRef = GetHandleFromVar(value);
    ObjCheckForFailure1(JsSetPrototype(handle_, prototypeRef));
}

bool JavaScriptObject::IsSealed::get()
{
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(engine_->UndefinedValue);
    args->Append(this);

    return engine_->Converter->ToBoolean(engine_->Object_isSealed_->Invoke(args));
}

bool JavaScriptObject::IsFrozen::get()
{
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(engine_->UndefinedValue);
    args->Append(this);

    return engine_->Converter->ToBoolean(engine_->Object_isFrozen_->Invoke(args));
}

#pragma endregion

#pragma region JavaScriptPrimitiveValue

DECLARE_JAVASCRIPT_PRIMITIVE_VALUE_IMPLEMENTATION(JavaScriptObject)

#pragma endregion 
