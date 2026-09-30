#pragma once

#include <paf/std/string.hpp>
#include <paf/std/vector.hpp>

namespace paf {
    class PhEvent;
    class PhWidget;

    class PhHandler {
    public:
        void KillTimerCB(int);
        void SetCallBack(int, void (*)(paf::PhWidget*, paf::PhEvent*, void*), void*);

        // could be I forgot some const so yeah
        virtual bool IsInherit(const char*) const;
        virtual void unk_0x04();
        virtual ~PhHandler();
        virtual void DestroyWidget();
        virtual void EventExec(paf::PhEvent*);
        virtual void HandlerProc(paf::PhEvent*);
        virtual void HandleKeycodeEvent(paf::PhEvent*);
        virtual void HandleAnalogEvent(paf::PhEvent*);
        virtual void HandlePointEvent(paf::PhEvent*);
        virtual void HandleFocusEvent(paf::PhEvent*);
        virtual void HandleFocusSwitchEvent(paf::PhEvent*);
        virtual void HandleFocusOutEvent(paf::PhEvent*);
        virtual void HandleFocusInEvent(paf::PhEvent*);
        virtual void HandleStateEvent(paf::PhEvent*);
        virtual void HandleCommandEvent(paf::PhEvent*);
        virtual void HandleSystemEvent(paf::PhEvent*);

        paf::string m_name;
        int dword10;
        int dword14;
        char byte18;
        char byte19;
        char gap1A[310];
        paf::vector<void*> cb;
        paf::vector<void*> cb_1;
        paf::vector<void*> cb_2;
        paf::vector<void*> cb_3;
        paf::vector<void*> cb_4;
        int dword1A0;
        char gap1A4[8];
        int dword1AC;
    };
}