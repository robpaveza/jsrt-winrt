#include "pch.h"
#include "JavaScriptEngine.h"
#include "JavaScriptEngineSymbolRegistry.h"
#include "JavaScriptFunction.h"
#include "JavaScriptConverter.h"
#include "JavaScriptObject.h"
#include "JavaScriptSymbol.h"

using namespace Platform::Collections;
using namespace Microsoft::Scripting::JavaScript;

JavaScriptEngineSymbolRegistry::JavaScriptEngineSymbolRegistry(JavaScriptEngine^ engine) :
    engine_(engine)
{
    // Constructor assumes it is invoked from the engine's constructor and the 
    // engine's context is current on the thread
    ctor_ = safe_cast<JavaScriptFunction^>(engine_->GetGlobalVariable(L"Symbol"));

    iterator_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"iterator"));

    hasInstance_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"hasInstance"));
    unscopables_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"unscopables"));

    /*match_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"match"));
    replace_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"replace"));
    search_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"search"));
    split_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"split"));

    isConcatSpreadable_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"isConcatSpreadable"));
    species_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"species"));
    toPrimitive_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"toPrimitive"));
    toStringTag_ = safe_cast<JavaScriptSymbol^>(ctor_->GetPropertyByName(L"toStringTag"));*/
}

JavaScriptFunction^ JavaScriptEngineSymbolRegistry::Constructor::get()
{
    return ctor_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::Iterator::get()
{
    return iterator_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::Unscopables::get()
{
    return unscopables_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::HasInstance::get()
{
    return hasInstance_;
}

/*
JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::Match::get()
{
    return match_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::Replace::get()
{
    return replace_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::Search::get()
{
    return search_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::Split::get()
{
    return split_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::IsConcatSpreadable::get()
{
    return isConcatSpreadable_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::Species::get()
{
    return species_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::ToPrimitive::get()
{
    return toPrimitive_;
}

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::ToStringTag::get()
{
    return toStringTag_;
}
*/

JavaScriptSymbol^ JavaScriptEngineSymbolRegistry::For(String^ key)
{
    // ctor_->GetPropertyByName will invoke ClaimContext
    auto forFn = safe_cast<JavaScriptFunction^>(ctor_->GetPropertyByName(L"for"));
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(ctor_);
    args->Append(engine_->Converter->FromString(key));

    auto result = forFn->Invoke(args);
    if (result->SimpleEquals(engine_->NullValue))
        return nullptr;

    return safe_cast<JavaScriptSymbol^>(result);
}

String^ JavaScriptEngineSymbolRegistry::KeyFor(JavaScriptSymbol^ symbol)
{
    // ctor_->GetPropertyByName will invoke ClaimContext
    auto forFn = safe_cast<JavaScriptFunction^>(ctor_->GetPropertyByName(L"keyFor"));
    auto args = ref new Vector<IJavaScriptValue^>();
    args->Append(ctor_);
    args->Append(symbol);

    auto result = forFn->Invoke(args);
    if (result->SimpleEquals(engine_->NullValue))
        return nullptr;

    return engine_->Converter->ToString(result);
}
