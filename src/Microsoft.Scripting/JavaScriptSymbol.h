#pragma once
#include "pch.h"
#include "Common.h"

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
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
                property String^ Description
                {
                    String^ get();
                }
                property int32 Length
                {
                    int32 get();
                }

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS()
            };
        }
    }
}
