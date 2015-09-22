#include "pch.h"
#include "JavaScriptFunction.h"
#include "JavaScriptPrimitiveValue.h"
#include "JavaScriptArray.h"
#include "JavaScriptObject.h"
#include "JavaScriptEngine.h"

#define UINT16_MAX_VALUE 65535

using namespace Microsoft::Scripting::JavaScript;
using namespace Platform::Collections;

JavaScriptFunction::JavaScriptFunction(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object) :
    handle_(GetHandleFromVar(primitive)),
    primitive_(primitive),
    object_(object),
    engine_(primitive->Engine)
{

}

IJavaScriptValue^ JavaScriptFunction::Invoke(IIterable<IJavaScriptValue^>^ arguments)
{
    std::vector<JsValueRef> argRefs;
    auto iter = arguments->First();
    bool hasCurrent;
    do
    {
        hasCurrent = iter->HasCurrent;
        if (hasCurrent)
        {
            argRefs.push_back(GetHandleFromVar(iter->Current));
        }
        iter->MoveNext();
    } while (hasCurrent);

    if (argRefs.size() > UINT16_MAX_VALUE)
        throw ref new Exception(E_INVALIDARG, "Exceeded maximum number of arguments.");

    engine_->ClaimContext();

    JsValueRef resultRef;
    JsValueRef* args = nullptr;
    if (argRefs.size() > 0)
        args = &argRefs[0];
#pragma warning(disable:4267)
    ObjCheckForFailure1(JsCallFunction(handle_, args, argRefs.size(), &resultRef));
#pragma warning(default:4267)

    return engine_->CreateValueFromHandle(resultRef);
}

IJavaScriptObject^ JavaScriptFunction::Construct(IIterable<IJavaScriptValue^>^ arguments)
{
    std::vector<JsValueRef> argRefs;
    auto iter = arguments->First();
    bool hasCurrent;
    do
    {
        hasCurrent = iter->HasCurrent;
        if (hasCurrent)
        {
            argRefs.push_back(GetHandleFromVar(iter->Current));
        }
    } while (hasCurrent);

    if (argRefs.size() > UINT16_MAX_VALUE)
        throw ref new Exception(E_INVALIDARG, "Exceeded maximum number of arguments.");

    engine_->ClaimContext();

    JsValueRef resultRef;
#pragma warning(disable:4267)
    ObjCheckForFailure1(JsConstructObject(handle_, &argRefs[0], argRefs.size(), &resultRef));
#pragma warning(default:4267)

    return engine_->CreateObjectFromHandle(resultRef);
}

JavaScriptFunction^ JavaScriptFunction::Bind(IJavaScriptObject^ thisObject, IIterable<IJavaScriptValue^>^ arguments)
{
    if (thisObject == nullptr)
        thisObject = engine_->NullValue;
   
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(thisObject);

    if (arguments != nullptr)
    {
        auto iter = arguments->First();
        bool hasCurrent;
        do
        {
            hasCurrent = iter->HasCurrent;
            if (hasCurrent)
                args->Append(iter->Current);
        }
        while (hasCurrent);
    }
    
    engine_->ClaimContext();
    auto bindFn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"bind"));
    return safe_cast<JavaScriptFunction^>(bindFn->Invoke(args));
}

IJavaScriptValue^ JavaScriptFunction::Apply(IJavaScriptObject^ thisObject, JavaScriptArray^ arguments)
{
    if (thisObject == nullptr)
        thisObject = engine_->NullValue;

    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(thisObject);
    if (arguments != nullptr)
        args->Append(arguments);

    engine_->ClaimContext();
    auto applyFn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"apply"));
    return applyFn->Invoke(args);
}

IJavaScriptValue^ JavaScriptFunction::Call(IJavaScriptObject^ thisObject, IIterable<IJavaScriptValue^>^ arguments)
{
    if (thisObject == nullptr)
        thisObject = engine_->NullValue;

    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(thisObject);

    if (arguments != nullptr)
    {
        auto iter = arguments->First();
        bool hasCurrent;
        do
        {
            hasCurrent = iter->HasCurrent;
            if (hasCurrent)
                args->Append(iter->Current);
        } while (hasCurrent);
    }

    engine_->ClaimContext();
    auto callFn = safe_cast<JavaScriptFunction^>(GetPropertyByName(L"call"));
    return callFn->Invoke(args);
}

DECLARE_JAVASCRIPT_OBJECT_IMPLEMENTATION(JavaScriptFunction)
