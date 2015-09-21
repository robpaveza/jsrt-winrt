#include "pch.h"
#include "JavaScriptPrimitiveValue.h"
#include "JavaScriptObject.h"
#include "JavaScriptArray.h"
#include "JavaScriptEngine.h"
#include "JavaScriptConverter.h"
#include "JavaScriptFunction.h"

using namespace Platform::Collections;
using namespace Microsoft::Scripting::JavaScript;

JavaScriptArray::JavaScriptArray(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object) :
    handle_(GetHandleFromVar(primitive)),
    primitive_(primitive),
    object_(object),
    engine_(primitive->Engine)
{

}

int32 JavaScriptArray::Length::get()
{
    engine_->ClaimContext();

    auto prop = GetPropertyByName(L"length");
    return (int)engine_->Converter->ToDouble(prop);
}

IJavaScriptValue^ JavaScriptArray::GetAt(int32 index)
{
    JsValueRef resultRef;
    ObjCheckForFailure1(JsGetIndexedProperty(handle_, GetHandleFromVar(engine_->Converter->FromDouble(index)), &resultRef));

    return engine_->CreateValueFromHandle(resultRef);
}

void JavaScriptArray::SetAt(int32 index, IJavaScriptValue^ value)
{
    ObjCheckForFailure1(JsSetIndexedProperty(handle_, GetHandleFromVar(engine_->Converter->FromDouble(index)), GetHandleFromVar(value)));
}

IJavaScriptValue^ JavaScriptArray::Pop()
{
    auto arg = ref new Vector<IJavaScriptValue^>(1);
    arg->Append(this);
    return safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"pop"))->Invoke(arg);
}

void JavaScriptArray::Push(IJavaScriptValue^ value)
{
    if (value == nullptr)
        value = engine_->NullValue;

    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(value);
    safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"push"))->Invoke(arg);
}

void JavaScriptArray::Reverse()
{
    auto arg = ref new Vector<IJavaScriptValue^>(1);
    arg->Append(this);
    safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"reverse"))->Invoke(arg);
}

IJavaScriptValue^ JavaScriptArray::Shift()
{
    auto arg = ref new Vector<IJavaScriptValue^>(1);
    arg->Append(this);
    return safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"shift"))->Invoke(arg);
}

int32 JavaScriptArray::Unshift(IIterable<IJavaScriptValue^>^ valuesToInsert)
{
    if (valuesToInsert == nullptr)
        throw ref new Exception(E_INVALIDARG);

    auto arg = ref new Vector<IJavaScriptValue^>();
    arg->Append(this);
    auto iter = valuesToInsert->First();
    while (iter->HasCurrent) {
        arg->Append(iter->Current);
        iter->MoveNext();
    }
    return engine_->Converter->ToDouble(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"unshift"))->Invoke(arg));
}

void JavaScriptArray::Sort(JavaScriptFunction^ compareFunction)
{
    auto arg = ref new Vector<IJavaScriptValue^>();
    arg->Append(this);
    if (compareFunction != nullptr)
        arg->Append(compareFunction);

    safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"sort"))->Invoke(arg);
}

JavaScriptArray^ JavaScriptArray::Splice(uint32 index, uint32 numberToRemove, IIterable<IJavaScriptValue^>^ valuesToInsert)
{
    auto arg = ref new Vector<IJavaScriptValue^>();
    arg->Append(this);
    arg->Append(engine_->Converter->FromDouble(index));
    arg->Append(engine_->Converter->FromDouble(numberToRemove));
    if (valuesToInsert != nullptr)
    {
        auto iter = valuesToInsert->First();
        while (iter->HasCurrent)
        {
            arg->Append(iter->Current);
            iter->MoveNext();
        }
    }

    return safe_cast<JavaScriptArray^>(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"splice"))->Invoke(arg));
}

JavaScriptArray^ JavaScriptArray::Concat(IIterable<IJavaScriptValue^>^ itemsToConcatenate)
{
    if (itemsToConcatenate == nullptr)
        throw ref new Exception(E_INVALIDARG);

    auto arg = ref new Vector<IJavaScriptValue^>();
    arg->Append(this);
    auto iter = itemsToConcatenate->First();
    while (iter->HasCurrent)
    {
        arg->Append(iter->Current);
        iter->MoveNext();
    }

    return safe_cast<JavaScriptArray^>(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"concat"))->Invoke(arg));
}

String^ JavaScriptArray::Join(String^ separator)
{
    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(engine_->Converter->FromString(separator));
    
    return engine_->Converter->ToString(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"join"))->Invoke(arg));
}

JavaScriptArray^ JavaScriptArray::Slice(int32 beginning)
{
    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(engine_->Converter->FromDouble(beginning));

    return safe_cast<JavaScriptArray^>(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"slice"))->Invoke(arg));
}

