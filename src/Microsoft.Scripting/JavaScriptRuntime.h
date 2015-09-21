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
            [MD::WebHostHidden]
            public ref class JavaScriptRuntime sealed
            {
            private:
                JavaScriptRuntimeSettings^ settings_;
                JsRuntimeHandle runtime_;

            public:
                JavaScriptRuntime(JavaScriptRuntimeSettings^ settings);
                virtual ~JavaScriptRuntime();
                void CollectGarbage();
                JavaScriptEngine^ CreateEngine();
                void EnableExecution();
                void DisableExecution();

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
            };
        };
    };
};

