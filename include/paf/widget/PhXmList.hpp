#pragma once

#include <paf/widget/PhWidget.hpp>

// todo: do proper def of this, as iirc this is sub of PhXmList etc.
namespace paf {
    class PhXmList : public PhWidget {
    public:
        // TODO: find better name lol
        enum Type : int {
            Type_Icon = 0,
            Type_Text = 4
        };

        void SetItemNum(int num);

        int dword32C;
        int m_items_count;
        int dword334;
        int dword338;
        int m_focused_item_id;
        float float340;
        int dword344;
        int dword348;
        float float34C;
        float float350;
        int dword354;
        int dword358;
        char byte35C;
        int dword360;
        int dword364;
        char gap368[8];
        float float370;
        float float374;
        float float378;
        float float37C;
        float float380;
        float float384;
        float float388;
        float float38C;
        int dword390;
        int dword394;
        int dword398;
        int dword39C;
        char gap3A0[24];
        char byte3B8;
        char byte3B9;
        int dword3BC;
        int dword3C0;
    };
}; // namespace paf