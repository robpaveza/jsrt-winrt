using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHost
{
    public abstract class UnitTest
    {
        protected UnitTest()
        {
            Assert = new Asserter();
        }

        public abstract void Setup();

        public abstract void Cleanup();

        public ILog Log { get; set; }

        public Asserter Assert
        {
            get; private set;
        }
    }
}
