#include "pch.h"
#include "JavaScriptPrimitiveValue.h"
#include "JavaScriptEngine.h"
#include "JavaScriptConverter.h"
#include "JavaScriptObject.h"
#include "JavaScriptFunction.h"
#include "JavaScriptArray.h"
#include "ScriptSource.h"
#include "Errors.h"
#include "JavaScriptSymbol.h"
#include "JavaScriptEngineSymbolRegistry.h"
#include "JavaScriptArrayBuffer.h"
#include "JavaScriptTypedArray.h"
#include "JavaScriptDataView.h"
#include <assert.h>

using namespace ::Platform::Collections;
using namespace Microsoft::Scripting;
using namespace Microsoft::Scripting::JavaScript;

String^ NoMutateJsRuntimeSettings = L"Can't change JavaScriptRuntimeSettings once it has been used to create a runtime.";
String^ EngineDisposed = L"The JavaScriptEngine has been disposed.";

#pragma region Thunks
ExternalObjectFinalizeCallbackThunkData::ExternalObjectFinalizeCallbackThunkData(
    JavaScriptExternalObjectFinalizeCallback^ cb,
    JavaScriptEngine^ eng,
    Object^ obj):
        callback_(cb),
        engine_(eng),
        data_(data)
{
    
}

NativeFunctionThunkData::NativeFunctionThunkData(JavaScriptCallableFunction^ cb, JavaScriptEngine^ eng) :
    callback_(cb),
    engine_(eng)
{

}
#pragma endregion

JavaScriptObject^ JavaScriptEngine::GlobalObject::get()
{
    std::call_once(gotGlobal_, [&]() {
        ClaimContext();

        JsValueRef globalRef;
        EngCheckForFailure1(JsGetGlobalObject(&globalRef));

        global_ = CreateFirmObjectFromHandle(globalRef);
    });
    
    return global_;
}

IJavaScriptValue^ JavaScriptEngine::UndefinedValue::get()
{
    std::call_once(gotUndefined_, [&]() {
        ClaimContext();

        JsValueRef globalRef;
        EngCheckForFailure1(JsGetUndefinedValue(&globalRef));

        undefined_ = CreateValueFromHandle(globalRef);
    });

    return undefined_;
}

JavaScriptObject^ JavaScriptEngine::NullValue::get()
{
    std::call_once(gotNull_, [&]() {
        ClaimContext();

        JsValueRef globalRef;
        EngCheckForFailure1(JsGetNullValue(&globalRef));

        null_ = CreateFirmObjectFromHandle(globalRef);
    });

    return global_;
}

IJavaScriptValue^ JavaScriptEngine::TrueValue::get()
{
    std::call_once(gotUndefined_, [&]() {
        ClaimContext();

        JsValueRef globalRef;
        EngCheckForFailure1(JsGetTrueValue(&globalRef));

        true_ = CreateValueFromHandle(globalRef);
    });

    return true_;
}

IJavaScriptValue^ JavaScriptEngine::FalseValue::get()
{
    std::call_once(gotUndefined_, [&]() {
        ClaimContext();

        JsValueRef globalRef;
        EngCheckForFailure1(JsGetFalseValue(&globalRef));

        false_ = CreateValueFromHandle(globalRef);
    });

    return false_;
}

bool JavaScriptEngine::HasException::get()
{
    ClaimContext();

    bool has;
    EngCheckForFailure1(JsHasException(&has));

    return has;
}

#ifdef USE_EDGEMODE_JSRT
JavaScriptEngineSymbolRegistry^ JavaScriptEngine::Symbol::get()
{
    return symbols_;
}
#endif // USE_EDGEMODE_JSRT

IntPtr JavaScriptEngine::Handle::get()
{
    return IntPtr(context_);
}

