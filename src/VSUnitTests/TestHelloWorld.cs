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
    public class TestHelloWorld
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;

        [TestInitialize]
        public void Initialize()
        {
            var settings = new JavaScriptRuntimeSettings();
            runtime_ = new JavaScriptRuntime(settings);
            engine_ = runtime_.CreateEngine();
        }

        [TestMethod]
        public void CallToEchoShouldResultInOneCall()
        {
            engine_.SetGlobalFunction("echo", Echo);
            engine_.Execute(new Microsoft.Scripting.ScriptSource("[eval code]", @"(function() { 
    echo('{0}, {1}!', 'Hello', 'world from JsRT!');
})();"));
        }

        private IJavaScriptValue Echo(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            Debug.WriteLine(string.Format(args.First().ToString(), (object[])args.Skip(1).ToArray()));
            return source.UndefinedValue;
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
