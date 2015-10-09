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

        [TestMethod(MaxTimeoutMs = 30000, RunWithDispatcher = true)]
        public async Task ScriptCanAccessProjectedNamespace()
        {
            engine_.SetGlobalVariable("echo", engine_.CreateFunction(Echo, "echo"));
            engine_.InitializeWindowsRuntimeNamespace("Windows");
            engine_.InitializeWindowsRuntimeNamespace("TestLib");
            engine_.Execute(new ScriptSource("[eval code]", @"(function(global) {
    echo(Object.keys(global).join(', '));
    var str = JSON.stringify(global, null, 2);
    //echo(str);

    var toaster = new TestLib.Toaster();
    toaster.addEventListener('done', function(e) {
        echo('Toast completed');
    });
    toaster.toastAsync().then(function()
    {
        echo('toast promise done');
    });
})(this);"));

            await Assert.WaitForSuccessOrFailure(30000);
        }

        private IJavaScriptValue Echo(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            string arg = args.First().ToString();
            arg = arg.Replace("{", "{{").Replace("}", "}}");
            Log.Message(string.Format(arg, (object[])args.Skip(1).ToArray()));

            if (arg == "toast promise done")
            {
                Assert.Succeeded();
            }
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
