using Microsoft.Scripting;
using Microsoft.Scripting.JavaScript;
using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TestLib;
using Windows.UI.Core;
using Windows.UI.Popups;

namespace VSUnitTests
{
    [TestClass]
    public class ProjectionTests
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;
        TaskCompletionSource<int> successSignal;

        [TestMethod/* (MaxTimeoutMs = 30000, RunWithDispatcher = true) */]
        public async Task ScriptCanAccessProjectedNamespace()
        {
            successSignal = new TaskCompletionSource<int>();
            await DispatchContainer.GlobalDispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
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
            });

            await successSignal.Task;
        }
        private IJavaScriptValue EchoForProjectedNamespace(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            string arg = args.First().ToString();
            arg = arg.Replace("{", "{{").Replace("}", "}}");
            Debug.WriteLine(string.Format(arg, (object[])args.Skip(1).ToArray()));

            if (arg == "toast promise done")
            {
                successSignal.SetResult(0);
            }
            return source.UndefinedValue;
        }

        [TestMethod/* (MaxTimeoutMs = 30000, RunWithDispatcher = true) */]
        public async Task ScriptCanAccessProjectedObjectWithoutNamespace()
        {
            successSignal = new TaskCompletionSource<int>();
            await DispatchContainer.GlobalDispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                engine_.SetGlobalVariable("echo", engine_.CreateFunction(EchoForNoNamespace, "echo"));
                engine_.SetGlobalVariable("Dialog", engine_.CreateFunction(MessageDialog, "Dialog"));
                engine_.SetGlobalVariable("succeeded", engine_.CreateFunction(Success, "succeeded"));
                engine_.SetGlobalVariable("failed", engine_.CreateFunction(Failure, "failed"));
                //UNDONE: Assert.ProjectToJavaScript(engine_, "Assert");
                //UNDONE: Log.ProjectToJavaScript(engine_);
                engine_.InitializeWindowsRuntimeNamespace("Windows.UI.Popups");
                engine_.Execute(new ScriptSource("[eval code]", @"(function(global) {
    try
    {
        var y = Dialog('Hi there!  I should fail.');
        failed('Should not be able to create a Dialog() instance without new()');
    }
    catch (e) 
    {
        // Log.message('Caught error: ' + e.toString());
        succeeded();
    }

    var dlg = new Dialog('Hello, world!  Click ""OK"" to make the test pass.');
    dlg.showAsync().then(function() {
        echo('callback completed');
    });
})(this);"));
            });

            await successSignal.Task;
        }

        private IJavaScriptValue Success(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            successSignal.SetResult(0);
            return source.UndefinedValue;
        }
        private IJavaScriptValue Failure(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            Assert.Fail();
            return source.UndefinedValue;
        }

        private IJavaScriptValue EchoForNoNamespace(JavaScriptEngine source, bool construct, IJavaScriptValue thisValue, IEnumerable<IJavaScriptValue> args)
        {
            string arg = args.First().ToString();

            Assert.AreEqual("callback completed", arg);
            successSignal.SetResult(0);
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

        
        [TestCleanup]
        public async Task Cleanup()
        {
            await DispatchContainer.GlobalDispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                engine_.Dispose();
                engine_ = null;
                runtime_.Dispose();
                runtime_ = null;
            });
        }


        [TestInitialize]
        public async Task Setup()
        {
            await DispatchContainer.GlobalDispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                runtime_ = new JavaScriptRuntime();
                engine_ = runtime_.CreateEngine();
            });
        }
    }
}
