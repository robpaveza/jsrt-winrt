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
                public IJavaScriptObject
            {
            private:
                JsValueRef handle_;
                JavaScriptEngine^ engine_;
                JavaScriptPrimitiveValue^ primitive_;
                JavaScriptObject^ object_;

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
                /// <summary>
                /// Gets the Symbol's <c>length</c> property.
                /// </summary>
                property int32 Length
                {
                    int32 get();
                }

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS()
            };
        }
    }
}
