using Microsoft.Scripting;
using Microsoft.Scripting.JavaScript;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHost.UnitTests
{
    public class ArrayBufferTests : UnitTest
    {
        private JavaScriptRuntime runtime_;
        private JavaScriptEngine engine_;
        private JavaScriptArrayBuffer buffer_;
        private JavaScriptTypedArray typedArray_;
        private JavaScriptDataView dataView_;

        public override void Setup()
        {
            runtime_ = new JavaScriptRuntime(new JavaScriptRuntimeSettings());
            engine_ = runtime_.CreateEngine();

            var baseline = new ScriptSource("test://init.js", @"(function(global) {
    global.buffer = new ArrayBuffer(1024);
    global.typedArray = new Uint8ClampedArray(buffer);
    global.dataView = new DataView(buffer, 1);
})(this);");
            engine_.Execute(baseline);
            buffer_ = (JavaScriptArrayBuffer)engine_.GetGlobalVariable("buffer");
            typedArray_ = (JavaScriptTypedArray)engine_.GetGlobalVariable("typedArray");
            dataView_ = (JavaScriptDataView)engine_.GetGlobalVariable("dataView");
        }

        [TestMethod]
        public void DefaultPropertiesOfArrayBufferTypesAreCorrect()
        {
            Assert.AreEqual(buffer_.ByteLength, 1024);
            Assert.AreEqual(typedArray_.ByteLength, 1024);
            Assert.AreEqual(typedArray_.ByteOffset, 0);
            Assert.AreEqual((int)typedArray_.ArrayType, (int)JavaScriptTypedArrayType.Uint8Clamped);
            Assert.AreEqual(dataView_.ByteLength, 1023);
            Assert.AreEqual(dataView_.ByteOffset, 1);
        }

        [TestMethod]
        public unsafe void MemoryTypedArrayAndDataViewInteractCorrectly()
        {
            var ibuf = buffer_.GetBackingMemory();
            var memoryAccessor = (IBufferByteAccess)ibuf;
            int* mem = (int*)memoryAccessor.Buffer().ToPointer();
            *mem = 0x01020304;

            Assert.AreEqual((int)engine_.Converter.ToDouble(typedArray_.GetValueAtIndex(engine_.Converter.FromDouble(2))), 2);
            Assert.AreEqual(dataView_.GetInt8(0), (short)3);
            Assert.AreEqual(dataView_.GetInt16(1, true), (short)0x0102);
            Assert.AreEqual(dataView_.GetInt16(1, false), (short)0x0201);
        }

        public override void Cleanup()
        {
            dataView_ = null;
            typedArray_ = null;
            buffer_ = null;

            engine_.Dispose();
            engine_ = null;
            runtime_.Dispose();
            runtime_ = null;
        }

    }
}
