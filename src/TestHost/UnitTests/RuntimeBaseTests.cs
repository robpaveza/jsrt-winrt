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
        [DebuggerStepThrough]
        public void DisableExecutionResultsInException()
        {
            runtime_.DisableExecution();
        }

        [TestMethod(ExpectedException = typeof(COMException))]
        [DebuggerStepThrough]
        public void RunIdleWorkResultsInException()
        {
            engine_.RunIdleWork();
        }

        [TestMethod]
        public void ShouldSeeMemoryAllocationEvents()
        {
            Assert.ExpectSuccess();
            runtime_.MemoryChanging += Runtime__MemoryChanging;

            engine_.Execute(new Microsoft.Scripting.ScriptSource("[eval code]", @"(function(global) {
    global.x = new Array(1048576);
})(this);"));
            engine_.Execute(new Microsoft.Scripting.ScriptSource("[eval code]", @"(function(global) {
    delete global.x;
})(this);"));

            Assert.Done();
        }

        private void Runtime__MemoryChanging(object sender, JavaScriptMemoryAllocationEventArgs e)
        {
            Log.Message(string.Format("{0}: {1:x}", e.Type, e.Amount));
            Assert.Succeeded();
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
