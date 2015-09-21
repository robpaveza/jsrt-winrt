using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHost.UnitTests
{
    public class TestHelloWorld : UnitTest
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
        public void CallToEchoShouldResultInOneCall()
        {
            engine_.SetGlobalFunction("echo", Echo);
            var fn = engine_.EvaluateScriptText(@"(function() { 
    echo('{0}, {1}!', 'Hello', 'world from JsRT!');
})();");
            fn.Invoke(Enumerable.Empty<IJavaScriptValue>());
        }

        private IJavaScriptValue Echo(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            Log.Message(string.Format(args.First().ToString(), (object[])args.Skip(1).ToArray()));
            return source.UndefinedValue;
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