JavaScriptEngine::JavaScriptEngine(JsContextRef context, JavaScriptRuntime^ runtime) :
    context_(context),
    runtime_(runtime)
{
    converter_ = ref new JavaScriptConverter(this);

    ClaimContext();
    this->Object_ctor_ = safe_cast<JavaScriptFunction^>(GetGlobalVariable(L"Object"));
    this->Object_keys_ = safe_cast<JavaScriptFunction^>(this->Object_ctor_->GetPropertyByName(L"keys"));
    this->Object_getOwnPropertyNames_ = safe_cast<JavaScriptFunction^>(this->Object_ctor_->GetPropertyByName(L"getOwnPropertyNames"));
    this->Object_seal_ = safe_cast<JavaScriptFunction^>(this->Object_ctor_->GetPropertyByName(L"seal"));
    this->Object_freeze_ = safe_cast<JavaScriptFunction^>(this->Object_ctor_->GetPropertyByName(L"freeze"));
    this->Object_isSealed_ = safe_cast<JavaScriptFunction^>(this->Object_ctor_->GetPropertyByName(L"isSealed"));
    this->Object_isFrozen_ = safe_cast<JavaScriptFunction^>(this->Object_ctor_->GetPropertyByName(L"isFrozen"));
    this->Object_defineProperties_ = safe_cast<JavaScriptFunction^>(this->Object_ctor_->GetPropertyByName(L"defineProperties"));

    this->Array_prototype_ = (safe_cast<JavaScriptFunction^>(GetGlobalVariable(L"Array")))->Prototype;
#ifdef USE_EDGEMODE_JSRT
    this->symbols_ = ref new JavaScriptEngineSymbolRegistry(this);
#endif // USE_EDGEMODE_JSRT
}

void JavaScriptEngine::QueueRelease(JsValueRef handle)
{
    releaseHold_.lock();
    try
    {
        releasedItems_.push_back(handle);
    }
    catch (...) 
    { 
        throw ref new OutOfMemoryException();
    } 
    releaseHold_.unlock();
}

void JavaScriptEngine::ProcessError(JsErrorCode error)
{
    assert(error != JsNoError);

    if (error == JsErrorScriptException)
    {
        RuntimeExceptionRaised(this, nullptr);
        return;
    }

    switch (error)
    {
    case JsErrorInvalidArgument:
    case JsErrorNullArgument:
        throw ref new Exception(E_INVALIDARG);

    case JsErrorInExceptionState:
        throw ref new Exception(E_UNEXPECTED, ERROR_ENGINE_IN_EXCEPTION_STATE);

    case JsErrorNotImplemented:
        throw ref new NotImplementedException();

    case JsErrorWrongThread:
        throw ref new WrongThreadException(ERROR_WRONG_THREAD);

    case JsErrorRuntimeInUse:
        throw ref new Exception(E_UNEXPECTED, ERROR_RUNTIME_IN_USE);

    case JsErrorBadSerializedScript:
        throw ref new Exception(E_INVALIDARG, ERROR_BAD_SERIALIZED_SCRIPT);

    case JsErrorInDisabledState:
        throw ref new Exception(E_UNEXPECTED, ERROR_DISABLED);

    case JsErrorCannotDisableExecution:
    case JsErrorIdleNotEnabled:
        throw ref new Exception(E_UNEXPECTED, ERROR_CONFIG_ERROR);

    case JsErrorArgumentNotObject:
        throw ref new Exception(E_INVALIDARG, ERROR_NOT_OBJECT);

    case JsErrorCannotStartProjection:
        throw ref new Exception(E_FAIL, ERROR_PROJECTION_NOT_STARTED);

    case JsErrorObjectNotInspectable:
        throw ref new Exception(E_INVALIDARG, ERROR_ARG_NOT_INSPECTABLE);

    case JsErrorPropertyNotSymbol:
        throw ref new Exception(E_INVALIDARG, ERROR_PROPERTY_NOT_SYMBOL);

    case JsErrorPropertyNotString:
        throw ref new Exception(E_INVALIDARG, ERROR_PROPERTY_NOT_STRING);

    case JsErrorOutOfMemory:
        throw ref new OutOfMemoryException();

    case JsErrorScriptCompile:
        throw ref new Exception(E_INVALIDARG, ERROR_COMPILATION_FAILED);

    case JsErrorScriptEvalDisabled:
        throw ref new Exception(E_ABORT, ERROR_SCRIPT_ATTEMPTED_EVAL);

    case JsErrorFatal:
        throw ref new Exception(E_FAIL);

    case JsErrorNoCurrentContext:
    case JsErrorHeapEnumInProgress:
    case JsErrorInProfileCallback:
    case JsErrorInThreadServiceCallback:
    case JsErrorCannotSerializeDebugScript:
    case JsErrorAlreadyDebuggingContext:
        // unexpected to occur normally
        assert(false);
        throw ref new Exception(E_FAIL);

    case JsCannotSetProjectionEnqueueCallback:
    case JsErrorInObjectBeforeCollectCallback:
    default:
        throw ref new Exception(E_FAIL);
    }
}

