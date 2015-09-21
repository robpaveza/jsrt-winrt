#include "pch.h"
#include "JavaScriptRuntimeSettings.h"

using namespace Microsoft::Scripting::JavaScript;

#ifndef USE_EDGEMODE_JSRT
JavaScriptRuntimeSettings::JavaScriptRuntimeSettings(JavaScriptRuntimeVersion version) :
    version_(version),
    backgroundWork_(false),
    allowScriptInterrupt_(false),
    enableIdle_(false),
    disableNativeCode_(false),
    disableEval_(false),
    used_(false)
{

}
#else
JavaScriptRuntimeSettings::JavaScriptRuntimeSettings():
    backgroundWork_(false),
    allowScriptInterrupt_(false),
    enableIdle_(false),
    disableNativeCode_(false),
    disableEval_(false),
    used_(false)
{

}
#endif // !USE_EDGEMODE_JSRT

JsRuntimeAttributes JavaScriptRuntimeSettings::GetRuntimeAttributes()
{
    uint32 result = JsRuntimeAttributeNone;
    if (backgroundWork_)
        result |= JsRuntimeAttributeDisableBackgroundWork;
    if (allowScriptInterrupt_)
        result |= JsRuntimeAttributeAllowScriptInterrupt;
    if (enableIdle_)
        result |= JsRuntimeAttributeEnableIdleProcessing;
    if (disableNativeCode_)
        result |= JsRuntimeAttributeDisableNativeCodeGeneration;
    if (disableEval_)
        result |= JsRuntimeAttributeDisableEval;

    return (JsRuntimeAttributes)result;
}

void JavaScriptRuntimeSettings::SetUsed()
{
    used_ = true;
}