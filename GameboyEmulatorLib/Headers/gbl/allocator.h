#pragma once

namespace gbl
{

class IGBLAllocator
{
public:
    virtual ~IGBLAllocator() = default;

    virtual void* Allocate(size_t size) = 0;
    virtual void Free(void* ptr) = 0;
};

template<class T>
T* New(IGBLAllocator* allocator)
{
    T* ptr = static_cast<T*>(allocator->Allocate(sizeof(T)));
    new (ptr) T(allocator);
    return ptr;
}

template<class T>
void Delete(T* ptr, IGBLAllocator* allocator)
{
    if (ptr)
    {
        ptr->~T();
        allocator->Free(ptr);
        ptr = nullptr;
    }
}

} // namespace gbl
