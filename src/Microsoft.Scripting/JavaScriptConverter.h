#pragma once
#include "Common.h"

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            [MD::WebHostHidden]
            public ref class JavaScriptConverter sealed
            {
            private:
                JavaScriptEngine^ engine_;
            internal:
                JavaScriptConverter(JavaScriptEngine^ engine);

            public:
                bool ToBoolean(IJavaScriptValue^ value);
                IJavaScriptValue^ FromBoolean(bool value);
                double ToDouble(IJavaScriptValue^ value);
                IJavaScriptValue^ FromDouble(double value);
                String^ ToString(IJavaScriptValue^ value);
                IJavaScriptValue^ FromString(String^ value);
            };
        };
    };
};