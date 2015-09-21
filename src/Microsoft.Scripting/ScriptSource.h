#pragma once
#include "pch.h"
#include "Common.h"

using namespace Platform;
namespace WFM = Windows::Foundation::Metadata;
using namespace WFM;

namespace Microsoft
{
    namespace Scripting
    {
        [MD::WebHostHidden]
        public ref class ScriptSource sealed
        {
        private:
            JsSourceContext sourceContextId_;
            String^ sourceLoc_;
            String^ sourceText_;
            static volatile JsSourceContext s_current;

        public:
            ScriptSource(String^ sourceLocation, String^ sourceText);

            property String^ SourceLocation
            {
                String^ get();
            }

            property String^ SourceText
            {
                String^ get();
            }

        internal:
            property JsSourceContext SourceContextId
            {
                JsSourceContext get();
            }
        };
    };
};