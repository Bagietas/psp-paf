#pragma once

#include "pspiofilemgr.h"
#include <paf/Surface.hpp>
#include <paf/image/Image.hpp>
#include <paf/std/memory.hpp>
#include <paf/std/vector.hpp>

namespace paf {
    class SurfacePool;
};

namespace util {
    int pspsh_printf(const char* fmt, ...) {
        char buffer[1024];

        va_list args;
        va_start(args, fmt);

        int len = sce_paf_private_vsnprintf(buffer, sizeof(buffer), fmt, args);

        va_end(args);

        if (len < 0)
            return len;

        if (len >= (int)sizeof(buffer))
            len = sizeof(buffer) - 1;

        return sceIoWrite(1, buffer, len);
    }
}

// this code is so shitty, that it's just made to work, nothing else
paf::SurfaceRCPtr<paf::Surface> GetTextureFromGim(void* gim_buf, unsigned int gim_buf_len, paf::SurfacePool* pool) {
    paf::SurfaceRCPtr<paf::Surface> tex;

    if (!gim_buf || !pool) return tex;

    paf::Image* img = paf::Image::OpenGIM(gim_buf, gim_buf_len);
    if (!img) return tex;

    const void* buf = img->ToBuffer(true);
    if (!buf) {
        delete img;
        return tex;
    }

    tex = new paf::Surface(pool, img->m_width, img->m_height, img->m_mode, img->m_order, false, img->m_unk_idk);
    int aligned_len = (tex->PixelsToBytes(img->m_width) + img->m_alignemtn - 1) / img->m_alignemtn * img->m_alignemtn;
    tex->Copy(0, buf, img->m_order, aligned_len);

    if (img->m_mode >= 4 && img->m_mode <= 7) {
        CLUTBuf clut_buf = img->ToCLUTBuffer(true);

        if (clut_buf.buf && img->m_clut_format >= 0 && img->m_clut_format <= 3) {

            int entryCount = (img->m_clut_format < 3) ? (clut_buf.buf_len >> 1) : (clut_buf.buf_len >> 2);

            paf::SurfaceRCPtr<paf::SurfaceCLUT> clut = new paf::SurfaceCLUT(pool, img->m_clut_format, entryCount);

            clut->Copy(clut_buf.buf);
            tex->AttachCLUT(clut);
        }
    }

    delete img;
    return tex;
}

paf::SurfaceRCPtr<paf::Surface> GetXmbCachedTexture(void* cache, const char* tex_name) {
    paf::SurfaceRCPtr<paf::Surface> result = paf::SurfaceRCPtr<paf::Surface>();

    if (!cache || !tex_name)
        return result;

    uint8_t* base = (uint8_t*)(cache);

    for (int block = 0; block < 4; ++block) {
        uint8_t* block_ptr = base + block * 0x10;

        int count = *(int*)block_ptr;
        const char* strings = *(const char**)(block_ptr + 0x04);
        uint8_t* data = *(uint8_t**)(block_ptr + 0x08);

        for (int row = 0; row < count; ++row) {
            const char* string_ptr = strings + row * 0x14;

            int col = -1;

            if (sce_paf_private_strcmp(string_ptr + 0x00, tex_name) == 0)
                col = 0;
            else if (sce_paf_private_strcmp(string_ptr + 0x04, tex_name) == 0)
                col = 1;
            else if (sce_paf_private_strcmp(string_ptr + 0x08, tex_name) == 0)
                col = 2;

            if (col < 0)
                continue;

            uint8_t* column = data + row * 0x58 + 0x04 + col * 0x1C;
            paf::Surface* surface = *(paf::Surface**)(column + 0x18);

            // idk who tf wrote that pseudo shared_ptr but it's weird for sure
            // or I'm just blind, but this works tm
            if (surface) {
                result.ptr = surface;
                result.ptr->ref_count++;
            }

            return result;
        }
    }

    return result;
}