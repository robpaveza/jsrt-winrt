#pragma once
#include "pch.h"
#include "Common.h"

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            /// <summary>
            /// Represents a JavaScript <c>DataView</c>.
            /// </summary>
            [MD::WebHostHidden]
            public ref class JavaScriptDataView sealed :
                public IJavaScriptObject
            {
            private:
                JsValueRef handle_;
                JavaScriptPrimitiveValue^ primitive_;
                JavaScriptObject^ object_;
                JavaScriptEngine^ engine_;

            internal:
                JavaScriptDataView(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object);

            public:
                /// <summary>
                /// Gets a reference to the underlying buffer.
                /// </summary>
                property JavaScriptArrayBuffer^ Buffer
                {
                    JavaScriptArrayBuffer^ get();
                }
                /// <summary>
                /// Gets the byte-length of the typed array's memory space.
                /// </summary>
                property uint32 ByteLength
                {
                    uint32 get();
                }
                /// <summary>
                /// Gets the byte offset of the typed array from the beginning of the underlying buffer.
                /// </summary>
                property uint32 ByteOffset
                {
                    uint32 get();
                }

                /// <summary>
                /// Gets a signed 8-bit integer (byte) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                int16 GetInt8(uint32 byteOffset);
                /// <summary>
                /// Gets an unsigned 8-bit integer (unsigned byte) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                uint8 GetUint8(uint32 byteOffset);
                /// <summary>
                /// Gets a signed 16-bit integer (short) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                int16 GetInt16(uint32 byteOffset);
                /// <summary>
                /// Gets a signed 16-bit integer (short) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="littleEndian">True to read as little-endian; otherwise read as big-endian.</param>
                int16 GetInt16(uint32 byteOffset, bool littleEndian);
                /// <summary>
                /// Gets an unsigned 16-bit integer (unsigned short) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                uint16 GetUint16(uint32 byteOffset);
                /// <summary>
                /// Gets an unsigned 16-bit integer (unsigned short) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="littleEndian">True to read as little-endian; otherwise read as big-endian.</param>
                uint16 GetUint16(uint32 byteOffset, bool littleEndian);
                /// <summary>
                /// Gets a signed 32-bit integer (long) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                int32 GetInt32(uint32 byteOffset);
                /// <summary>
                /// Gets a signed 32-bit integer (long) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="littleEndian">True to read as little-endian; otherwise read as big-endian.</param>
                int32 GetInt32(uint32 byteOffset, bool littleEndian);
                /// <summary>
                /// Gets an unsigned 32-bit integer (unsigned long) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                uint32 GetUint32(uint32 byteOffset);
                /// <summary>
                /// Gets an unsigned 32-bit integer (unsigned long) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="littleEndian">True to read as little-endian; otherwise read as big-endian.</param>
                uint32 GetUint32(uint32 byteOffset, bool littleEndian);
                /// <summary>
                /// Gets a signed 32-bit float (float) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                float32 GetFloat32(uint32 byteOffset);
                /// <summary>
                /// Gets a signed 32-bit float (float) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="littleEndian">True to read as little-endian; otherwise read as big-endian.</param>
                float32 GetFloat32(uint32 byteOffset, bool littleEndian);
                /// <summary>
                /// Gets a signed 64-bit float (double) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                float64 GetFloat64(uint32 byteOffset);
                /// <summary>
                /// Gets a signed 64-bit float (double) at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="littleEndian">True to read as little-endian; otherwise read as big-endian.</param>
                float64 GetFloat64(uint32 byteOffset, bool littleEndian);


                /// <summary>
                /// Stores a signed 8-bit integer (byte) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                void SetInt8(uint32 byteOffset, int16 value);
                /// <summary>
                /// Stores an unsigned 8-bit integer (unsigned byte) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                void SetUint8(uint32 byteOffset, uint8 value);
                /// <summary>
                /// Stores a signed 16-bit integer (short) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                void SetInt16(uint32 byteOffset, int16 value);
                /// <summary>
                /// Stores a signed 16-bit integer (short) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                /// <param name="littleEndian">True to store as little-endian; otherwise store as big-endian.</param>
                void SetInt16(uint32 byteOffset, int16 value, bool littleEndian);
                /// <summary>
                /// Stores an unsigned 16-bit integer (ushort) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                void SetUint16(uint32 byteOffset, uint16 value);
                /// <summary>
                /// Stores an unsigned 16-bit integer (ushort) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                /// <param name="littleEndian">True to store as little-endian; otherwise store as big-endian.</param>
                void SetUint16(uint32 byteOffset, uint16 value, bool littleEndian);
                /// <summary>
                /// Stores a signed 32-bit integer (int) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                void SetInt32(uint32 byteOffset, int32 value);
                /// <summary>
                /// Stores a signed 32-bit integer (int) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                /// <param name="littleEndian">True to store as little-endian; otherwise store as big-endian.</param>
                void SetInt32(uint32 byteOffset, int32 value, bool littleEndian);
                /// <summary>
                /// Stores an unsigned 32-bit integer (uint) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                void SetUint32(uint32 byteOffset, uint32 value);
                /// <summary>
                /// Stores an unsigned 32-bit integer (uint) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                /// <param name="littleEndian">True to store as little-endian; otherwise store as big-endian.</param>
                void SetUint32(uint32 byteOffset, uint32 value, bool littleEndian);
                /// <summary>
                /// Stores a 32-bit float (float) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                void SetFloat32(uint32 byteOffset, float32 value);
                /// <summary>
                /// Stores a 32-bit float (float) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                /// <param name="littleEndian">True to store as little-endian; otherwise store as big-endian.</param>
                void SetFloat32(uint32 byteOffset, float32 value, bool littleEndian);
                /// <summary>
                /// Stores a 64-bit integer (double) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                void SetFloat64(uint32 byteOffset, float64 value);
                /// <summary>
                /// Stores a 64-bit integer (double) value at the specified byte offset from the start of the view.
                /// </summary>
                /// <param name="byteOffset">The offset from the beginning of the DataView's view of the underlying <c>ArrayBuffer</c>.</param>
                /// <param name="value">The value to store.</param>
                /// <param name="littleEndian">True to store as little-endian; otherwise store as big-endian.</param>
                void SetFloat64(uint32 byteOffset, float64 value, bool littleEndian);

                DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS(JavaScriptDataView)
            };
        }
    }
}