#pragma region publics
JavaScriptEngine::~JavaScriptEngine()
{
    std::for_each(finalizers_.begin(), finalizers_.end(), [](IUnknown* punk) {
        punk->Release();
    });
    finalizers_.clear();

    std::for_each(nativeFuncs_.begin(), nativeFuncs_.end(), [](IUnknown* punk) {
        punk->Release();
    });
    nativeFuncs_.clear();

    releaseHold_.lock();
    std::for_each(releasedItems_.begin(), releasedItems_.end(), [](JsValueRef handle) {
        JsRelease(handle, nullptr);
    });
    releaseHold_.unlock();

    if (context_ != JS_INVALID_REFERENCE)
    {
        JsRelease(context_, nullptr);
        context_ = JS_INVALID_REFERENCE;
    }
    runtime_ = nullptr;
    converter_ = nullptr;
}

JavaScriptFunction^ JavaScriptEngine::EvaluateScriptText(String^ code)
{
    ClaimContext();
    auto src = ref new ScriptSource(L"[eval code]", code);

    return Evaluate(src);
}

JavaScriptFunction^ JavaScriptEngine::Evaluate(ScriptSource^ source)
{
    if (source == nullptr)
        throw ref new NullReferenceException();
    ClaimContext();

    auto loc = source->SourceLocation;
    auto text = source->SourceText;
    auto id = source->SourceContextId;

    JsValueRef resultRef;
    EngCheckForFailure1(JsParseScript(text->Begin(), id, loc->Begin(), &resultRef));

    return CreateFunctionFromHandle(resultRef);
}

JavaScriptFunction^ JavaScriptEngine::Evaluate(ScriptSource^ source, const Array<uint8>^ compiledCode)
{
    if (source == nullptr || compiledCode == nullptr)
        throw ref new NullReferenceException();
    ClaimContext();

    auto loc = source->SourceLocation;
    auto text = source->SourceText;
    auto id = source->SourceContextId;

    JsValueRef resultRef;
    EngCheckForFailure1(JsParseSerializedScript(text->Begin(), compiledCode->Data, id, loc->Begin(), &resultRef));

    return CreateFunctionFromHandle(resultRef);
}

Array<uint8>^ JavaScriptEngine::Compile(ScriptSource^ source)
{
    if (source == nullptr)
        throw ref new NullReferenceException();
    ClaimContext();

    auto text = source->SourceText;
    unsigned long length = 0;
    JsSerializeScript(text->Begin(), nullptr, &length);

    auto result = ref new Array<uint8>(length);
    EngCheckForFailure1(JsSerializeScript(text->Begin(), result->Data, &length));

    return result;
}

IJavaScriptValue^ JavaScriptEngine::Execute(ScriptSource^ source)
{
    if (source == nullptr)
        throw ref new NullReferenceException();
    ClaimContext();

    auto loc = source->SourceLocation;
    auto text = source->SourceText;
    auto id = source->SourceContextId;

    JsValueRef resultRef;
    EngCheckForFailure1(JsRunScript(text->Begin(), id, loc->Begin(), &resultRef));

    return CreateValueFromHandle(resultRef);
}

