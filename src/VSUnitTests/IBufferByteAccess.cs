using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace VSUnitTests
{
    [ComImport]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    [Guid("905a0fef-bc53-11df-8c49-001e4fc686da")]
    internal interface IBufferByteAccess
    {
        IntPtr Buffer();
    }
}
