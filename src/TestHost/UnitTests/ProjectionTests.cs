using Microsoft.Scripting;
using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHost.UnitTests
{
    public class ProjectionTests : UnitTest
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;

        [TestMethod]
        public void ScriptCanAccessProjectedNamespace()
        {
            engine_.SetGlobalVariable("echo", engine_.CreateFunction(Echo, "echo"));
            engine_.InitializeWindowsRuntimeNamespace("Windows");
            engine_.InitializeWindowsRuntimeNamespace("TestLib");
            engine_.Execute(new ScriptSource("[eval code]", @"(function(global) {
    echo(Object.keys(global).join(', '));
    echo(JSON.stringify(global));
})(this);"));
        }

        private IJavaScriptValue Echo(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            string arg = args.First().ToString();
            arg = arg.Replace("{", "{{").Replace("}", "}}");
            Log.Message(string.Format(arg, (object[])args.Skip(1).ToArray()));
            return source.UndefinedValue;
        }

        public override void Cleanup()
        {
            engine_.Dispose();
            engine_ = null;
            runtime_.Dispose();
            runtime_ = null;
        }

        public override void Setup()
        {
            runtime_ = new JavaScriptRuntime();
            engine_ = runtime_.CreateEngine();
        }
    }
}
