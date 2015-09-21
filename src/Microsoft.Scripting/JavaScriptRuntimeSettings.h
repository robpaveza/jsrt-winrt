#pragma once
#include "Common.h"

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
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
                JavaScriptRuntimeSettings();
#endif // !USE_EDGEMODE_JSRT

                property bool DisableBackgroundWork
                {
                    bool get() { return backgroundWork_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        backgroundWork_ = value;
                    }
                }

                property bool AllowScriptInterrupt
                {
                    bool get() { return allowScriptInterrupt_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        allowScriptInterrupt_ = value;
                    }
                }

                property bool EnableIdleProcessing 
                {
                    bool get() { return enableIdle_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        enableIdle_ = value;
                    }
                }

                property bool DisableNativeCodeGeneration
                {
                    bool get() { return disableNativeCode_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        disableNativeCode_ = value;
                    }
                }

                property bool DisableEval
                {
                    bool get() { return disableEval_; }
                    void set(bool value) {
                        if (used_) throw ref new Platform::FailureException(NoMutateJsRuntimeSettings);
                        disableEval_ = value;
                    }
                }

#ifndef USE_EDGEMODE_JSRT
                property JavaScriptRuntimeVersion Version
                {
                    JavaScriptRuntimeVersion get() { return version_; }
                }
#endif
            };
        };
    };
};