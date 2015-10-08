#pragma once
#include "Common.h"

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            /// <summary>
            /// Converts between JavaScript value references and host-provided primitive types.
            /// </summary>
            [MD::WebHostHidden]
            public ref class JavaScriptConverter sealed
            {
            private:
                JavaScriptEngine^ engine_;
            internal:
                JavaScriptConverter(JavaScriptEngine^ engine);

            public:
                /// <summary>
                /// Converts a value to its Boolean value.  If the value is not a Boolean value, it will be coerced.
                /// </summary>
                bool ToBoolean(IJavaScriptValue^ value);
                /// <summary>
                /// Creates a JavaScript Boolean value from the native Boolean.
                /// </summary>
                IJavaScriptValue^ FromBoolean(bool value);
                /// <summary>
                /// Converts a number to a double-precision floating point number.  If the value is not a number value, it will be coerced.
                /// </summary>
                double ToDouble(IJavaScriptValue^ value);
                /// <summary>
                /// Converts a double-precision floating point number to a JavaScript number.
                /// </summary>
                IJavaScriptValue^ FromDouble(double value);
                /// <summary>
                /// Converts a number to a 32-bit integer.  If the value is not a number value, it will be coerced.
                /// </summary>
                int32 ToInt32(IJavaScriptValue^ value);
                /// <summary>
                /// Converts an integer to a JavaScript number.
                /// </summary>
                IJavaScriptValue^ FromInt32(int32 value);
                /// <summary>
                /// Converts a JavaScript value to a String.  If the value is not a String value, it will be coerced.
                /// </summary>
                String^ ToString(IJavaScriptValue^ value);
                /// <summary>
                /// Converts a String to a JavaScript string.
                /// </summary>
                IJavaScriptValue^ FromString(String^ value);
            };
        };
    };
};