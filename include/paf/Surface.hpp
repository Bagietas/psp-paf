#pragma once

#include <cstddef>

#include <paf/image/Image.hpp>
#include <paf/std/memory.hpp>

namespace paf {
    class SurfacePool;
    class SurfaceCLUT;
    template <class T>
    class SurfaceRCPtr;

    class SurfaceBase {
    public:
        int PixelsToBytes(int) const;
    };

    class Surface : public SurfaceBase {
    public:
        Surface(paf::SurfacePool*, int, int, ImageMode, ImageOrder, bool, int);

        void Copy(int, const void*, ImageOrder, int);
        void AttachCLUT(const paf::SurfaceRCPtr<paf::SurfaceCLUT>&);

        void* operator new(size_t size);

        void* vtable;
        int ref_count;
        char fill[96];
    };

    class SurfaceCLUT {
    public:
        SurfaceCLUT(paf::SurfacePool*, ImageMode, int);

        void Copy(void*);

        void* operator new(size_t size);

        void* vtable;
        int ref_count;
        char fill[28];
    };

    // I hate whoever wrote this back in the day due to how dumb this is, and for example the ptr holds the ref_count :sob:
    template <class T>
    class SurfaceRCPtr {
    public:
        T* ptr;
        int unk_04;
        int unk_08;
        int unk_0C;

        SurfaceRCPtr() : ptr(nullptr), unk_04(0), unk_08(0), unk_0C(0) {}

        SurfaceRCPtr(T* raw_ptr) : ptr(raw_ptr), unk_04(0), unk_08(0), unk_0C(0) {
            if (ptr)
                ptr->ref_count++;
        }

        SurfaceRCPtr(const SurfaceRCPtr& other) {
            ptr = other.ptr;
            unk_04 = other.unk_04;
            unk_08 = other.unk_08;
            unk_0C = other.unk_0C;
            if (ptr)
                ptr->ref_count++;
        }

        SurfaceRCPtr& operator=(const SurfaceRCPtr& other) {
            if (this != &other) {
                release();
                ptr = other.ptr;
                unk_04 = other.unk_04;
                unk_08 = other.unk_08;
                unk_0C = other.unk_0C;
                if (ptr)
                    ptr->ref_count++;
            }
            return *this;
        }

        SurfaceRCPtr& operator=(T* raw_ptr) {
            if (ptr != raw_ptr) {
                release();

                ptr = raw_ptr;

                if (ptr) {
                    ptr->ref_count++;
                }
            }
            return *this;
        }

        T* operator->() {
            return ptr;
        }

        ~SurfaceRCPtr() {
            release();
        }

        void release() {
            if (ptr != nullptr) {
                // why tf ref_count is part of the type rather than holder of the ptr...
                ptr->ref_count--;

                if (ptr->ref_count <= 0) {
                    // very retarded way to do this lmao
                    void** vtable = (void**)ptr->vtable;

                    typedef void (*desc)(T*);
                    desc deleter = (desc)vtable[1];

                    deleter(ptr);
                }

                ptr = nullptr;
            }
        }
    };
}
