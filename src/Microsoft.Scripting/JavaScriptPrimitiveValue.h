#pragma once
#include "Common.h"

using namespace Platform;
namespace WFM = Windows::Foundation::Metadata;
using namespace Windows::Foundation;

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            [MD::WebHostHidden]
            ref class JavaScriptPrimitiveValue :
                public IJavaScriptValue
            {
            internal:
                JsValueRef handle_;
                JavaScriptEngine^ engine_;
                JavaScriptPrimitiveValue(JsValueRef jsRef, JavaScriptEngine^ engine);

            public:
                virtual ~JavaScriptPrimitiveValue();
                virtual String^ ToString();
                property JavaScriptValueType Type
                {
                    virtual JavaScriptValueType get();
                }

                property bool IsTruthy
                {
                    virtual bool get();
                }

                property JavaScriptEngine^ Engine
                {
                    virtual JavaScriptEngine^ get();
                }
                virtual bool SimpleEquals(IJavaScriptValue^ other);
                virtual bool StrictEquals(IJavaScriptValue^ other);

                property IntPtr Handle
                {
                    virtual IntPtr get() { return handle_; }
                }
            };
        };
    };
};
