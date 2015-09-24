#pragma once
#include "pch.h"
#include <windows.storage.streams.h>
#include <robuffer.h>
#include <wrl.h>

class ArrayBufferBackingStore : public Microsoft::WRL::RuntimeClass<
                                            Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRtClassicComMix>,
                                            ABI::Windows::Storage::Streams::IBuffer, 
                                            Windows::Storage::Streams::IBufferByteAccess>
{
    InspectableClass(InterfaceName_Windows_Storage_Streams_IBuffer, BaseTrust)

private:
    void* backing_;
    uint32 capacity_;

public:
    ArrayBufferBackingStore();
    virtual ~ArrayBufferBackingStore();
    HRESULT RuntimeClassInitialize(void* ptr, uint32 capacity);

    STDMETHOD(get_Capacity)(_Out_ uint32* value);
    STDMETHOD(get_Length)(_Out_ uint32* value);
    STDMETHOD(put_Length)(_In_ uint32 value);
    STDMETHOD(Buffer)(_Out_ byte** value);
};