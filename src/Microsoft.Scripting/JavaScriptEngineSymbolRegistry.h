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
            public ref class JavaScriptEngineSymbolRegistry sealed
            {
            private:
                JavaScriptEngine^ engine_;
                JavaScriptFunction^ ctor_;
                JavaScriptSymbol ^iterator_, ^match_, ^replace_, ^search_, ^split_, ^hasInstance_, ^isConcatSpreadable_,
                    ^unscopables_, ^species_, ^toPrimitive_, ^toStringTag_;

            internal:
                JavaScriptEngineSymbolRegistry(JavaScriptEngine^ engine);

            public:
                property JavaScriptFunction^ Constructor
                {
                    JavaScriptFunction^ get();
                }
                property JavaScriptSymbol^ Iterator
                {
                    JavaScriptSymbol^ get();
                }

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

                property JavaScriptSymbol^ HasInstance
                {
                    JavaScriptSymbol^ get();
                }
                property JavaScriptSymbol^ IsConcatSpreadable
                {
                    JavaScriptSymbol^ get();
                }
                property JavaScriptSymbol^ Unscopables
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

                JavaScriptSymbol^ For(String^ key);
                String^ KeyFor(JavaScriptSymbol^ symbol);
            };
        }
    }
}