IJavaScriptValue^ JavaScriptEngine::Execute(ScriptSource^ source, const Array<uint8>^ compiledCode)
{
    if (source == nullptr || compiledCode == nullptr)
        throw ref new NullReferenceException();
    ClaimContext();

    auto loc = source->SourceLocation;
    auto text = source->SourceText;
    auto id = source->SourceContextId;

    JsValueRef resultRef;
    EngCheckForFailure1(JsRunSerializedScript(text->Begin(), compiledCode->Data, id, loc->Begin(), &resultRef));

    return CreateValueFromHandle(resultRef);
}

JavaScriptObject^ JavaScriptEngine::CreateObject(JavaScriptObject^ prototype)
{
    ClaimContext();

    bool hasPrototype = false;
    JsValueRef prototypeRef = JS_INVALID_REFERENCE;
    JsValueRef resultRef;
    JsErrorCode jsErr;

    if (prototype != nullptr)
    {
        prototypeRef = GetHandleFromVar(prototype);
        hasPrototype = true;
    }

    EngCheckForFailure(JsCreateObject(&resultRef));
    if (hasPrototype) 
    {
        EngCheckForFailure(JsSetPrototype(resultRef, prototypeRef));
    }

    return static_cast<JavaScriptObject^>(CreateObjectFromHandle(resultRef));
}

JavaScriptSymbol^ JavaScriptEngine::CreateSymbol(String^ description)
{
    ClaimContext();

    JsValueRef resultRef;
    auto descJsStr = converter_->FromString(description);
    EngCheckForFailure1(JsCreateSymbol(GetHandleFromVar(descJsStr), &resultRef));

    return CreateSymbolFromHandle(resultRef);
}

JavaScriptObject^ JavaScriptEngine::CreateExternalObject(Object^ externalData, JavaScriptExternalObjectFinalizeCallback^ finalizer)
{
    if (externalData != nullptr && finalizer == nullptr)
        throw ref new NullReferenceException(L"External objects must be paired with a finalizer callback.");

    ClaimContext();
    void* optData = nullptr;
    ExternalObjectFinalizeCallbackThunkData^ thunkData = nullptr;
    if (finalizer != nullptr) 
    {
        thunkData = ref new ExternalObjectFinalizeCallbackThunkData(finalizer, this, externalData);
        optData = reinterpret_cast<void*>(thunkData);
    }

    JsValueRef objRef;
    auto jsErr = JsCreateExternalObject(optData, JavaScriptEngine::ExternalObjectFinalizeCallbackThunk, &objRef);
    if (jsErr == JsNoError && optData)
    {
        auto punk = reinterpret_cast<IUnknown*>(thunkData);
        try
        {
            finalizers_.insert(punk);
            // Need to AddRef here to ensure that a hard reference is kept.  CX will
            // not note the reference count because it only deals with hats, not stars.
            punk->AddRef();
        }
        catch (...)
        {
            // finalizers_.insert threw, so no need to release punk.
            
            throw ref new OutOfMemoryException();
        }
    }

    return CreateFirmObjectFromHandle(objRef);
}

void JavaScriptEngine::RemoveThunk(ExternalObjectFinalizeCallbackThunkData^ data)
{
    auto punk = reinterpret_cast<IUnknown*>(data);
    finalizers_.erase(punk);
    // Explicitly release the value we addref'd when we created it.
    punk->Release();
}

TimeSpan JavaScriptEngine::RunIdleWork()
{
    ClaimContext();

    unsigned int nextTick;
    EngCheckForFailure1(JsIdle(&nextTick));

    auto result = TimeSpan();
    result.Duration = nextTick;
    return result;
}

bool JavaScriptEngine::HasGlobalVariable(String^ name)
{
    ClaimContext();

    auto global = this->GlobalObject;
    return global->HasOwnProperty(name);
}

IJavaScriptValue^ JavaScriptEngine::GetGlobalVariable(String^ name)
{
    ClaimContext();

    auto global = this->GlobalObject;
    return global->GetPropertyByName(name);
}

void JavaScriptEngine::SetGlobalVariable(String^ name, IJavaScriptValue^ value)
{
    if (value == nullptr)
        throw ref new NullReferenceException();
    ClaimContext();

    auto global = this->GlobalObject;
    global->SetPropertyByName(name, value);
}

