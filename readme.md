# jsrt-winrt

JsRT-WinRT is a Windows Runtime Component that you can add to your C++ and .NET Windows Store apps to enable them to be scripted with JavaScript.  It uses the same Chakra JavaScript engine that powers Microsoft Edge by interacting with the [JavaScript Runtime Hosting APIs](https://msdn.microsoft.com/en-us/library/dn249552(v=vs.94).aspx), also known as JsRT.  You don't need to use JsRT-WinRT to host the Chakra engine, but I at least think it'll be an easier entry point, particularly for .NET developers who would have to write a lot of P/Invoke interop code.  

## General Principles

JsRT-WinRT tries to provide an API surface which will be familiar both to Windows Runtime developers and JavaScript developers.  The API surface is modeled after the way that JavaScript developers see the world from within the JavaScript environment, well-known globals generally live directly off of the JavaScript engine, and the goal is that it's easy to run code and use it to safely extend your app.

## Getting Started

To start, add a reference to the Microsoft.Scripting.winmd file (or the Microsoft.Scripting project) to your app.  Add a reference to the namespaces in your code file:

    using Microsoft.Scripting;
    using Microsoft.Scripting.JavaScript;

Then create the runtime and engine:

	var settings = new JavaScriptRuntimeSettings();
	var runtime = new JavaScriptRuntime(settings);
    var engine = runtime.CreateEngine();

To add a host-provided function, you pass a delegate into the Engine's `CreateFunction` or `SetGlobalFunction` methods:

	engine.SetGlobalFunction("alert", (eng, construct, thisObj, args) => {
		var message = string.Format(args.First().ToString(), (object[])args.Skip(1).ToArray());
		new Windows.Popups.UI.MessageDialog(message).ShowAsync();
		return eng.UndefinedValue;
	});

Host callback functions get four arguments: 

- The JavaScriptEngine from which the callback is originating.
- Whether the function is being called as a constructor (via the `new` operator).
- The `this` object context, if any (such as if the developer had called `apply` or `bind`)
- The arguments from left-to-right order.

Host callback functions must also return a value.  Because it's JavaScript, that could be `undefined`, which you access via the `UndefinedValue` property on the engine.  

If you wanted to cause an exception to be thrown from your callback, you have two options:

- Just throw an exception from your callback; the JavaScript engine will get an `Error` object with the message in your Exception
- Create any JavaScript value and call `engine.SetException(value)`.
