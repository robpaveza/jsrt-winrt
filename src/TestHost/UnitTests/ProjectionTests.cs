using Microsoft.Scripting;
using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Popups;

namespace TestHost.UnitTests
{
    public class ProjectionTests : UnitTest
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;

        [TestMethod(MaxTimeoutMs = 30000, RunWithDispatcher = true)]
        public async Task ScriptCanAccessProjectedNamespace()
        {
            engine_.SetGlobalVariable("echo", engine_.CreateFunction(EchoForProjectedNamespace, "echo"));
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
        private IJavaScriptValue EchoForProjectedNamespace(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
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

        [TestMethod(MaxTimeoutMs = 30000, RunWithDispatcher = true)]
        public async Task ScriptCanAccessProjectedObjectWithoutNamespace()
        {
            engine_.SetGlobalVariable("echo", engine_.CreateFunction(EchoForNoNamespace, "echo"));
            engine_.SetGlobalVariable("Dialog", engine_.CreateFunction(MessageDialog, "Dialog"));
            Assert.ProjectToJavaScript(engine_, "Assert");
            Log.ProjectToJavaScript(engine_);
            engine_.InitializeWindowsRuntimeNamespace("Windows.UI.Popups");
            engine_.Execute(new ScriptSource("[eval code]", @"(function(global) {
    try
    {
        var y = Dialog('Hi there!  I should fail.');
        Assert.failed('Should not be able to create a Dialog() instance without new()');
    }
    catch (e) 
    {
        Log.message('Caught error: ' + e.toString());
        Assert.succeeded();
    }

    var dlg = new Dialog('Hello, world!  Click ""OK"" to make the test pass.');
    dlg.showAsync().then(function() {
        echo('callback completed');
    });
})(this);"));

            await Assert.WaitForSuccessOrFailure(30000);
        }

        private IJavaScriptValue EchoForNoNamespace(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            string arg = args.First().ToString();

            if (arg == "callback completed")
            {
                Assert.Succeeded();
            }
            return source.UndefinedValue;
        }

        private IJavaScriptValue MessageDialog(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            if (!construct)
            {
                var undefined = source.UndefinedValue;
                source.SetException(source.CreateTypeError("Must call as a constructor."));
                return undefined;
            }

            var dlg = new MessageDialog(args.First().ToString());
            return source.Converter.FromWindowsRuntimeObject(dlg);
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
