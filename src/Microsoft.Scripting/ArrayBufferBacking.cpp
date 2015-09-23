#include "pch.h"
#include "ArrayBufferBacking.h"

ArrayBufferBackingStore::ArrayBufferBackingStore()
{

}

ArrayBufferBackingStore::~ArrayBufferBackingStore()
{
    backing_ = nullptr;
    capacity_ = 0;
}

void ArrayBufferBackingStore::RuntimeClassInitialize(void* backing, uint32 capacity)
{
    backing_ = backing;
    capacity_ = capacity;
}

IFACEMETHODIMP ArrayBufferBackingStore::get_Capacity(_Out_ uint32* value)
{
    *value = capacity_;
    return S_OK;
}

IFACEMETHODIMP ArrayBufferBackingStore::get_Length(_Out_ uint32* value)
{
    *value = capacity_;
    return S_OK;
}

IFACEMETHODIMP ArrayBufferBackingStore::put_Length(_In_ uint32 value)
{
    UNREFERENCED_PARAMETER(value);
    return S_OK;
}

IFACEMETHODIMP ArrayBufferBackingStore::get_Buffer(_Out_ byte** value)
{
    *value = (byte*)backing_;
    return S_OK;
}
