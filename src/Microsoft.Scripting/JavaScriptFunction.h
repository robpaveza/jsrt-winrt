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
            /// <summary>
            /// Represents a JavaScript function object.
            /// </summary>
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
                /// <summary>
                /// Invokes a function with arguments.  The first argument is the <c>this</c> object passed into the function's execution context.
                /// </summary>
                IJavaScriptValue^ Invoke(IIterable<IJavaScriptValue^>^ arguments);
                /// <summary>
                /// Invokes a function with arguments as if it was called with <c>new</c>.
                /// </summary>
                IJavaScriptObject^ Construct(IIterable<IJavaScriptValue^>^ arguments);
                
                /// <summary>
                /// Creates a native bound function, such as by calling <c>Function.bind()</c>.
                /// </summary>
                JavaScriptFunction^ Bind(IJavaScriptObject^ thisObject, IIterable<IJavaScriptValue^>^ arguments);
                /// <summary>
                /// Calls a function, spreading the arguments from an array to the function's individual arguments (such as by calling <c>Function.apply()</c>).
                /// </summary>
                IJavaScriptValue^ Apply(IJavaScriptObject^ thisObject, JavaScriptArray^ arguments);
                /// <summary>
                /// Calls a function, specifying a this object and arguments.
                /// </summary>
                IJavaScriptValue^ Call(IJavaScriptObject^ thisObject, IIterable<IJavaScriptValue^>^ arguments);

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS(JavaScriptFunction)

            };
        };
    };
};