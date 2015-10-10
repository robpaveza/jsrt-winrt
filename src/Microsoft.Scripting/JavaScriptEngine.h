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
            /// <summary>
            /// Represents an isolated JavaScript execution context.
            /// </summary>
            [MD::WebHostHidden]
            public ref class JavaScriptEngine sealed
            {
            private:
                JsContextRef context_;
                JavaScriptRuntime^ runtime_;
                JavaScriptConverter^ converter_;
#ifdef USE_EDGEMODE_JSRT
                JavaScriptEngineSymbolRegistry^ symbols_;
#endif // USE_EDGEMODE_JSRT
                
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
                /// <summary>
                /// Closes the engine and frees the related resources.
                /// </summary>
                virtual ~JavaScriptEngine();

                /// <summary>
                /// When fired, indicates that the script engine has encountered a runtime error.  In order to get the exception, call <reference name="GetAndClearException" />.
                /// </summary>
                event EventHandler<Object^>^ RuntimeExceptionRaised;

                // properties
                /// <summary>
                /// Gets a reference to the runtime to which this engine belongs.
                /// </summary>
                property JavaScriptRuntime^ Runtime
                {
                    JavaScriptRuntime^ get() { return runtime_; }
                }
                /// <summary>
                /// Gets the engine's converter to perform value-to-host-value conversions.
                /// </summary>
                property JavaScriptConverter^ Converter
                {
                    JavaScriptConverter^ get() { return converter_; }
                }
                /// <summary>
                /// Gets the global object for the engine.
                /// </summary>
                property JavaScriptObject^ GlobalObject
                {
                    JavaScriptObject^ get();
                }
                /// <summary>
                /// Gets the <c>undefined</c> value for the engine.
                /// </summary>
                property IJavaScriptValue^ UndefinedValue
                {
                    IJavaScriptValue^ get();
                }
                /// <summary>
                /// Gets the <c>null</c> value for the engine.
                /// </summary>
                property JavaScriptObject^ NullValue
                {
                    JavaScriptObject^ get();
                }
                /// <summary>
                /// Gets the <c>true</c> value for the engine.
                /// </summary>
                property IJavaScriptValue^ TrueValue
                {
                    IJavaScriptValue^ get();
                }
                /// <summary>
                /// Gets the <c>false</c> value for the engine.
                /// </summary>
                property IJavaScriptValue^ FalseValue
                {
                    IJavaScriptValue^ get();
                }
                /// <summary>
                /// Gets whether the engine is currently in an exception state.  If so, many operations cannot be completed until <reference name="GetAndClearException" /> is called.
                /// </summary>
                property bool HasException
                {
                    bool get();
                }
#ifdef USE_EDGEMODE_JSRT
                /// <summary>
                /// Gets a reference to the engine's <c>Symbol</c> constructor and global Symbol registry.
                /// </summary>
                property JavaScriptEngineSymbolRegistry^ Symbol
                {
                    JavaScriptEngineSymbolRegistry^ get();
                }
#endif // USE_EDGEMODE_JSRT
                /// <summary>
                /// Gets the context handle (<c>JsContextRef</c>) contained by this engine.
                /// </summary>
                /// <remarks>
                /// You should not use this property unless you absolutely know what you are doing.  Doing so may cause memory corruption.
                /// </remarks>
                property IntPtr Handle
                {
                    IntPtr get();
                }

                // serialization methods
                /// <summary>
                /// Parses script and returns a function representing the code.
                /// </summary>
                JavaScriptFunction^ EvaluateScriptText(String^ code);
                /// <summary>
                /// Parses a script source and returns a function representing the code.
                /// </summary>
                JavaScriptFunction^ Evaluate(ScriptSource^ source);
                /// <summary>
                /// Parses a script source with serialized code and returns a function representing the code.
                /// </summary>
                JavaScriptFunction^ Evaluate(ScriptSource^ source, const Array<uint8>^ compiledCode);
                /// <summary>
                /// Compiles a script source into serialized code.
                /// </summary>
                Array<uint8>^ Compile(ScriptSource^ source);
                /// <summary>
                /// Executes a script source immediately and returns the valuation of the last expression in the script.
                /// </summary>
                IJavaScriptValue^ Execute(ScriptSource^ source);
                /// <summary>
                /// Executes a compiled script source immediately and returns the valuation of the last expression in the script.
                /// </summary>
                IJavaScriptValue^ Execute(ScriptSource^ source, const Array<uint8>^ compiledCode);

                // API methods
                /// <summary>
                /// Creates a JavaScript Object.
                /// </summary>
                /// <param name="prototype">The object's prototype.  If <c>null</c>, the prototype will be unassigned (such as by calling <c>Object.create(null)</c>).</param>
                JavaScriptObject^ CreateObject(JavaScriptObject^ prototype);
                /// <summary>
                /// Creates an JavaScript Object that can be associated with a host object.
                /// </summary>
                JavaScriptObject^ CreateExternalObject(Object^ externalData, JavaScriptExternalObjectFinalizeCallback^ finalizeCallback);
                /// <summary>
                /// Creates a JavaScript Symbol value with an optional description.
                /// </summary>
                JavaScriptSymbol^ CreateSymbol(String^ description);
                /// <summary>
                /// Tells the runtime to do any idle processing it need to do. 
                /// </summary>
                /// <remarks><para>If idle processing has been enabled for the current runtime, calling <c>RunIdleWork</c> will inform the current runtime that the 
                ///  host is idle and that the runtime can perform memory cleanup tasks.</para>
                /// <para>RunIdleWork can also return the amount of time until there will be more idle work for the runtime to do.  Calling RunIdleWork before this 
                ///  number of ticks has passed will do no work.</para>
                /// </remarks>
                TimeSpan RunIdleWork();

                /// <summary>
                /// Gets whether the global object has a property with the specified name.
                /// </summary>
                bool HasGlobalVariable(String^ name);
                /// <summary>
                /// Gets the global variable with the specified name (returning <c>null</c> if it doesn't exist).
                /// </summary>
                IJavaScriptValue^ GetGlobalVariable(String^ name);
                /// <summary>
                /// Sets a global variable with the specified name.
                /// </summary>
                void SetGlobalVariable(String^ name, IJavaScriptValue^ value);
                /// <summary>
                /// Calls a global function with the specified name and arguments.
                /// </summary>
                /// <remarks>The function will be called with the global object as the <c>this</c> context.</remarks>
                IJavaScriptValue^ CallGlobalFunction(String^ functionName, IIterable<IJavaScriptValue^>^ arguments);
                /// <summary>
                /// Adds a host function as a function on the global object.
                /// </summary>
                void SetGlobalFunction(String^ functionName, JavaScriptCallableFunction^ nativeFunction);
                /// <summary>
                /// Creates a JavaScript Array object with the specified length.
                /// </summary>
                JavaScriptArray^ CreateArray(int32 length);
                /// <summary>
                /// Creates a JavaScript Function object that represents the specified native function.
                /// </summary>
                JavaScriptFunction^ CreateFunction(JavaScriptCallableFunction^ nativeFunction);
                /// <summary>
                /// Creates a JavaScript Function with the specified name that represents the specified native function.
                /// </summary>
                JavaScriptFunction^ CreateFunction(JavaScriptCallableFunction^ nativeFunction, String^ name);
                /// <summary>
                /// If the engine is in an exception state, retrieves the JavaScript value that was thrown and clears the exception state.
                /// </summary>
                IJavaScriptValue^ GetAndClearException();
                /// <summary>
                /// Sets the engine into an exception state.
                /// </summary>
                /// <remarks>One example of when you might do this is in a host function called from script, if the script passed in incorrect arguments.</remarks>
                void SetException(IJavaScriptValue^ exception);

                /// <summary>
                /// Creates an <c>Error</c> object.
                /// </summary>
                JavaScriptObject^ CreateError(String^ message);
                /// <summary>
                /// Creates a <c>RangeError</c> object.
                /// </summary>
                JavaScriptObject^ CreateRangeError(String^ message);
                /// <summary>
                /// Creates a <c>ReferenceError</c> object.
                /// </summary>
                JavaScriptObject^ CreateReferenceError(String^ message);
                /// <summary>
                /// Creates a <c>SyntaxError</c> object.
                /// </summary>
                JavaScriptObject^ CreateSyntaxError(String^ message);
                /// <summary>
                /// Creates a <c>TypeError</c> object.
                /// </summary>
                JavaScriptObject^ CreateTypeError(String^ message);
                /// <summary>
                /// Creates a <c>URIError</c> object.
                /// </summary>
                JavaScriptObject^ CreateUriError(String^ message);

#if defined( _WINRT_DLL )
                /// <summary>
                /// Enables Windows Runtime projection of a particular namespace to the JavaScript engine.
                /// </summary>
                void InitializeWindowsRuntimeNamespace(String^ namespaceName);
#endif // defined( _WINRT_DLL )

                /// <summary>
                /// Starts debugging in the current engine.
                /// </summary>
                /// <remarks>
                ///     <para>
                ///     The host should make sure that CoInitializeEx is called with COINIT_MULTITHREADED or COINIT_APARTMENTTHREADED at least once before using this API.
                ///     </para>
                ///     <para>
                ///     It is not possible to back out debugging on an app; as a result, you should only enable debugging within debug modes, or else performance 
                ///     may suffer.  To debug your application, launch or attach your app under Visual Studio, choosing Script debugging as the debug target (or 
                ///     Native with Script).  When debugging your script, you can't set breakpoints in your source files; you must do so only after the script file
                ///     has been enumerated in the engine, or optionally, you can use the <c>debugger</c> keyword to trigger a programmatic breakpoint.
                ///     </para>
                /// </remarks>
                void EnableDebugging();
            };
        };
    };
};