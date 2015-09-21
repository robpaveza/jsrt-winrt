#pragma once
#include "Common.h"

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            /// <summary>
            /// Contains several settings that must be used before creating a JavaScript Runtime.
            /// </summary>
            /// <remarks> Once a <c>JavaScriptRuntime</c> is created with a particular settings object, the settings object
            ///   may no longer be mutated. </remarks>
            [MD::WebHostHidden]
            public ref class JavaScriptRuntimeSettings sealed
            {
            private:
#ifndef USE_EDGEMODE_JSRT
                JavaScriptRuntimeVersion version_;
#endif // !USE_EDGEMODE_JSRT
                bool backgroundWork_;
                bool allowScriptInterrupt_;
                bool enableIdle_;
                bool disableNativeCode_;
                bool disableEval_;
                bool used_;

            internal:
                JsRuntimeAttributes GetRuntimeAttributes();
                void SetUsed();

            public:
#ifndef USE_EDGEMODE_JSRT
                JavaScriptRuntimeSettings(JavaScriptRuntimeVersion runtimeVersion);
#else 
                /// <summary>
                /// Creates a new instance of JavaScriptRuntimeSettings.
                /// </summary>
                JavaScriptRuntimeSettings();
#endif // !USE_EDGEMODE_JSRT

                /// <summary>
                /// Gets or sets whether background work will be disabled.
                /// </summary>
                /// <remarks>By default, this property is false, meaning that the runtime will manage background threads 
                ///   to perform garbage collection and memory management.  If set to true, all of these tasks will be 
                ///   performed on the foreground thread.</remarks>
                property bool DisableBackgroundWork
                {
                    bool get() { return backgroundWork_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        backgroundWork_ = value;
                    }
                }

                /// <summary>
                /// Gets or sets whether scripts will be interruptible.
                /// </summary>
                /// <remarks>By default, this property is false.  Setting it to true will enable the a <c>JavaScriptRuntime</c>
                ///    or <c>JavaScriptEngine</c> to pause or otherwise interrupt the execution of JavaScript code.  This may 
                ///    result in a slight performance degradation, but will also result in allowing the host to prevent code 
                ///    from running away in an infinite loop.</remarks>
                property bool AllowScriptInterrupt
                {
                    bool get() { return allowScriptInterrupt_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        allowScriptInterrupt_ = value;
                    }
                }

                /// <summary>
                /// TODO: Document
                /// </summary>
                property bool EnableIdleProcessing 
                {
                    bool get() { return enableIdle_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        enableIdle_ = value;
                    }
                }

                /// <summary>
                /// Gets or sets whether native code will be compiled.
                /// </summary>
                /// <remarks>By default, this property is false.  Setting it to true will cause any script engines to skip
                ///    JIT code generation, but will impose a heavy performance penalty.</remarks>
                property bool DisableNativeCodeGeneration
                {
                    bool get() { return disableNativeCode_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        disableNativeCode_ = value;
                    }
                }

                /// <summary>
                /// Gets or sets whether dynamic code execution will be disabled.
                /// </summary>
                /// <remarks>By default, this property is false.  Setting it to true will cause script calls to <c>eval</c>, the
                ///     <c>Function</c> constructor, or other means in which code can be synthesized and then executed to 
                ///     cause a runtime error to occur.</remarks>
                property bool DisableEval
                {
                    bool get() { return disableEval_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        disableEval_ = value;
                    }
                }

#ifndef USE_EDGEMODE_JSRT
                /// <summary>
                /// Gets or sets the version of the runtime which may be targeted.
                /// </summary>
                /// <remarks>This property is only available if compiling targeting jscript9.dll, which is not currently 
                ///     supported for Windows Store apps.  It is included presently only for completeness.</remarks>
                property JavaScriptRuntimeVersion Version
                {
                    JavaScriptRuntimeVersion get() { return version_; }
                }
#endif
            };
        };
    };
};