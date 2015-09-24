#pragma once
#include "pch.h"
#include "Common.h"

using namespace Windows::Foundation;

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            /// <summary>
            /// Represents a JavaScript typed array.
            /// </summary>
            [MD::WebHostHidden]
            public ref class JavaScriptTypedArray sealed :
                IJavaScriptObject
            {
            private:
                JsValueRef handle_;
                JavaScriptPrimitiveValue^ primitive_;
                JavaScriptObject^ object_;
                JavaScriptEngine^ engine_;
                JavaScriptTypedArrayType type_;
                std::once_flag gotType_;

            internal:
                JavaScriptTypedArray(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object);

            public:
                /// <summary>
                /// Gets a reference to the underlying buffer.
                /// </summary>
                property JavaScriptArrayBuffer^ Buffer
                {
                    JavaScriptArrayBuffer^ get();
                }
                /// <summary>
                /// Gets the byte-length of the typed array's memory space.
                /// </summary>
                property uint32 ByteLength
                {
                    uint32 get();
                }
                /// <summary>
                /// Gets the byte offset of the typed array from the beginning of the underlying buffer.
                /// </summary>
                property uint32 ByteOffset
                {
                    uint32 get();
                }
                /// <summary>
                /// Gets the element length of the typed array.
                /// </summary>
                property uint32 Length
                {
                    uint32 get();
                }
                /// <summary>
                /// Gets the type of the typed array (for example, <c>Uint8</c>).
                /// </summary>
                property JavaScriptTypedArrayType ArrayType
                {
                    JavaScriptTypedArrayType get();
                }

                // Todo: Come back to these
                /*
                static JavaScriptTypedArray^ From(JavaScriptTypedArrayType arrayType, IJavaScriptObject^ arrayLikeOrIterableObject);
                static JavaScriptTypedArray^ Of(JavaScriptTypedArrayType arrayType, IIterable<IJavaScriptValue^>^ elements);
                
                void CopyWithin(uint32 targetStartIndex, uint32 sourceStartIndex);
                void CopyWithin(uint32 targetStartIndex, uint32 sourceStartIndex, uint32 sourceEndIndex);
                IJavaScriptObject^ Entries();
                bool Every(JavaScriptFunction^ predicate);
                void Fill(IJavaScriptValue^ value);
                void Fill(IJavaScriptValue^ value, uint32 start);
                void Fill(IJavaScriptValue^ value, uint32 start, uint32 end);
                JavaScriptArray^ Filter(JavaScriptFunction^ predicate);
                IJavaScriptValue^ Find(JavaScriptFunction^ predicate);
                IJavaScriptValue^ FindIndex(JavaScriptFunction^ predicate);
                void ForEach(JavaScriptFunction^ callback);
                bool Includes(IJavaScriptValue^ searchElement);
                bool Includes(IJavaScriptValue^ searchElement, uint32 fromIndex);
                int32 IndexOf(IJavaScriptValue^ searchElement);
                int32 IndexOf(IJavaScriptValue^ searchElement, uint32 fromIndex);
                String^ Join();
                String^ Join(String^ separator);
                JavaScriptArray^ Keys();
                */

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS(JavaScriptTypedArray)
            };
        }
    }
}
