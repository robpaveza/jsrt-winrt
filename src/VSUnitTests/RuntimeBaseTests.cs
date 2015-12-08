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
    public static class TestHelper 
    {
        public static void ExpectedException<T>(Action a) where T :Exception
        {
            try
            {
                a();
                Assert.Fail("Expected exception");
            }
            catch (T exception)
            {
                Assert.IsNotNull(exception);
            }
        }
    }

    [TestClass]
    public class RuntimeBaseTests 
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;

        [TestInitialize]
        public void Setup()
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
                Assert.Fail("No runtime exception should have been raised.");
            };
            var baseline = runtime_.RuntimeMemoryUsage;
            engine_.Execute(new Microsoft.Scripting.ScriptSource("[eval code]", @"(function(global) {
    var x = [];
    for (var i = 0; i < 1024 * 256; i++) {
        x[i] = i;
    }

    global['x'] = x;
})(this);"));

            Assert.IsTrue(runtime_.RuntimeMemoryUsage > baseline);
        }

        [TestMethod]
        [DebuggerStepThrough]
        public void DisableExecutionResultsInException()
        {
            TestHelper.ExpectedException<COMException>(() => runtime_.DisableExecution());
        }

        [TestMethod]
        [DebuggerStepThrough]
        public void RunIdleWorkResultsInException()
        {
            TestHelper.ExpectedException<COMException>(() => engine_.RunIdleWork());
        }

        [TestMethod]
        public void ShouldSeeMemoryAllocationEvents()
        {
            runtime_.MemoryChanging += Runtime__MemoryChanging;

            engine_.Execute(new Microsoft.Scripting.ScriptSource("[eval code]", @"(function(global) {
    global.x = new Array(1048576);
})(this);"));
            engine_.Execute(new Microsoft.Scripting.ScriptSource("[eval code]", @"(function(global) {
    delete global.x;
})(this);"));

            Assert.IsTrue(true, "run to completion is good!");
        }

        private void Runtime__MemoryChanging(object sender, JavaScriptMemoryAllocationEventArgs e)
        {
            Debug.WriteLine(string.Format("{0}: {1:x}", e.Type, e.Amount));
            Assert.IsTrue(true, "run to completion is good!");
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
