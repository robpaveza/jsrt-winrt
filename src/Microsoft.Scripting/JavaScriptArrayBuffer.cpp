#include "pch.h"
#include "ArrayBufferBacking.h"
#include "Common.h"
#include "JavaScriptArrayBuffer.h"
#include "JavaScriptPrimitiveValue.h"
#include "JavaScriptEngine.h"
#include "JavaScriptObject.h"

using namespace Microsoft::Scripting::JavaScript;
using namespace Microsoft::WRL;

JavaScriptArrayBuffer::JavaScriptArrayBuffer(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object):
    primitive_(primitive),
    object_(object),
    handle_(GetHandleFromVar(primitive)),
    engine_(primitive->Engine)
{

}

Windows::Storage::Streams::IBuffer^ JavaScriptArrayBuffer::GetBackingMemory()
{
    engine_->ClaimContext();
    BYTE* buffer;
    uint32 len;
    ObjCheckForFailure1(JsGetArrayBufferStorage(handle_, &buffer, &len));

    ComPtr<ArrayBufferBackingStore> buf;
    HRESULT hr = MakeAndInitialize<ArrayBufferBackingStore>(&buf, (void*)buffer, len);
    if (FAILED(hr))
        throw ref new Exception(hr);

    auto inspectable = reinterpret_cast<IInspectable*>(buf.Detach());
    auto result = reinterpret_cast<Windows::Storage::Streams::IBuffer^>(inspectable);

    return result;
}

uint32 JavaScriptArrayBuffer::ByteLength::get()
{
    std::call_once(gotLen_, [&]() {
        this->engine_->ClaimContext();

        BYTE* buffer;
        uint32 len;
        ObjCheckForFailure1(JsGetArrayBufferStorage(handle_, &buffer, &len));
        len_ = len;
    });
    
    return len_;
}

DECLARE_JAVASCRIPT_OBJECT_IMPLEMENTATION(JavaScriptArrayBuffer)
