#include "pch.h"
#include "JavaScriptArrayBuffer.h"
#include "JavaScriptPrimitiveValue.h"
#include "ArrayBufferBacking.h"
#include "JavaScriptEngine.h"
#include <wrl.h>

using namespace Microsoft::Scripting::JavaScript;

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

    /*ComPtr<ArrayBufferBackingStore> buf;
    HRESULT hr = MakeAndInitialize<ArrayBufferBackingStore>(&buf, (void*)buffer, len);
    auto inspectable = reinterpret_cast<IInspectable*>(buf.Detach());
    auto result = reinterpret_cast<Windows::Storage::Streams::IBuffer^>(inspectable);

    return result;*/
    return nullptr;
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
