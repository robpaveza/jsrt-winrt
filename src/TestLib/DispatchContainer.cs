using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Core;

namespace TestLib
{
    public static class DispatchContainer
    {
        public static CoreDispatcher GlobalDispatcher
        {
            get; set;
        }
    }
}
