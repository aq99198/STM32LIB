using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace STBootLib
{
    static class STBootTimeout
    {

        public static async Task<T> WithTimeout<T>(this Task<T> task, int timeout)
        {
            /*  */
            var cts = new CancellationTokenSource(timeout);
            /*  */
            var tcs = new TaskCompletionSource<bool>();

            using (cts.Token.Register(s => 
                ((TaskCompletionSource<bool>)s).TrySetResult(true), tcs)) {
                /*  */
                if (task != await Task.WhenAny(task, tcs.Task))
                    throw new OperationCanceledException();
            }

            return await task;
        }
    }
}