IJavaScriptValue^ JavaScriptEngine::CallGlobalFunction(String^ functionName, IIterable<IJavaScriptValue^>^ args)
{
    ClaimContext();

    auto global = this->GlobalObject;
    JavaScriptFunction^ fn = safe_cast<JavaScriptFunction^>(global->GetPropertyByName(functionName));
    auto result = fn->Call(global, args);

    return result;
}

void JavaScriptEngine::SetGlobalFunction(String^ functionName, JavaScriptCallableFunction^ nativeFunction)
{
    if (nativeFunction == nullptr)
        throw ref new Exception(E_INVALIDARG);
    ClaimContext();

    auto global = this->GlobalObject;
    auto fn = CreateFunction(nativeFunction);
    global->SetPropertyByName(functionName, fn);
}

JavaScriptArray^ JavaScriptEngine::CreateArray(int32 length)
{
    if (length < 0)
        throw ref new Exception(E_INVALIDARG);
    ClaimContext();

    JsValueRef arrayRef;
    EngCheckForFailure1(JsCreateArray((unsigned int)length, &arrayRef));

    return CreateArrayFromHandle(arrayRef);
}

JavaScriptFunction^ JavaScriptEngine::CreateFunction(JavaScriptCallableFunction^ nativeFunction)
{
    if (nativeFunction == nullptr)
        throw ref new NullReferenceException();

    auto thunk = ref new NativeFunctionThunkData(nativeFunction, this);
    auto punk = reinterpret_cast<IUnknown*>(thunk);

    try
    {
        nativeFuncs_.insert(punk);
        punk->AddRef();
    }
    catch (...)
    {
        // Nothing to do here, AddRef hasn't been called
        throw ref new OutOfMemoryException();
    }

    JsValueRef functionRef;
    EngCheckForFailure1(JsCreateFunction(JavaScriptEngine::NativeCallbackThunk, punk, &functionRef));

    return CreateFunctionFromHandle(functionRef);
}

IJavaScriptValue^ JavaScriptEngine::GetAndClearException()
{
    ClaimContext();

    JsValueRef excRef;
    EngCheckForFailure1(JsGetAndClearException(&excRef));

    return CreateValueFromHandle(excRef);
}

void JavaScriptEngine::SetException(IJavaScriptValue^ exception)
{
    if (exception == nullptr)
        throw ref new Exception(E_INVALIDARG);
    ClaimContext();

    auto varRef = GetHandleFromVar(exception);
    EngCheckForFailure1(JsSetException(varRef));
}

JavaScriptObject^ JavaScriptEngine::CreateError(String^ message)
{
    ClaimContext();

    auto jsStr = converter_->FromString(message);
    auto strRef = GetHandleFromVar(jsStr);

    JsValueRef resultRef;
    EngCheckForFailure1(JsCreateError(strRef, &resultRef));

    return CreateFirmObjectFromHandle(resultRef);
}

JavaScriptObject^ JavaScriptEngine::CreateRangeError(String^ message)
{
    ClaimContext();

    auto jsStr = converter_->FromString(message);
    auto strRef = GetHandleFromVar(jsStr);

    JsValueRef resultRef;
    EngCheckForFailure1(JsCreateRangeError(strRef, &resultRef));

    return CreateFirmObjectFromHandle(resultRef);
}

JavaScriptObject^ JavaScriptEngine::CreateReferenceError(String^ message)
{
    ClaimContext();

    auto jsStr = converter_->FromString(message);
    auto strRef = GetHandleFromVar(jsStr);

    JsValueRef resultRef;
    EngCheckForFailure1(JsCreateReferenceError(strRef, &resultRef));

    return CreateFirmObjectFromHandle(resultRef);
}

JavaScriptObject^ JavaScriptEngine::CreateSyntaxError(String^ message)
{
    ClaimContext();

    auto jsStr = converter_->FromString(message);
    auto strRef = GetHandleFromVar(jsStr);

    JsValueRef resultRef;
    EngCheckForFailure1(JsCreateSyntaxError(strRef, &resultRef));

    return CreateFirmObjectFromHandle(resultRef);
}

