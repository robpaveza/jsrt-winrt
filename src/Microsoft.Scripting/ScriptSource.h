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
        /// <summary>
        /// Provides the text content and source location of script code.
        /// </summary>
        [MD::WebHostHidden]
        public ref class ScriptSource sealed
        {
        private:
            JsSourceContext sourceContextId_;
            String^ sourceLoc_;
            String^ sourceText_;
            static volatile JsSourceContext s_current;

        public:
            /// <summary> Initializes a new <c>ScriptSource</c>. </summary>
            /// <param name="sourceLocation">The source location of the script.</param>
            /// <param name="sourceText">The actual script code.</param>
            ScriptSource(String^ sourceLocation, String^ sourceText);

            /// <summary>Gets the origin or location of the source.</summary>
            /// <remarks>
            ///  <para>The source location is used for debugging purposes and for document identification. For example, 
            ///   if a particular script causes an exception to be raised, the stack trace reported will include the 
            ///   source location for the file.</para>
            /// </remarks>
            property String^ SourceLocation
            {
                String^ get();
            }

            /// <summary> Gets the actual script code to be executed. </summary>
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