#pragma once
#include "Common.h"

using namespace Windows::Foundation;
using namespace Platform;

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            [MD::WebHostHidden]
            public ref class JavaScriptFunction sealed :
                public IJavaScriptObject
            {
            private:
                JsValueRef handle_;
                JavaScriptPrimitiveValue^ primitive_;
                JavaScriptObject^ object_;
                JavaScriptEngine^ engine_;

            internal:
                JavaScriptFunction(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object);

            public:
                IJavaScriptValue^ Invoke(IIterable<IJavaScriptValue^>^ arguments);
                IJavaScriptObject^ Construct(IIterable<IJavaScriptValue^>^ arguments);
                
                JavaScriptFunction^ Bind(IJavaScriptObject^ thisObject, IIterable<IJavaScriptValue^>^ arguments);
                IJavaScriptValue^ Apply(IJavaScriptObject^ thisObject, JavaScriptArray^ arguments);
                IJavaScriptValue^ Call(IJavaScriptObject^ thisObject, IIterable<IJavaScriptValue^>^ arguments);

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS()

            };
        };
    };
};