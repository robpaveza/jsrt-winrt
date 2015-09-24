#pragma once
using namespace Windows::Foundation;

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            /// <summary>
            /// Represents a JavaScript <c>Array</c> object.
            /// </summary>
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
                /// <summary>
                /// Gets the length of the Array.
                /// </summary>
                property int32 Length
                {
                    int32 get();
                }
                /// <summary>
                /// Gets the value at the specified index.
                /// </summary>
                IJavaScriptValue^ GetAt(int32 index);
                /// <summary>
                /// Sets the value at the specified index.
                /// </summary>
                void SetAt(int32 index, IJavaScriptValue^ value);

                /// <summary>
                /// Removes the item at the end of the array and returns its value.
                /// </summary>
                IJavaScriptValue^ Pop();
                /// <summary>
                /// Adds a new item to the end of the array.
                /// </summary>
                void Push(IJavaScriptValue^ value);
                /// <summary>
                /// Reverses the array in-place.
                /// </summary>
                void Reverse();
                /// <summary>
                /// Removes the first element from the array and returns its value.  If the array is empty, returns undefined.
                /// </summary>
                IJavaScriptValue^ Shift();
                /// <summary>
                /// Inserts values at the beginning of the array.
                /// </summary>
                int32 Unshift(IIterable<IJavaScriptValue^>^ valuesToInsert);
                /// <summary>
                /// Sorts the array.
                /// </summary>
                /// <param name="compareFunction">A function to determine ordering of elements - <c>function(a, b)</c> which returns a number.  If this parameter is null, the default ordering is used.</param>
                void Sort(JavaScriptFunction^ compareFunction);
                /// <summary>
                /// Removes and optionally inserts items.
                /// </summary>
                JavaScriptArray^ Splice(uint32 index, uint32 numberToRemove, IIterable<IJavaScriptValue^>^ objectsToInsert);

                /// <summary>
                /// Creates a new array composed of the elements of the current array and the items or contents of arrays passed in as parameters.
                /// </summary>
                JavaScriptArray^ Concat(IIterable<IJavaScriptValue^>^ itemsToConcatenate);
                /// <summary>
                /// Stringifies each item in the current array and creates a complete string containing the items and separators.
                /// </summary>
                String^ Join(String^ separator);
                /// <summary>
                /// Returns a shallow copy of a portion of an array into a new array object.
                /// </summary>
                JavaScriptArray^ Slice(int32 beginning);
                /// <summary>
                /// Returns a shallow copy of a portion of an array into a new array object.
                /// </summary>
                JavaScriptArray^ Slice(int32 beginning, int32 end);
                /// <summary>
                /// Looks for a value in the array; if found, returns the index, otherwise returns -1.
                /// </summary>
                int32 IndexOf(IJavaScriptValue^ valueToFind);
                /// <summary>
                /// Looks for a value in the array starting at the specified index; if found, returns the index, otherwise returns -1.
                /// </summary>
                int32 IndexOf(IJavaScriptValue^ valueToFind, int32 startIndex);
                /// <summary>
                /// Looks in reverse for a value in the array starting at the end; if found, returns the index, otherwise returns -1.
                /// </summary>
                int32 LastIndexOf(IJavaScriptValue^ valueToFind);
                /// <summary>
                /// Looks in reverse for a value in the array starting at the specified index; if found, returns the index, otherwise returns -1.
                /// </summary>
                int32 LastIndexOf(IJavaScriptValue^ valueToFind, int32 lastIndex);

                /// <summary>
                /// Executes a callback function for each item in the array.
                /// </summary>
                void ForEach(JavaScriptFunction^ callee);
                /// <summary>
                /// Returns true if invoking the predicate function on every element in the array returns true; otherwise returns false.
                /// </summary>
                bool Every(JavaScriptFunction^ predicate);
                /// <summary>
                /// Returns true if invoking the predicate function on any element in the array returns true; otherwise returns false.
                /// </summary>
                bool Some(JavaScriptFunction^ predicate);
                /// <summary>
                /// Returns a new array containing the elements which caused the predicate callback to return true.
                /// </summary>
                JavaScriptArray^ Filter(JavaScriptFunction^ predicate);
                /// <summary>
                /// Returns a new array containing the projection of each value resulted from passing the value of each item in the array to the converter function.
                /// </summary>
                JavaScriptArray^ Map(JavaScriptFunction^ converter);
                /// <summary>
                /// Applies a function against an accumulator with each value in the array from start to end, reducing it to a single scalar value.
                /// </summary>
                IJavaScriptValue^ Reduce(JavaScriptFunction^ aggregator);
                /// <summary>
                /// Applies a function against an accumulator with each value in the array from start to end, reducing it to a single scalar value.
                /// </summary>
                IJavaScriptValue^ Reduce(JavaScriptFunction^ aggregator, IJavaScriptValue^ initialValue);
                /// <summary>
                /// Applies a function against an accumulator with each value in the array from end to start, reducing it to a single scalar value.
                /// </summary>
                IJavaScriptValue^ ReduceRight(JavaScriptFunction^ aggregator);
                /// <summary>
                /// Applies a function against an accumulator with each value in the array from end to start, reducing it to a single scalar value.
                /// </summary>
                IJavaScriptValue^ ReduceRight(JavaScriptFunction^ aggregator, IJavaScriptValue^ initialValue);

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS(JavaScriptArray)
            };
        };
    };
};
