#pragma once
#include "Common.h"
#include <set>

#define ENSURE_VALID_ENGINE() \
    if (context_ == JS_INVALID_REFERENCE) \
        throw ref new ObjectDisposedException(EngineDisposed)

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
#pragma region Thunks
            ref class ExternalObjectFinalizeCallbackThunkData sealed
            {
            private:
                JavaScriptExternalObjectFinalizeCallback^ callback_;
                JavaScriptEngine^ engine_;
                Object^ data_;

            public:
                ExternalObjectFinalizeCallbackThunkData(JavaScriptExternalObjectFinalizeCallback^ cb, JavaScriptEngine^ eng, Object^ obj);
                property JavaScriptExternalObjectFinalizeCallback^ callback
                {
                    JavaScriptExternalObjectFinalizeCallback^ get() { return callback_; }
                }
                property JavaScriptEngine^ engine
                {
                    JavaScriptEngine^ get() { return engine_; }
                }
                property Object^ data
                {
                    Object^ get() { return data_; }
                }
            };

            ref class NativeFunctionThunkData sealed
            {
            private:
                JavaScriptCallableFunction^ callback_;
                JavaScriptEngine^ engine_;

            public:
                NativeFunctionThunkData(JavaScriptCallableFunction^ cb, JavaScriptEngine^ eng);

                property JavaScriptCallableFunction^ Target
                {
                    JavaScriptCallableFunction^ get() { return callback_; }
                }
                property JavaScriptEngine^ Engine 
                {
                    JavaScriptEngine^ get() { return engine_; }
                }
            };
#pragma endregion

            [MD::WebHostHidden]
            public ref class JavaScriptEngine sealed
            {
            private:
                JsContextRef context_;
                JavaScriptRuntime^ runtime_;
                JavaScriptConverter^ converter_;
                JavaScriptEngineSymbolRegistry^ symbols_;
                
                std::once_flag gotGlobal_, gotUndefined_, gotNull_, gotTrue_, gotFalse_;
                JavaScriptObject ^global_, ^null_;
                IJavaScriptValue ^undefined_, ^true_, ^false_;
                std::set<IUnknown*> finalizers_, nativeFuncs_;
                std::vector<JsValueRef> releasedItems_;
                std::mutex releaseHold_;

                static void CALLBACK ExternalObjectFinalizeCallbackThunk(void* data);
                static JsValueRef CALLBACK NativeCallbackThunk(JsValueRef callee, bool asConstructor, JsValueRef* args, unsigned short argCount, void* data);
                void RemoveThunk(ExternalObjectFinalizeCallbackThunkData^ data);

            internal:
                JavaScriptFunction ^Object_ctor_, ^Object_keys_, ^Object_getOwnPropertyNames_, ^Object_seal_,
                    ^Object_isSealed_, ^Object_freeze_, ^Object_isFrozen_, ^Object_defineProperties_;
                IJavaScriptObject ^Array_prototype_;
                JavaScriptEngine(JsContextRef context, JavaScriptRuntime^ runtime);
                void ClaimContext();
                IJavaScriptValue^ CreateValueFromHandle(JsValueRef handle);
                JavaScriptArray^ CreateArrayFromHandle(JsValueRef handle);
                JavaScriptFunction^ CreateFunctionFromHandle(JsValueRef handle);
                IJavaScriptObject^ CreateObjectFromHandle(JsValueRef handle);
                JavaScriptObject^ CreateFirmObjectFromHandle(JsValueRef handle);
                JavaScriptSymbol^ CreateSymbolFromHandle(JsValueRef handle);
                void QueueRelease(JsValueRef handle);
                void ProcessError(JsErrorCode error);

            public:
                virtual ~JavaScriptEngine();

                event EventHandler<Object^>^ RuntimeExceptionRaised;

                // properties
                property JavaScriptRuntime^ Runtime
                {
                    JavaScriptRuntime^ get() { return runtime_; }
                }
                property JavaScriptConverter^ Converter
                {
                    JavaScriptConverter^ get() { return converter_; }
                }
                property JavaScriptObject^ GlobalObject
                {
                    JavaScriptObject^ get();
                }
                property IJavaScriptValue^ UndefinedValue
                {
                    IJavaScriptValue^ get();
                }
                property JavaScriptObject^ NullValue
                {
                    JavaScriptObject^ get();
                }
                property IJavaScriptValue^ TrueValue
                {
                    IJavaScriptValue^ get();
                }
                property IJavaScriptValue^ FalseValue
                {
                    IJavaScriptValue^ get();
                }
                property bool HasException
                {
                    bool get();
                }
                property JavaScriptEngineSymbolRegistry^ Symbol
                {
                    JavaScriptEngineSymbolRegistry^ get();
                }

                // serialization methods
                JavaScriptFunction^ EvaluateScriptText(String^ code);
                JavaScriptFunction^ Evaluate(ScriptSource^ source);
                JavaScriptFunction^ Evaluate(ScriptSource^ source, const Array<uint8>^ compiledCode);
                Array<uint8>^ Compile(ScriptSource^ source);
                IJavaScriptValue^ Execute(ScriptSource^ source);
                IJavaScriptValue^ Execute(ScriptSource^ source, const Array<uint8>^ compiledCode);

                // API methods
                JavaScriptObject^ CreateObject(JavaScriptObject^ prototype);
                JavaScriptObject^ CreateExternalObject(Object^ externalData, JavaScriptExternalObjectFinalizeCallback^ finalizeCallback);
                JavaScriptSymbol^ CreateSymbol(String^ description);
                DateTime RunIdleWork();

                bool HasGlobalVariable(String^ name);
                IJavaScriptValue^ GetGlobalVariable(String^ name);
                void SetGlobalVariable(String^ name, IJavaScriptValue^ value);
                IJavaScriptValue^ CallGlobalFunction(String^ functionName, IIterable<IJavaScriptValue^>^ arguments);
                void SetGlobalFunction(String^ functionName, JavaScriptCallableFunction^ nativeFunction);
                JavaScriptArray^ CreateArray(int32 length);
                JavaScriptFunction^ CreateFunction(JavaScriptCallableFunction^ nativeFunction);
                IJavaScriptValue^ GetAndClearException();
                void SetException(IJavaScriptValue^ exception);

                JavaScriptObject^ CreateError(String^ message);
                JavaScriptObject^ CreateRangeError(String^ message);
                JavaScriptObject^ CreateReferenceError(String^ message);
                JavaScriptObject^ CreateSyntaxError(String^ message);
                JavaScriptObject^ CreateTypeError(String^ message);
                JavaScriptObject^ CreateUriError(String^ message);
            };
        };
    };
};