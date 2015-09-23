using Microsoft.Scripting;
using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Diagnostics;
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
        [DebuggerStepThrough]
        public void SymbolDescriptionThrows()
        {
            var symbol = engine_.CreateSymbol("foo");
            var x = symbol.Description;

            Assert.Failed("Should have thrown.");
        }

        [TestMethod]
        public void InformationalDisplayOfAllSymbolProperties()
        {
            engine_.RuntimeExceptionRaised += Engine__RuntimeExceptionRaised;
            engine_.SetGlobalFunction("echo", (eng, ctor, thisObj, args) =>
            {
                Log.Message(string.Format(args.First().ToString(), (object[])args.Skip(1).ToArray()));
                Assert.AreEqual(args.First().ToString(), "length, prototype, name, hasInstance, iterator, unscopables, for, keyFor, caller, arguments");
                return eng.UndefinedValue;
            });

            var src = new ScriptSource("[eval code]", @"(function() {
    var x = Object.getOwnPropertyNames(Symbol).join(', ');

    echo(x);
})();");
            engine_.Execute(src);
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