JavaScriptArray^ JavaScriptArray::Slice(int32 beginning, int32 end)
{
    auto arg = ref new Vector<IJavaScriptValue^>(3);
    arg->Append(this);
    arg->Append(engine_->Converter->FromDouble(beginning));
    arg->Append(engine_->Converter->FromDouble(end));

    return safe_cast<JavaScriptArray^>(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"slice"))->Invoke(arg));
}

int32 JavaScriptArray::IndexOf(IJavaScriptValue^ valueToFind)
{
    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(valueToFind);

    return engine_->Converter->ToDouble((safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"indexOf"))->Invoke(arg)));
}

int32 JavaScriptArray::IndexOf(IJavaScriptValue^ valueToFind, int32 startIndex)
{
    auto arg = ref new Vector<IJavaScriptValue^>(3);
    arg->Append(this);
    arg->Append(valueToFind);
    arg->Append(engine_->Converter->FromDouble(startIndex));

    return engine_->Converter->ToDouble((safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"indexOf"))->Invoke(arg)));
}

int32 JavaScriptArray::LastIndexOf(IJavaScriptValue^ valueToFind)
{
    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(valueToFind);

    return engine_->Converter->ToDouble((safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"lastIndexOf"))->Invoke(arg)));
}

int32 JavaScriptArray::LastIndexOf(IJavaScriptValue^ valueToFind, int32 lastIndex)
{
    auto arg = ref new Vector<IJavaScriptValue^>(3);
    arg->Append(this);
    arg->Append(valueToFind);
    arg->Append(engine_->Converter->FromDouble(lastIndex));

    return engine_->Converter->ToDouble((safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"lastIndexOf"))->Invoke(arg)));
}

void JavaScriptArray::ForEach(JavaScriptFunction^ callee)
{
    if (callee == nullptr)
        throw ref new Exception(E_INVALIDARG);

    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(callee);

    safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"forEach"))->Invoke(arg);
}

bool JavaScriptArray::Every(JavaScriptFunction^ predicate)
{
    if (predicate == nullptr)
        throw ref new Exception(E_INVALIDARG);

    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(predicate);

    return engine_->Converter->ToBoolean(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"every"))->Invoke(arg));
}

bool JavaScriptArray::Some(JavaScriptFunction^ predicate)
{
    if (predicate == nullptr)
        throw ref new Exception(E_INVALIDARG);

    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(predicate);

    return engine_->Converter->ToBoolean(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"some"))->Invoke(arg));
}

JavaScriptArray^ JavaScriptArray::Filter(JavaScriptFunction^ predicate)
{
    if (predicate == nullptr)
        throw ref new Exception(E_INVALIDARG);

    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(predicate);

    return safe_cast<JavaScriptArray^>(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"filter"))->Invoke(arg));
}

JavaScriptArray^ JavaScriptArray::Map(JavaScriptFunction^ converter)
{
    if (converter == nullptr)
        throw ref new Exception(E_INVALIDARG);

    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(converter);

    return safe_cast<JavaScriptArray^>(safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"map"))->Invoke(arg));
}

IJavaScriptValue^ JavaScriptArray::Reduce(JavaScriptFunction^ aggregator)
{
    if (aggregator == nullptr)
        throw ref new Exception(E_INVALIDARG);

    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(aggregator);

    return safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"reduce"))->Invoke(arg);
}

IJavaScriptValue^ JavaScriptArray::Reduce(JavaScriptFunction^ aggregator, IJavaScriptValue^ initialValue)
{
    if (aggregator == nullptr)
        throw ref new Exception(E_INVALIDARG);
    if (initialValue == nullptr)
        initialValue = engine_->UndefinedValue;

    auto arg = ref new Vector<IJavaScriptValue^>(3);
    arg->Append(this);
    arg->Append(aggregator);
    arg->Append(initialValue);

    return safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"reduce"))->Invoke(arg);
}

IJavaScriptValue^ JavaScriptArray::ReduceRight(JavaScriptFunction^ aggregator)
{
    if (aggregator == nullptr)
        throw ref new Exception(E_INVALIDARG);

    auto arg = ref new Vector<IJavaScriptValue^>(2);
    arg->Append(this);
    arg->Append(aggregator);

    return safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"reduceRight"))->Invoke(arg);
}

IJavaScriptValue^ JavaScriptArray::ReduceRight(JavaScriptFunction^ aggregator, IJavaScriptValue^ initialValue)
{
    if (aggregator == nullptr)
        throw ref new Exception(E_INVALIDARG);
    if (initialValue == nullptr)
        initialValue = engine_->UndefinedValue;

    auto arg = ref new Vector<IJavaScriptValue^>();
    arg->Append(this);
    arg->Append(aggregator);
    arg->Append(initialValue);

    return safe_cast<JavaScriptFunction^>(engine_->Array_prototype_->GetPropertyByName(L"reduceRight"))->Invoke(arg);
}

DECLARE_JAVASCRIPT_OBJECT_IMPLEMENTATION(JavaScriptArray)
