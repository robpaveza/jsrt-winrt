using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TestHost.UnitTests
{
    public class RuntimeBaseTests : UnitTest
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;

        public override void Setup()
        {
            var settings = new JavaScriptRuntimeSettings();
            runtime_ = new JavaScriptRuntime(settings);
            engine_ = runtime_.CreateEngine();
        }

        [TestMethod]
        public void MemoryUsageShouldGrowWhenAllocationIsPreservedOnGlobal()
        {
            engine_.RuntimeExceptionRaised += (s, e) =>
            {
                Assert.Failed("No runtime exception should have been raised.");
            };
            var baseline = runtime_.RuntimeMemoryUsage;
            var func = engine_.EvaluateScriptText(@"(function(global) {
    var x = [];
    for (var i = 0; i < 1024 * 256; i++) {
        x[i] = i;
    }

    global['x'] = x;
})(this);");

            func.Invoke(Enumerable.Empty<IJavaScriptValue>());

            Assert.IsGreaterThan(runtime_.RuntimeMemoryUsage, baseline);
        }

        [TestMethod(ExpectedException = typeof(COMException))]
        public void DisableExecutionResultsInException()
        {
            runtime_.DisableExecution();
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
