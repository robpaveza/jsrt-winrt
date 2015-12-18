using Microsoft.Scripting.JavaScript;
using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace VSUnitTests
{
    [TestClass]
    public class RuntimeTestsWithScriptInterruptEnabled
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;

        [TestInitialize]
        public void Setup()
        {
            var settings = new JavaScriptRuntimeSettings() { AllowScriptInterrupt = true, };
            runtime_ = new JavaScriptRuntime(settings);
            engine_ = runtime_.CreateEngine();
        }

        [TestMethod]
        [DebuggerStepThrough]
        public void DisableExecutionSucceeds()
        {
            runtime_.DisableExecution();
            Assert.IsTrue(true); // Got past DisableExecution thrown; see RuntimeBaseTests.DisableExecutionResultsInException

            // UNDONE: original test checked for COMException, is Exception OK?
            TestHelper.ExpectedException<Exception>(() =>
            {
                engine_.EvaluateScriptText(@"(function() {})();");
            });
        }

        [TestCleanup]
        public void Cleanup()
        {
            engine_.Dispose();
            engine_ = null;
            runtime_.Dispose();
            runtime_ = null;
        }
    }
}
