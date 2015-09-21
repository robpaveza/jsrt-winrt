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

    auto prop = GetPropertyByName("length");
    return (int)engine_->Converter->ToDouble(prop);
}

IJavaScriptValue^ JavaScriptArray::GetAt(int32 index)
{
    return nullptr;
}

void JavaScriptArray::SetAt(int32 index, IJavaScriptValue^ value)
{

}

IJavaScriptValue^ JavaScriptArray::Pop()
{
    return nullptr;
}

void JavaScriptArray::Push(IJavaScriptValue^ value)
{

}

void JavaScriptArray::Reverse()
{

}

IJavaScriptValue^ JavaScriptArray::Shift()
{
    return nullptr;
}

int32 JavaScriptArray::Unshift(IIterable<IJavaScriptValue^>^ valuesToInsert)
{
    return -1;
}

void JavaScriptArray::Sort(JavaScriptFunction^ compareFunction)
{

}

JavaScriptArray^ JavaScriptArray::Splice(uint32 index, uint32 numberToRemove, IIterable<IJavaScriptValue^>^ valuesToInsert)
{
    return nullptr;
}

JavaScriptArray^ JavaScriptArray::Concat(IIterable<IJavaScriptValue^>^ itemsToConcatenate)
{
    return nullptr;
}

String^ JavaScriptArray::Join(String^ separator)
{
    return nullptr;
}

JavaScriptArray^ JavaScriptArray::Slice(int32 beginning)
{
    return nullptr;
}

JavaScriptArray^ JavaScriptArray::Slice(int32 beginning, int32 end)
{
    return nullptr;
}

int32 JavaScriptArray::IndexOf(IJavaScriptValue^ valueToFind)
{
    return -1;
}

int32 JavaScriptArray::IndexOf(IJavaScriptValue^ valueToFind, int32 startIndex)
{
    return -1;
}

int32 JavaScriptArray::LastIndexOf(IJavaScriptValue^ valueToFind)
{
    return -1;
}

int32 JavaScriptArray::LastIndexOf(IJavaScriptValue^ valueToFind, int32 lastIndex)
{
    return -1;
}

void JavaScriptArray::ForEach(JavaScriptFunction^ callee)
{

}

bool JavaScriptArray::Every(JavaScriptFunction^ predicate)
{
    return false;
}

bool JavaScriptArray::Some(JavaScriptFunction^ predicate)
{
    return false;
}

JavaScriptArray^ JavaScriptArray::Filter(JavaScriptFunction^ predicate)
{
    return nullptr;
}

JavaScriptArray^ JavaScriptArray::Map(JavaScriptFunction^ converter)
{
    return nullptr;
}

IJavaScriptValue^ JavaScriptArray::Reduce(JavaScriptFunction^ aggregator)
{
    return nullptr;
}

IJavaScriptValue^ JavaScriptArray::Reduce(JavaScriptFunction^ aggregator, IJavaScriptValue^ initialValue)
{
    return nullptr;
}

IJavaScriptValue^ JavaScriptArray::ReduceRight(JavaScriptFunction^ aggregator)
{
    return nullptr;
}

IJavaScriptValue^ JavaScriptArray::ReduceRight(JavaScriptFunction^ aggregator, IJavaScriptValue^ initialValue)
{
    return nullptr;
}

DECLARE_JAVASCRIPT_OBJECT_IMPLEMENTATION(JavaScriptArray)
