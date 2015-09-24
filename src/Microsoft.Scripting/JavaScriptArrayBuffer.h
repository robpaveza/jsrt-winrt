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
            /// Represents a JavaScript <c>ArrayBuffer</c>.
            /// </summary>
            [MD::WebHostHidden]
            public ref class JavaScriptArrayBuffer sealed :
                public IJavaScriptObject
            {
            private:
                JsValueRef handle_;
                JavaScriptEngine^ engine_;
                JavaScriptPrimitiveValue^ primitive_;
                JavaScriptObject^ object_;
                uint32 len_;
                std::once_flag gotLen_;

            internal:
                JavaScriptArrayBuffer(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object);

            public:
                /// <summary>
                /// Gets a reference to the underlying memory.
                /// </summary>
                /// <remarks>Calling this method does not prevent the ArrayBuffer from being 
                /// garbage-collected.  Consequently, if you release the reference to the ArrayBuffer
                /// and the engine doesn't have any references to it, the IBuffer will no longer
                /// point to valid memory.  Use with discretion.</remarks>
                Windows::Storage::Streams::IBuffer^ GetBackingMemory();
                /// <summary>
                /// Gets the byte length of the underlying memory store.
                /// </summary>
                property uint32 ByteLength
                {
                    uint32 get();
                }

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS(JavaScriptArrayBuffer)
            };
        }
    }
}