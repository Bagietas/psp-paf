#pragma once

#include "paf/widget/PhHandler.hpp"

namespace paf {
    class PhDispatcher : public PhHandler {
    public:
        virtual void unk_0x44();

        int dword1B0;
        char gap1B4[16];
        int dword1C4;
        int dword1C8;
    };
}