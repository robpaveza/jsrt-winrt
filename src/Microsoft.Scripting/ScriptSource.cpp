#include "pch.h"
#include "ScriptSource.h"

using namespace Microsoft::Scripting;
#pragma warning(disable:4307) // Integral constant overflow
volatile JsSourceContext ScriptSource::s_current = JS_SOURCE_CONTEXT_NONE + 2;
#pragma warning(default:4307)

ScriptSource::ScriptSource(String^ sourceLocation, String^ sourceText) :
    sourceLoc_(sourceLocation),
    sourceText_(sourceText)
{
    sourceContextId_ = ::InterlockedIncrement(&ScriptSource::s_current);
}

String^ ScriptSource::SourceLocation::get()
{
    return sourceLoc_;
}

String^ ScriptSource::SourceText::get()
{
    return sourceText_;
}

JsSourceContext ScriptSource::SourceContextId::get()
{
    return sourceContextId_;
}
