#pragma once
#include "Common.h"
#include "JavaScriptRuntimeSettings.h"

#define ENSURE_VALID_RUNTIME() \
    if (runtime_ == JS_INVALID_RUNTIME_HANDLE) \
        throw ref new ObjectDisposedException()

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            /// <summary>
            /// Provides the runtime and used for running JavaScript code.
            /// </summary>
            [MD::WebHostHidden]
            public ref class JavaScriptRuntime sealed
            {
            private:
                JavaScriptRuntimeSettings^ settings_;
                JsRuntimeHandle runtime_;

            public:
                /// <summary>
                /// Creates a new <c>JavaScriptRuntime</c> with the given settings.
                /// </summary>
                /// <param name="settings">Settings which dictate how the runtime should behave.</param>
                JavaScriptRuntime(JavaScriptRuntimeSettings^ settings);
                /// <summary>
                /// Releases native resources owned by the runtime.
                /// </summary>
                virtual ~JavaScriptRuntime();

                /// <summary>
                /// Causes the runtime to schedule a garbage collection.
                /// </summary>
                void CollectGarbage();

                /// <summary>
                /// Creates a new <c>JavaScriptEngine</c> within the current runtime.
                /// </summary>
                JavaScriptEngine^ CreateEngine();

                /// <summary>
                /// Allows the runtime to re-enable execution after it has been disabled.
                /// </summary>
                void EnableExecution();
                /// <summary>
                /// Disables execution or usage of any part of the runtime.  Only functions if the engine was created with the 
                /// <c>AllowScriptInterrupt</c> property was enabled.
                /// </summary>
                void DisableExecution();

                /// <summary>
                /// Gets the amount of memory used by the runtime.
                /// </summary>
                property int64 RuntimeMemoryUsage 
                {
                    int64 get()
                    {
                        ENSURE_VALID_RUNTIME();

                        size_t usage;
                        auto err = JsGetRuntimeMemoryUsage(runtime_, &usage);
                        if (err != JsNoError)
                            throw ref new Exception(E_FAIL);

                        return (int64)usage;
                    }
                }

                /// <summary>
                /// Gets whether execution is presently enabled on the runtime.
                /// </summary>
                property bool IsExecutionEnabled
                {
                    bool get()
                    {
                        ENSURE_VALID_RUNTIME();

                        bool result;
                        auto err = JsIsRuntimeExecutionDisabled(runtime_, &result);
                        if (err != JsNoError)
                            throw ref new Exception(E_FAIL);

                        return result;
                    }
                }

                /// <summary>
                /// Gets the runtime handle (<c>JsRuntimeHandle</c>) contained by this runtime.
                /// </summary>
                /// <remarks>
                /// You should not use this property unless you absolutely know what you are doing.  Doing so may cause memory corruption.
                /// </remarks>
                property IntPtr Handle
                {
                    IntPtr get()
                    {
                        return IntPtr(runtime_);
                    }
                }
            };
        };
    };
};

