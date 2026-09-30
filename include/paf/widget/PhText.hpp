#pragma once

#include <paf/widget/PhWidget.hpp>

namespace paf {
    // made enums classes so I can use same names without introducing ugly prefixes, also this shit (FontStyles) is cursed, I don't understand lol
    // if you have more braincells (2) than me please fix this
    struct PhFontStyle {
        enum ItalicStyles : int16_t {
            ITALIC_BOLD = 1,
            ITALIC_THIN = 2,
            ITALIC_THIN_ITALIC = 3 // italic italic italic lol
        };

        // unk_2
        enum BoldStyles : int16_t {
            BOLD_BOLD = 1,
            BOLD_BOLD_ITALIC = 2,
            BOLD_THIN_ITALIC = 3,
        };

        ItalicStyles unk_0;
        BoldStyles unk_2;
        int16_t unk_4;
    };

    enum PhTextAttrType {

    };

    class PhText : public PhWidget {
    public:
        PhText(paf::PhWidget*, paf::PhAppear*);

        void SetSize(float, float, float);
        void SetFontStyle(const paf::PhFontStyle&);
        void AddAttr(paf::PhTextAttrType, unsigned int, unsigned int);

        virtual void unk_0xC8();

        int dword330;
        char byte334;
        char byte335;
        char fill[12];
    };
}; // namespace paf