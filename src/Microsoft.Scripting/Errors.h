#pragma once
#include "pch.h"
#include "Common.h"
#include <assert.h>

// planned map:
/*
    JsErrorInvalidArgument          = E_INVALIDARG
    JsErrorNullArgument             = E_INVALIDARG
    JsErrorNoCurrentContext         Should never happen, we always claim context
    JsErrorInExceptionState         = E_UNEXPECTED, ERROR_ENGINE_IN_EXCEPTION_STATE
    JsErrorNotImplemented           E_NOTIMPL
    JsErrorWrongThread              ERROR_WINDOW_OF_OTHER_THREAD, ERROR_WRONG_THREAD
    JsErrorRuntimeInUse             E_UNEXPECTED, ERROR_RUNTIME_IN_USE
    JsErrorBadSerializedScript      E_INVALIDARG, ERROR_BAD_SERIALIZED_SCRIPT
    JsErrorInDisabledState          E_UNEXPECTED, ERROR_DISABLED
    JsErrorCannotDisableExecution   E_UNEXPECTED, ERROR_CONFIG_ERROR
    JsErrorHeapEnumInProgress       Should not happen, we don't support
    JsErrorArgumentNotObject        E_INVALIDARG, ERROR_NOT_OBJECT
    JsErrorInProfileCallback        Should not happen, we don't support
    JsErrorInThreadServiceCallback  Should not happen, we don't support
    JsErrorCannotSerializeDebugScr  Should not happen, we don't support
    JsErrorAlreadyDebuggingContext  E_UNEXPECTED, ERROR_ALREADY_DEBUGGING
    JsErrorAlreadyProfilingContext  Should not happen, we don't support
    JsErrorIdleNotEnabled           E_UNEXPECTED, ERROR_CONFIG_ERROR
    JsCannotSetProjectionEnqueueCa  Unknown what this does
    JsErrorCannotStartProjection    E_FAIL, ERROR_PROJECTION_NOT_STARTED
    JsErrorInObjectBeforeCollectCa  ...Do we support this callback?
    JsErrorObjectNotInspectable     E_INVALIDARG, ERROR_ARG_NOT_INSPECTABLE
    JsErrorPropertyNotSymbol        E_INVALIDARG, ERROR_PROPERTY_NOT_SYMBOL
    JsErrorPropertyNotString        E_INVALIDARG, ERROR_PROPERTY_NOT_STRING

    JsErrorOutOfMemory              E_OUTOFMEMORY

    JsErrorScriptException          Turn into success case
    JsErrorScriptCompile            E_INVALIDARG, ERROR_COMPILATION_FAILED
    JsErrorScriptTerminated         Turn into success case
    JsErrorScriptEvalDisabled       E_ABORT, ERROR_SCRIPT_ATTEMPTED_EVAL

    JsErrorFatal                    E_FAIL
*/
extern String^ ERROR_ENGINE_IN_EXCEPTION_STATE;
extern String^ ERROR_WRONG_THREAD;
extern String^ ERROR_RUNTIME_IN_USE;
extern String^ ERROR_BAD_SERIALIZED_SCRIPT;
extern String^ ERROR_DISABLED;
extern String^ ERROR_CONFIG_ERROR;
extern String^ ERROR_NOT_OBJECT;
extern String^ ERROR_PROJECTION_NOT_STARTED;
extern String^ ERROR_ARG_NOT_INSPECTABLE;
extern String^ ERROR_PROPERTY_NOT_SYMBOL;
extern String^ ERROR_PROPERTY_NOT_STRING;
extern String^ ERROR_COMPILATION_FAILED;
extern String^ ERROR_SCRIPT_ATTEMPTED_EVAL;
extern String^ ERROR_ALREADY_DEBUGGING;

inline void ProcessRuntimeError(JsErrorCode error)
{
    assert(error != JsNoError);

    switch (error)
    {
    case JsErrorInvalidArgument:
    case JsErrorNullArgument:
        throw ref new InvalidArgumentException();

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
    case JsErrorScriptException:
    case JsErrorScriptTerminated:
    case JsErrorInExceptionState:
        // unexpected to occur normally
        assert(false);
        throw ref new Exception(E_FAIL);

    case JsCannotSetProjectionEnqueueCallback:
    case JsErrorInObjectBeforeCollectCallback:
    default:
        throw ref new Exception(E_FAIL);
    }
}
