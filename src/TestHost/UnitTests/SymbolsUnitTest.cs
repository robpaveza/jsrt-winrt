using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHost.UnitTests
{
    public class SymbolsUnitTest : UnitTest
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
        public void SymbolToStringFromScriptShouldEqualSymbolPlusNameInParentheses()
        {
            var fn = engine_.EvaluateScriptText(@"(function() {
    var x = Symbol('foo');
    return x.toString();
})();");
            Assert.AreEqual(fn.Invoke(Enumerable.Empty<IJavaScriptValue>()).ToString(), "Symbol(foo)");
        }

        [TestMethod(ExpectedException = typeof(NotImplementedException))]
        public void SymbolDescriptionThrows()
        {
            var symbol = engine_.CreateSymbol("foo");
            var x = symbol.Description;

            Assert.Failed("Should have thrown.");
        }

        // not a test method because the engine doesn't support it right now
        //[TestMethod]
        public void SymbolShouldCreateAndRenderStringCorrectly()
        {
            engine_.RuntimeExceptionRaised += Engine__RuntimeExceptionRaised;
            var symbol = engine_.CreateSymbol("foo");
            Log.Message(symbol.Description);
            Assert.AreEqual(symbol.Description, "Symbol (foo)");
        }

        private void Engine__RuntimeExceptionRaised(object sender, object e)
        {
            var ex = engine_.GetAndClearException();
            var json = (JavaScriptObject)engine_.GetGlobalVariable("JSON");
            var stringify = (JavaScriptFunction)json.GetPropertyByName("stringify");
            var result = stringify.Invoke(new[] { json, ex });
            var resultText = result.ToString();
            Log.Message(resultText);

            Assert.Failed("An exception was raised by the JavaScript engine.");
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