JavaScriptObject^ JavaScriptEngine::CreateTypeError(String^ message)
{
    ClaimContext();

    auto jsStr = converter_->FromString(message);
    auto strRef = GetHandleFromVar(jsStr);

    JsValueRef resultRef;
    EngCheckForFailure1(JsCreateTypeError(strRef, &resultRef));

    return CreateFirmObjectFromHandle(resultRef);
}

JavaScriptObject^ JavaScriptEngine::CreateUriError(String^ message)
{
    ClaimContext();
    
    auto jsStr = converter_->FromString(message);
    auto strRef = GetHandleFromVar(jsStr);

    JsValueRef resultRef;
    EngCheckForFailure1(JsCreateURIError(strRef, &resultRef));

    return CreateFirmObjectFromHandle(resultRef);
}
#pragma endregion

#pragma region internals
void JavaScriptEngine::ClaimContext()
{
    ENSURE_VALID_ENGINE();

    auto err = JsSetCurrentContext(context_);
    if (err != JsNoError)
        throw ref new Exception(E_FAIL);

    if (releasedItems_.size() > 0)
    {
        releaseHold_.lock();
        std::for_each(releasedItems_.begin(), releasedItems_.end(), [](JsValueRef item) {
            JsRelease(item, nullptr);
        });
        releasedItems_.clear();
        releaseHold_.unlock();
    }

    try {
        
    }
    catch (std::system_error er)
    {
        OutputDebugStringA(er.what());
    }
}

IJavaScriptValue^ JavaScriptEngine::CreateValueFromHandle(JsValueRef handle)
{
    ClaimContext();

    JsValueType kind;
    EngCheckForFailure1(JsGetValueType(handle, &kind));

    auto primitive = ref new JavaScriptPrimitiveValue(handle, this);
    JavaScriptObject^ objectImpl = nullptr;

    switch (kind)
    {
    case JsArray:
        objectImpl = ref new JavaScriptObject(primitive);
        return ref new JavaScriptArray(primitive, objectImpl);
    case JsFunction:
        objectImpl = ref new JavaScriptObject(primitive);
        return ref new JavaScriptFunction(primitive, objectImpl);
    case JsObject:
    case JsError:
        objectImpl = ref new JavaScriptObject(primitive);
        return objectImpl;
    case JsSymbol:
        objectImpl = ref new JavaScriptObject(primitive);
        return ref new JavaScriptSymbol(primitive, objectImpl);
    case JsArrayBuffer:
        objectImpl = ref new JavaScriptObject(primitive);
        return ref new JavaScriptArrayBuffer(primitive, objectImpl);
    case JsTypedArray:
        objectImpl = ref new JavaScriptObject(primitive);
        return ref new JavaScriptTypedArray(primitive, objectImpl);
    case JsDataView:
        objectImpl = ref new JavaScriptObject(primitive);
        return ref new JavaScriptDataView(primitive, objectImpl);
    case JsBoolean:
    case JsNumber:
    case JsString:
    case JsNull:
    case JsUndefined:
    default:
        return primitive;
    }
}

JavaScriptArray^ JavaScriptEngine::CreateArrayFromHandle(JsValueRef handle)
{
    ClaimContext();

    JsValueType kind;
    EngCheckForFailure1(JsGetValueType(handle, &kind));
    if (kind != JsArray)
        throw ref new Exception(E_INVALIDARG);

    auto primitive = ref new JavaScriptPrimitiveValue(handle, this);
    auto obj = ref new JavaScriptObject(primitive);
    return ref new JavaScriptArray(primitive, obj);
}

JavaScriptFunction^ JavaScriptEngine::CreateFunctionFromHandle(JsValueRef handle)
{
    ClaimContext();

    JsValueType kind;
    EngCheckForFailure1(JsGetValueType(handle, &kind));
    if (kind != JsFunction)
        throw ref new Exception(E_INVALIDARG);

    auto primitive = ref new JavaScriptPrimitiveValue(handle, this);
    auto obj = ref new JavaScriptObject(primitive);
    return ref new JavaScriptFunction(primitive, obj);
}

