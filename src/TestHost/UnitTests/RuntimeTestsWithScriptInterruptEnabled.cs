using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TestHost.UnitTests
{
    public class RuntimeTestsWithScriptInterruptEnabled : UnitTest
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;

        public override void Setup()
        {
            var settings = new JavaScriptRuntimeSettings() { AllowScriptInterrupt = true, };
            runtime_ = new JavaScriptRuntime(settings);
            engine_ = runtime_.CreateEngine();
        }

        [TestMethod(ExpectedException = typeof(COMException))]
        [DebuggerStepThrough]
        public void DisableExecutionSucceeds()
        {
            runtime_.DisableExecution();
            Assert.Succeeded(); // Got past DisableExecution thrown; see RuntimeBaseTests.DisableExecutionResultsInException

            engine_.EvaluateScriptText(@"(function() {})();");
        }

        public override void Cleanup()
        {
            engine_.Dispose();
            engine_ = null;
            runtime_.Dispose();
            runtime_ = null;
        }
    }
}
