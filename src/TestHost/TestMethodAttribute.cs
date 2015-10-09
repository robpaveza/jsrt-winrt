using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHost
{
    [AttributeUsage(AttributeTargets.Method)]
    public sealed class TestMethodAttribute : Attribute
    {
        public TestMethodAttribute() { }

        public Type ExpectedException
        {
            get;
            set;
        }

        public bool RunWithDispatcher
        {
            get;
            set;
        }

        public int MaxTimeoutMs
        {
            get;
            set;
        }
    }
}
