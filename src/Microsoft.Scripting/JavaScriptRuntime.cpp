#include "pch.h"
#include "JavaScriptRuntime.h"
#include "JavaScriptRuntimeSettings.h"
#include "JavaScriptEngine.h"

using namespace Microsoft::Scripting::JavaScript;

JavaScriptRuntime::JavaScriptRuntime(JavaScriptRuntimeSettings^ settings):
    settings_(settings)
{
    if (nullptr == settings)
        throw ref new Platform::NullReferenceException();

    auto attrs = settings->GetRuntimeAttributes();
    
    JsRuntimeHandle handle;
#ifndef USE_EDGEMODE_JSRT
    JsErrorCode errorCode = JsCreateRuntime(attrs, settings->Version, nullptr, &handle);
#else
    JsErrorCode errorCode = JsCreateRuntime(attrs, nullptr, &handle);
#endif // !USE_EDGEMODE_JSRT

    if (errorCode != JsNoError)
        throw ref new Platform::Exception(errorCode);

    settings->SetUsed();
    this->runtime_ = handle;
}

JavaScriptRuntime::~JavaScriptRuntime()
{
    if (runtime_ != JS_INVALID_RUNTIME_HANDLE)
    {
        JsRelease(runtime_, nullptr);
        runtime_ = JS_INVALID_RUNTIME_HANDLE;
    }
}

void JavaScriptRuntime::CollectGarbage()
{
    ENSURE_VALID_RUNTIME();

    auto err = JsCollectGarbage(runtime_);
    if (err != JsNoError)
        throw ref new Exception(E_FAIL);
}

void JavaScriptRuntime::EnableExecution()
{
    ENSURE_VALID_RUNTIME();

    auto err = JsEnableRuntimeExecution(runtime_);
    if (err != JsNoError)
        throw ref new Exception(E_FAIL);
}

void JavaScriptRuntime::DisableExecution()
{
    ENSURE_VALID_RUNTIME();

    auto err = JsDisableRuntimeExecution(runtime_);
    if (err != JsNoError)
        throw ref new Exception(E_FAIL);
}

JavaScriptEngine^ JavaScriptRuntime::CreateEngine()
{
    ENSURE_VALID_RUNTIME();

    JsContextRef engine;
#ifndef USE_EDGEMODE_JSRT
    auto err = JsCreateContext(runtime_, nullptr, &engine);
#else
    auto err = JsCreateContext(runtime_, &engine);
#endif // !USE_EDGEMODE_JSRT

    return ref new JavaScriptEngine(engine, this);
}
