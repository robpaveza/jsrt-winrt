#pragma once
#include "pch.h"
#include <wrl.h>
#include <wrl/implements.h>
#include <robuffer.h>
#include <windows.storage.streams.h>

using namespace Microsoft::WRL;
using namespace ABI::Windows::Storage::Streams;
using namespace Windows::Storage::Streams;

class ArrayBufferBackingStore : public RuntimeClass<
                                            RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>, 
                                            ABI::Windows::Storage::Streams::IBuffer, 
                                            CloakedIid<IBufferByteAccess>>
{
    InspectableClass(InterfaceName_Windows_Storage_Streams_IBuffer, BaseTrust)

private:
    void* backing_;
    uint32 capacity_;

public:
    ArrayBufferBackingStore();
    virtual ~ArrayBufferBackingStore();
    void RuntimeClassInitialize(void* ptr, uint32 capacity);

    STDMETHOD(get_Capacity)(_Out_ uint32* value);
    STDMETHOD(get_Length)(_Out_ uint32* value);
    STDMETHOD(put_Length)(_In_ uint32 value);
    STDMETHOD(get_Buffer)(_Out_ byte** value);
};