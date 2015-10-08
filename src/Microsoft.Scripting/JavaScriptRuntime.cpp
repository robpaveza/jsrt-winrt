#include "pch.h"
#include "JavaScriptRuntime.h"
#include "JavaScriptRuntimeSettings.h"
#include "JavaScriptEngine.h"
#include <assert.h>

using namespace Microsoft::Scripting::JavaScript;

#ifdef USE_EDGEMODE_JSRT
JavaScriptRuntime::JavaScriptRuntime():
    JavaScriptRuntime(ref new JavaScriptRuntimeSettings())
{

}
#endif // USE_EDGEMODE_JSRT

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

    InitMemoryCallback();
}

void JavaScriptRuntime::InitMemoryCallback()
{
    // no need to go about adding a ref to the thunk here - 
    // this will stay alive as long as the runtime is alive.
    allocationThunkData_ = ref new AllocationCallbackThunkData(this);

    JsErrorCode errorCode = JsSetRuntimeMemoryAllocationCallback(runtime_, reinterpret_cast<void*>(allocationThunkData_), JavaScriptRuntime::MemoryCallbackThunk);
    if (errorCode != JsNoError)
        throw ref new Exception(E_FAIL);
}

JavaScriptRuntime::~JavaScriptRuntime()
{
    if (runtime_ != JS_INVALID_RUNTIME_HANDLE)
    {
        JsSetRuntimeMemoryAllocationCallback(runtime_, nullptr, nullptr);
        JsDisposeRuntime(runtime_);
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

bool CALLBACK JavaScriptRuntime::MemoryCallbackThunk(_In_opt_ void *callbackState, _In_ JsMemoryEventType allocationEvent, _In_ size_t allocationSize)
{
    try 
    {
        IInspectable* insp = reinterpret_cast<IInspectable*>(callbackState);
        auto data = reinterpret_cast<AllocationCallbackThunkData^>(insp);
        if (data == nullptr) 
        {
            assert(false);
            // Todo: investigate cause of this
            return true;
        }

        auto runtime = data->runtime;
        assert(runtime != nullptr);

        auto args = ref new JavaScriptMemoryAllocationEventArgs((JavaScriptMemoryAllocationEventType)allocationEvent, allocationSize);
        runtime->MemoryChanging(runtime, args);
        if (args->Cancel && args->IsCancelable)
            return false;

        return true;
    }
    catch (...)
    {
        ::OutputDebugString(L"Structured exception raised during callback to JavaScriptRuntime::MemoryChanging event.");
        assert(false);

        return true;
    }
}
