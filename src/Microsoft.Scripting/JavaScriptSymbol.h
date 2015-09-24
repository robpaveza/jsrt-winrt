#pragma once
#include "pch.h"
#include "Common.h"

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            /// <summary>
            /// Represents a JavaScript <c>Symbol</c> value.
            /// </summary>
            [MD::WebHostHidden]
            public ref class JavaScriptSymbol sealed :
                public IJavaScriptValue
            {
            private:
                JsValueRef handle_;
                JavaScriptEngine^ engine_;
                JavaScriptPrimitiveValue^ primitive_;

            internal:
                JavaScriptSymbol(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object);

            public:
                /// <summary>
                /// Gets the Symbol's description.
                /// </summary>
                property String^ Description
                {
                    String^ get();
                }

                DECLARE_JAVASCRIPT_PRIMITIVE_VALUE_VIRTUAL_MEMBERS(JavaScriptSymbol)
            };
        }
    }
}
