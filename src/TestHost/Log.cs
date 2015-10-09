using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Core;

namespace TestHost
{
    public interface ILog
    {
        void Message(string text);
    }

    class Log : ILog
    {
        private HomeViewModel vm_;
        public Log(HomeViewModel vm)
        {
            vm_ = vm;
        }

        public void Message(string text)
        {
            vm_.AddMessage(string.Format("[Log]: {0}", text));
            vm_.UpdateMessages();
        }
    }

    public static class LogExtensions
    {
        public static void ProjectToJavaScript(this ILog log, JavaScriptEngine engine, string globalName = "Log")
        {
            var obj = engine.CreateObject(null);
            obj.SetPropertyByName("message", engine.CreateFunction((e, ctor, thisObj, args) =>
            {
                var msg = string.Format(args.First().ToString(), (object[])args.Skip(1).ToArray());
                log.Message(msg);

                return e.UndefinedValue;
            }, "Log.message"));

            engine.SetGlobalVariable(globalName, obj);
        }
    }
}
