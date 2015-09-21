using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.UI.Core;

namespace TestHost
{
    public class HomeViewModel : ViewModelBase
    {
        public HomeViewModel()
        {
            canRun_ = true;
            run_ = new RelayCommand(() => { RunAsync(); });
            text_ = new ObservableCollection<string>();
            messages_ = new List<string>();
        }

        public async Task RunAsync()
        {
            CanRun = false;
            Text.Clear();
            Text.Add("Beginning test run...");
            await InvokeTests();
            CanRun = true;
        }

        private async Task InvokeTests()
        {
            var tr = new TestRunner(this);
            await tr.Execute();
        }

        private ICommand run_;
        public ICommand RunCommand
        {
            get
            {
                return run_;
            }
        }

        private bool canRun_;
        public bool CanRun
        {
            get { return canRun_; }
            set
            {
                canRun_ = value;
                RaisePropertyChanged(() => CanRun);
            }
        }

        private ObservableCollection<string> text_;
        public ObservableCollection<string> Text
        {
            get
            {
                return text_;
            }
        }

        private List<string> messages_;
        public void AddMessage(string message)
        {
            lock (messages_)
            {
                messages_.Add(message);
            }
        }

        public CoreDispatcher Dispatcher
        {
            get; set;
        }

        public async void UpdateMessages()
        {
            await Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                lock (messages_)
                {
                    messages_.ForEach(s => { text_.Add(s); });
                    messages_.Clear();
                }
            });
        }
    }
}
