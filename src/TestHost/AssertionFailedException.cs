using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHost
{
    public class AssertionFailedException : Exception
    {
        public AssertionFailedException(string testName, string failureMode)
            : base(string.Format("Test assertion failed:\n\tTest name: {0}\n\tDetails: {1}\n", testName, failureMode))
        {

        }

        public AssertionFailedException(string testName, string failureMode, Exception inner)
            : base(string.Format("Test assertion failed:\n\tTest name: {0}\n\tDetails: {1}\n\tInner exception stack trace:\n{2}", testName, failureMode, inner))
        {

        }
    }
}
