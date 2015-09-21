#pragma once
using namespace Windows::Foundation;

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            [MD::WebHostHidden]
            public ref class JavaScriptArray sealed :
                public IJavaScriptObject
            {
            private:
                JsValueRef handle_;
                JavaScriptPrimitiveValue^ primitive_;
                JavaScriptObject^ object_;
                JavaScriptEngine^ engine_;

            internal:
                JavaScriptArray(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object);

            public:
                property int32 Length
                {
                    int32 get();
                }
                IJavaScriptValue^ GetAt(int32 index);
                void SetAt(int32 index, IJavaScriptValue^ value);

                IJavaScriptValue^ Pop();
                void Push(IJavaScriptValue^ value);
                void Reverse();
                IJavaScriptValue^ Shift();
                int32 Unshift(IIterable<IJavaScriptValue^>^ valuesToInsert);
                void Sort(JavaScriptFunction^ compareFunction);
                JavaScriptArray^ Splice(uint32 index, uint32 numberToRemove, IIterable<IJavaScriptValue^>^ objectsToInsert);

                JavaScriptArray^ Concat(IIterable<IJavaScriptValue^>^ itemsToConcatenate);
                String^ Join(String^ separator);
                JavaScriptArray^ Slice(int32 beginning);
                JavaScriptArray^ Slice(int32 beginning, int32 end);
                int32 IndexOf(IJavaScriptValue^ valueToFind);
                int32 IndexOf(IJavaScriptValue^ valueToFind, int32 startIndex);
                int32 LastIndexOf(IJavaScriptValue^ valueToFind);
                int32 LastIndexOf(IJavaScriptValue^ valueToFind, int32 lastIndex);

                void ForEach(JavaScriptFunction^ callee);
                bool Every(JavaScriptFunction^ predicate);
                bool Some(JavaScriptFunction^ predicate);
                JavaScriptArray^ Filter(JavaScriptFunction^ predicate);
                JavaScriptArray^ Map(JavaScriptFunction^ converter);
                IJavaScriptValue^ Reduce(JavaScriptFunction^ aggregator);
                IJavaScriptValue^ Reduce(JavaScriptFunction^ aggregator, IJavaScriptValue^ initialValue);
                IJavaScriptValue^ ReduceRight(JavaScriptFunction^ aggregator);
                IJavaScriptValue^ ReduceRight(JavaScriptFunction^ aggregator, IJavaScriptValue^ initialValue);

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS()
            };
        };
    };
};
