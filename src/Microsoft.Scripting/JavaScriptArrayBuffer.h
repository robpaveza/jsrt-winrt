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
                Windows::Storage::Streams::IBuffer^ GetBackingMemory();
                property uint32 ByteLength
                {
                    uint32 get();
                }

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS()
            };
        }
    }
}