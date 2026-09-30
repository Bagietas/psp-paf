#pragma once

#include "paf/std/string.hpp"
#include <cstdint>
#include <paf/Surface.hpp>

namespace paf {
    class PhWidget;
    class ResourceContext;
    class Surface;

    struct EventFunctionEntry {
        const char* name;
        void* func_ptr;
    };

    class View {
    public:
        class InitParam {
            paf::string m_view_name;
            paf::string m_entry_name; // entrypoint func name ?
            void* m_interf_f0;        // those are some func ptrs that are grabbed from view/module interface
            void* m_interf_f1;
            void* m_interf_f2;
            void* m_interf_f3;
            paf::string m_resource_path;
            int dword34;
            int dword38;
            int dword3C;
            int dword40;
            int dword44;
            int dword48;
            int dword4C;
            paf::string m_executable_path;
            int dword5C;
            int dword60;
            int dword64;
            int dword68;
            int dword6C;
        };

        static paf::View* Find(const char* name);

        void** GetInterface(int id) const;
        wchar_t* GetString(const char* name) const;
        paf::PhWidget* FindWidget(const char* name) const;
        paf::SurfaceRCPtr<Surface> GetTexture(const char* name) const;
        paf::SurfaceRCPtr<Surface> GetTexture(void* tex_define) const;

        int fill;
        paf::string m_name; // I'm not sure about this now lol
        char pad_08[72];
        ResourceContext* resourceCtx;
        char filllll[320];
        paf::SurfacePool* m_surface_pool;
    };

    struct ResourceContext {
        unsigned char pad_00[0x0C];
        int32_t* nodeBase;
    };
}