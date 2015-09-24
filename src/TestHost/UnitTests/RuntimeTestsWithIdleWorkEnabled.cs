using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHost.UnitTests
{
    public class RuntimeTestsWithIdleWorkEnabled : UnitTest
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;

        public override void Setup()
        {
            var settings = new JavaScriptRuntimeSettings() { EnableIdleProcessing = true, };
            runtime_ = new JavaScriptRuntime(settings);
            engine_ = runtime_.CreateEngine();
        }

        [TestMethod]
        public void RunIdleWorkSucceeds()
        {
            engine_.RunIdleWork();
            Assert.Succeeded(); // Got past RunIdleWork thrown; see RuntimeBaseTests.RunIdleWorkResultsInException
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
