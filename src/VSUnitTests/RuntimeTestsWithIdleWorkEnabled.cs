using Microsoft.Scripting.JavaScript;
using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VSUnitTests
{
    [TestClass]
    public class RuntimeTestsWithIdleWorkEnabled
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;

        [TestInitialize]
        public void Setup()
        {
            var settings = new JavaScriptRuntimeSettings() { EnableIdleProcessing = true, };
            runtime_ = new JavaScriptRuntime(settings);
            engine_ = runtime_.CreateEngine();
        }

        [TestMethod]
        public void RunIdleWorkSucceeds()
        {
            engine_.RunIdleWork();
            Assert.IsTrue(true); // Got past RunIdleWork thrown; see RuntimeBaseTests.RunIdleWorkResultsInException
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
