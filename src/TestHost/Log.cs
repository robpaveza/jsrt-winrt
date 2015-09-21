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
}
