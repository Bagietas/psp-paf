#pragma once

#include "paf/widget/PhXmList.hpp"
#include <paf/widget/PhWidget.hpp>

namespace paf {
    class PhXmBar : public PhWidget {
    public:
        void SetListNum(int num);
        int GetFocusedItemIndex(int cat_id);
        void CloseUp(int, int, bool, bool, float, float);
        void SetFocusIndex(int, int);
        void Redraw(int, int);
        void DeleteItem(int, int, float);
        void UpdateItems(int);

        int dword32C;
        int dword330;
        int m_categories_count;
        int m_focused_cat_id;
        float float33C;
        float float340;
        float float344;
        int gap[6];
        paf::vector<paf::PhXmList*> m_sub_lists;
    };
}; // namespace paf