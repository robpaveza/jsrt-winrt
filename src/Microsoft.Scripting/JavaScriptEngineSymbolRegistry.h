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
            /// Contains the well-known Symbols that live on the Symbol global object.
            /// </summary>
            /// <remarks>
            /// Because the Symbol global object can't both be a JavaScriptFunction and an object containing properties
            /// like it really is in JavaScript, engine.Symbols contains an access to the Symbol constructor, as well as
            /// the other properties and functions that have access to the Symbol constructor's function.
            /// </remarks>
            [MD::WebHostHidden]
            public ref class JavaScriptEngineSymbolRegistry sealed
            {
            private:
                JavaScriptEngine^ engine_;
                JavaScriptFunction^ ctor_;
                JavaScriptSymbol ^iterator_ , ^unscopables_, ^hasInstance_ /*, ^match_, ^replace_, ^search_, ^split_, ^isConcatSpreadable_,
                    ^species_, ^toPrimitive_, ^toStringTag_ */;

            internal:
                JavaScriptEngineSymbolRegistry(JavaScriptEngine^ engine);

            public:
                /// <summary>
                /// Gets the global <c>Symbol</c> object.
                /// </summary>
                property JavaScriptFunction^ Constructor
                {
                    JavaScriptFunction^ get();
                }

                /// <summary>
                /// Gets the well-known <c>Symbol.iterator</c> Symbol for the current engine.
                /// </summary>
                property JavaScriptSymbol^ Iterator
                {
                    JavaScriptSymbol^ get();
                }
                /// <summary>
                /// Gets the well-known <c>Symbol.hasInstance</c> Symbol for the current engine.
                /// </summary>
                property JavaScriptSymbol^ HasInstance
                {
                    JavaScriptSymbol^ get();
                }
                /// <summary>
                /// Gets the well-known <c>Symbol.unscopables</c> Symbol for the current engine.
                /// </summary>
                property JavaScriptSymbol^ Unscopables
                {
                    JavaScriptSymbol^ get();
                }
                
                // Edge doesn't yet support these public well-known symbols yet
                /*
                property JavaScriptSymbol^ Match
                {
                    JavaScriptSymbol^ get();
                }
                property JavaScriptSymbol^ Replace
                {
                    JavaScriptSymbol^ get();
                }
                property JavaScriptSymbol^ Search
                {
                    JavaScriptSymbol^ get();
                }
                property JavaScriptSymbol^ Split
                {
                    JavaScriptSymbol^ get();
                }

                property JavaScriptSymbol^ IsConcatSpreadable
                {
                    JavaScriptSymbol^ get();
                }
                property JavaScriptSymbol^ Species
                {
                    JavaScriptSymbol^ get();
                }
                property JavaScriptSymbol^ ToPrimitive
                {
                    JavaScriptSymbol^ get();
                }
                property JavaScriptSymbol^ ToStringTag
                {
                    JavaScriptSymbol^ get();
                }
                */

                /// <summary>
                /// Gets the <c>Symbol</c> for the specified key from the global Symbol registry.
                /// </summary>
                /// <param name="key">The key to the Symbol.</param>
                /// <returns>A JavaScriptSymbol reference if it was found; otherwise <c>null</c>.</returns>
                JavaScriptSymbol^ For(String^ key);
                /// <summary>
                /// Gets the key for a specified <c>Symbol</c>.
                /// </summary>
                /// <param name="symbol">The Symbol to interrogate.</param>
                /// <returns>The name of a Symbol, if one was specified.</returns>
                String^ KeyFor(JavaScriptSymbol^ symbol);
            };
        }
    }
}
