using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Windows.UI.Core;

namespace TestHost 
{
    public class TestRunner
    {
        private HomeViewModel vm_;
        private Log log_;
        public TestRunner(HomeViewModel viewModel)
        {
            vm_ = viewModel;
            log_ = new Log(viewModel);
            vm_.Dispatcher = CoreWindow.GetForCurrentThread().Dispatcher;
        }

        public Task Execute()
        {
            TestLib.DispatchContainer.GlobalDispatcher = vm_.Dispatcher;

            return Task.Run(async () =>
            {
                try
                {
                    var testContainer = GetTestingAssembly();
                    int failureCount = 0, totalCount = 0;
                    foreach (Type testType in GetTestTypes(testContainer))
                    {
                        UnitTest test = (UnitTest)Activator.CreateInstance(testType);
                        test.Log = log_;
                        int testFailures = 0;

                        foreach (var methodDesc in GetTestMethods(testType))
                        {
                            test.Assert.TestName = methodDesc.Method.DeclaringType.Name + "." + methodDesc.Method.Name;
                            if (methodDesc.RunInDispatcherContext)
                            {
                                EventWaitHandle wh = new EventWaitHandle(false, EventResetMode.AutoReset);
                                test.Assert.WaitHandle = wh;
                                await vm_.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, async () =>
                                {
                                    testFailures = await ExecuteTest(test, testFailures, methodDesc);
                                    wh.Set();
                                });
                                wh.WaitOne(methodDesc.Timeout);
                            }
                            else
                            {
                                testFailures = await ExecuteTest(test, testFailures, methodDesc);
                            }
                        }

                        failureCount += testFailures;
                        totalCount += test.Assert.Count;
                        vm_.AddMessage(string.Format("Completed '{0}' [{1} assertion(s), {2} failure(s)]", testType.Name, test.Assert.Count, testFailures));
                        vm_.UpdateMessages();
                    }

                    vm_.AddMessage(string.Format("All tests completed [{0} assertion(s), {1} failure(s)]", totalCount, failureCount));
                    vm_.UpdateMessages();
                }
                catch (Exception ex)
                {
                    if (Debugger.IsAttached)
                        Debugger.Break();
                }
            });
        }

        private async Task<int> ExecuteTest(UnitTest test, int testFailures, TestExpectations methodDesc)
        {
            test.Setup();

            try
            {
                if (methodDesc.Method.ReturnType == typeof(Task))
                {
                    Task result = methodDesc.Method.Invoke(test, new object[0]) as Task;
                    await result;
                }
                else
                {
                    methodDesc.Method.Invoke(test, new object[0]);
                }

                if (methodDesc.ExpectedExceptionType != null)
                {
                    vm_.AddMessage(string.Format("Expected exception of type '{0}' but no exception was thrown.", methodDesc.ExpectedExceptionType.FullName));
                    testFailures++;
                }

                // success
            }
            catch (TargetInvocationException tie)
            {
                var inner = tie.InnerException;
                var innerType = inner.GetType();
                if (innerType == typeof(AssertionFailedException))
                {
                    vm_.AddMessage(inner.ToString());
                    testFailures++;
                }
                else if (innerType != methodDesc.ExpectedExceptionType)
                {
                    // failure
                    vm_.AddMessage(new AssertionFailedException(test.Assert.TestName, "Threw unexpected exception", inner).ToString());
                    testFailures++;
                }
                else
                {
                    // success
                    if (methodDesc.ExpectedExceptionType != null)
                        test.Assert.Succeeded();
                }
            }
            catch (AssertionFailedException ex)
            {
                vm_.AddMessage(ex.ToString());
                testFailures++;
            }
            catch (Exception ex)
            {
                if (ex.GetType() != methodDesc.ExpectedExceptionType)
                {
                    // failure
                    vm_.AddMessage(ex.ToString());
                    testFailures++;
                }
                else
                {
                    // success
                    if (methodDesc.ExpectedExceptionType != null)
                        test.Assert.Succeeded();
                }
            }

            test.Cleanup();
            return testFailures;
        }

        private Assembly GetTestingAssembly()
        {
            return Assembly.Load(new AssemblyName("TestHost"));
        }

        private IEnumerable<Type> GetTestTypes(Assembly container)
        {
            return container
                        .GetTypes()
                        .Where(t => t.Namespace == "TestHost.UnitTests" && typeof(UnitTest).IsAssignableFrom(t));
        }

        private IEnumerable<TestExpectations> GetTestMethods(Type container)
        {
            return container
                        .GetMethods()
                        .Select(mi => new
                        {
                            Method = mi,
                            Attribute = mi.GetCustomAttribute<TestMethodAttribute>()
                        })
                        .Where(mi => mi.Attribute != null)
                        .Select(mi => new TestExpectations
                        {
                            Method = mi.Method,
                            ExpectedExceptionType = mi.Attribute.ExpectedException,
                            RunInDispatcherContext = mi.Attribute.RunWithDispatcher,
                            Timeout = mi.Attribute.MaxTimeoutMs,
                        });
        }

        private class TestExpectations
        {
            public MethodInfo Method;
            public Type ExpectedExceptionType;
            public bool RunInDispatcherContext;
            public int Timeout;
        }
    }
}
