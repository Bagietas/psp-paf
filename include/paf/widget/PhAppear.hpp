#pragma once

#include "paf/geom.hpp"

namespace paf {
    class PhAppear {
    public:
        // those are obv guessed as they are not getting exported outside of paf
        virtual ~PhAppear();
        virtual void unk_0x08();
        virtual void SetStyle(int, bool);
        virtual void SetStyle(int, int);
        virtual void SetStyle(int, float);
        virtual void SetStyle(int, const vec4&);
        virtual void SetStyle(int, int, bool); // not sure if this is SetStyle even lol(and the ones below) virtual void SetStyle(int, int, int);
        virtual void SetStyle(int, int, float);
        virtual void SetStyle(int, int, const vec4&);
        virtual void unk_0x2C();
        virtual void unk_0x30();
        virtual void unk_0x34();
        virtual void unk_0x38();
        virtual void unk_0x3C();
        virtual void unk_0x40();
        virtual void unk_0x44();
        virtual void unk_0x48();
        virtual void GetStyle(int, bool&); // sub_157010
        virtual void GetStyle(int, int&);
        virtual void GetStyle(int, float&);
        virtual void GetStyle(int, const vec4&);
        virtual void GetStyle(int, int, bool&);
        virtual void GetStyle(int, int, int&);
        virtual void GetStyle(int, int, float&);
        virtual void GetStyle(int, int, const vec4&);
        virtual void unk_0x6C();
        virtual void unk_0x70();
        virtual void unk_0x74();
        virtual void unk_0x78();
        virtual void unk_0x7C();
        virtual void unk_0x80();
        virtual void unk_0x84();
        virtual void unk_0x88();
        virtual void unk_0x8C();
    };
}