JavaScriptSymbol^ JavaScriptEngine::CreateSymbolFromHandle(JsValueRef handle)
{
    ClaimContext();

    JsValueType kind;
    EngCheckForFailure1(JsGetValueType(handle, &kind));

    auto primitive = ref new JavaScriptPrimitiveValue(handle, this);
    auto obj = ref new JavaScriptObject(primitive);

    switch (kind)
    {
    case JsSymbol:
        return ref new JavaScriptSymbol(primitive, obj);

    default:
        throw ref new Exception(E_INVALIDARG);
    }
}

IJavaScriptObject^ JavaScriptEngine::CreateObjectFromHandle(JsValueRef handle)
{
    ClaimContext();

    JsValueType kind;
    EngCheckForFailure1(JsGetValueType(handle, &kind));

    auto primitive = ref new JavaScriptPrimitiveValue(handle, this);
    auto obj = ref new JavaScriptObject(primitive);
    switch (kind)
    {
    case JsArray:
        return ref new JavaScriptArray(primitive, obj);
    case JsFunction:
        return ref new JavaScriptFunction(primitive, obj);
    case JsObject:
    case JsError:
        return obj;
    case JsSymbol:
        return ref new JavaScriptSymbol(primitive, obj);
    case JsArrayBuffer:
        return ref new JavaScriptArrayBuffer(primitive, obj);
    case JsTypedArray:
        return ref new JavaScriptTypedArray(primitive, obj);
    case JsDataView:
        return ref new JavaScriptDataView(primitive, obj);
    case JsBoolean:
    case JsNumber:
    case JsString:
    case JsNull:
    case JsUndefined:
    default:
        throw ref new Exception(E_INVALIDARG);
    }
}

JavaScriptObject^ JavaScriptEngine::CreateFirmObjectFromHandle(JsValueRef handle)
{
    ClaimContext();

    JsValueType kind;
    EngCheckForFailure1(JsGetValueType(handle, &kind));

    JavaScriptPrimitiveValue^ primitive = nullptr;
    switch (kind)
    {
    case JsObject:
    case JsError:
        primitive = ref new JavaScriptPrimitiveValue(handle, this);
        return ref new JavaScriptObject(primitive);
    case JsArray:
    case JsBoolean:
    case JsFunction:
    case JsNumber:
    case JsString:
    case JsNull:
    case JsUndefined:
    case JsArrayBuffer:
    case JsTypedArray:
    case JsDataView:
    default:
        throw ref new Exception(E_INVALIDARG);
    }
}

void CALLBACK JavaScriptEngine::ExternalObjectFinalizeCallbackThunk(void* data)
{
    if (!data)
        return;

    auto springboard = reinterpret_cast<ExternalObjectFinalizeCallbackThunkData^>(data);
    auto callback = springboard->callback;
    auto inspectable = springboard->data;
    auto engine = springboard->engine;

    engine->RemoveThunk(springboard);

    callback->Invoke(inspectable);
}

JsValueRef CALLBACK JavaScriptEngine::NativeCallbackThunk(JsValueRef callee, bool asConstructor, JsValueRef* args, unsigned short argCount, void* data)
{
    if (!data)
        return JS_INVALID_REFERENCE;

    auto thunk = reinterpret_cast<NativeFunctionThunkData^>(data);
    auto callback = thunk->Target;
    auto engine = thunk->Engine;

    IJavaScriptValue^ thisObj;
    Vector<IJavaScriptValue^>^ argsList = ref new Vector<IJavaScriptValue^>();

    if (argCount > 0)
    {
        thisObj = engine->CreateValueFromHandle(*args);

        args++;
        argCount--;

        while (argCount > 0)
        {
            auto arg = engine->CreateValueFromHandle(*args);
            argsList->Append(arg);
            args++;
            argCount--;
        }
    }

    try
    {
        auto result = callback->Invoke(engine, asConstructor, thisObj, argsList->GetView());
        return GetHandleFromVar(result);
    }
    catch (Exception^ e)
    {
        auto err = engine->CreateError(e->Message);
        engine->SetException(err);

        return JS_INVALID_REFERENCE;
    }
}
#pragma endregion
