using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestHost
{
    public class Asserter
    {
        private int count_;
        private bool expectingSuccess_;
        private bool waiting_, flagged_;

        public string TestName
        {
            get; set;
        }

        public int Count
        {
            get { return count_; }
        }

        public void Failed(string message)
        {
            count_++;
            expectingSuccess_ = false;
            ReleaseWait();
            throw new AssertionFailedException(TestName, message);
        }
        
        public void ExpectSuccess()
        {
            expectingSuccess_ = true;
        }

        public void Succeeded()
        {
            count_++;
            expectingSuccess_ = false;
            ReleaseWait();
        }

        public async Task WaitForSuccessOrFailure(int msTimeout)
        {
            // fishy
            ExpectSuccess();
            waiting_ = true;
            int iterationCount = 10;
            while (iterationCount > 0)
            {
                await Task.Delay(msTimeout / 10);

                if (waiting_ && flagged_)
                {
                    waiting_ = flagged_ = false;
                    break;
                }
                iterationCount--;
            }

            Failed("Timeout of " + msTimeout + "ms expired waiting for success or failure.");
        }

        public void Done()
        {
            if (expectingSuccess_)
            {
                Failed("Expected success but didn't observe.");
            }
        }

        private void ReleaseWait()
        {
            var wh = WaitHandle;
            if (wh != null)
                wh.Set();
            WaitHandle = null;
        }

        public EventWaitHandle WaitHandle
        {
            get;
            set;
        }

        #region AreEqual
        public void AreEqual<T>(T actual, T expected)
            where T : class, IEquatable<T>
        {
            count_ ++;
            if (actual == null && expected == null)
                return;
            if (actual == null)
                throw new AssertionFailedException(TestName, string.Format("Expected 'null' to be '{0}'", expected));
            if (expected == null)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be 'null'", actual));
            if (actual.Equals(expected))
                return;

            throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(byte actual, byte expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(sbyte actual, sbyte expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(short actual, short expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(ushort actual, ushort expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(int actual, int expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(uint actual, uint expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(long actual, long expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(ulong actual, ulong expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(DateTime actual, DateTime expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreEqual(TimeSpan actual, TimeSpan expected)
        {
            count_++;
            if (actual != expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }
        #endregion

        #region AreNotEqual
        public void AreNotEqual<T>(T actual, T expected)
            where T : class, IEquatable<T>
        {
            count_++;
            if (actual == null && expected == null)
                throw new AssertionFailedException(TestName, "Expected 'null' to not be 'null'");
            if (actual == null || expected == null)
                return;
            if (!actual.Equals(expected))
                return;

            throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to not be '{1}'", actual, expected));
        }

        public void AreNotEqual(byte actual, byte expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreNotEqual(sbyte actual, sbyte expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreNotEqual(short actual, short expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreNotEqual(ushort actual, ushort expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreNotEqual(int actual, int expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreNotEqual(uint actual, uint expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreNotEqual(long actual, long expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreNotEqual(ulong actual, ulong expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreNotEqual(DateTime actual, DateTime expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }

        public void AreNotEqual(TimeSpan actual, TimeSpan expected)
        {
            count_++;
            if (actual == expected)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be '{1}'", actual, expected));
        }
        #endregion

        #region IsTrue / IsFalse
        public void IsTrue(bool val)
        {
            count_++;
            if (!val)
                throw new AssertionFailedException(TestName, "Expected value to be true.");
        }

        public void IsFalse(bool val)
        {
            count_++;
            if (val)
                throw new AssertionFailedException(TestName, "Expected value to be false.");
        }
        #endregion

        #region Greater than / less than
        public void IsGreaterThan<T>(T newValue, T comparand)
            where T : struct, IComparable<T>
        {
            count_++;
            if (newValue.CompareTo(comparand) <= 0)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be greater than '{1}'", newValue, comparand));
        }

        public void IsGreaterThanOrEqualTo<T>(T newValue, T comparand)
            where T : struct, IComparable<T>
        {
            count_++;
            if (newValue.CompareTo(comparand) < 0)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be greater than or equal to '{1}'", newValue, comparand));
        }

        public void IsLessThan<T>(T newValue, T comparand)
            where T : struct, IComparable<T>
        {
            count_++;
            if (newValue.CompareTo(comparand) >= 0)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be less than '{1}'", newValue, comparand));
        }

        public void IsLessThanOrEqualTo<T>(T newValue, T comparand)
            where T : struct, IComparable<T>
        {
            count_++;
            if (newValue.CompareTo(comparand) > 0)
                throw new AssertionFailedException(TestName, string.Format("Expected '{0}' to be less than or equal to '{1}'", newValue, comparand));
        }

        #endregion
    }
}
