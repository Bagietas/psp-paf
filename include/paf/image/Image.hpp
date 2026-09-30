#pragma once

#include <cstdint>

enum ImageMode {

};

enum ImageOrder {

};

// made up shit by me
struct CLUTBuf {
    void* buf;
    unsigned int buf_len;
};

namespace paf {
    class Image {
    public:
        static paf::Image* OpenGIM(void* buf, unsigned int buf_size);

        ~Image();

        const void* ToBuffer(bool);
        CLUTBuf ToCLUTBuffer(bool); // unsure of the ret type

        virtual void virtual_fill_stub(); // this got only defined to make the vtable get generated, yeah ik I could have just put void* VTable but who cares lol

        char fill[8];
        uint32_t unk_0x0C;
        uint32_t m_unk_idk;
        char fillll[24];
        uint32_t m_width;
        uint32_t m_height;
        char fillarz[48];
        ImageMode m_mode;
        ImageOrder m_order;
        char filluairz[8];
        uint16_t m_alignemtn; // ????
        uint16_t fill_0x76;
        uint32_t fill_0x78;
        ImageMode m_clut_format;
    };
}