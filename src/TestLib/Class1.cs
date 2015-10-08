using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.UI.Core;

namespace TestLib
{
    public sealed class Toaster
    {
        public Toaster()
        {
            crispness_ = 5;
        }
        private int crispness_;

        public int Crispness
        {
            get { return crispness_; }
            set
            {
                if (value < 1 || value > 10)
                    throw new ArgumentException("value");

                crispness_ = value;
            }
        }

        public IAsyncAction ToastAsync()
        {
            var tb = new TaskCompletionSource<int>();
            Timer timer = null;
            var dispatcher = CoreWindow.GetForCurrentThread().Dispatcher;
            timer = new Timer(async (state) =>
            {
                timer.Dispose();
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    tb.SetResult(0);
                });
            }, null, 0, crispness_ * 1000);
            return tb.Task.AsAsyncAction();
        }

        private void OnDone()
        {
            var d = Done;
            if (d != null)
                d(this, null);
        }
        public event EventHandler<object> Done;
    }
}
