#pragma once

#include "paf/geom.hpp"
#include "paf/std/vector.hpp"
#include "paf/std/wstring.hpp"
#include "paf/widget/PhDispatcher.hpp"
#include <paf/Surface.hpp>

class vec4;

enum Option : int32_t {
    Option_None = 0,
    Option_UseInterface = 1,
    Option_LoadTypeCdlg = 2
};

namespace paf {
    class PhAppear;
    class PhEvent;
    class Surface;

    class PhWidget : public PhDispatcher {
    public:
        // made up enum, could be they used #defines hence why they put int as arg (cracked NIDs)
        // made up names in enum by me
        enum TextAlign : int {
            TextAlign_Center,
            TextAlign_Left,
            TextAlign_Right
        };

        enum WidgetPos : int {
            WidgetPos_Center,
            WidgetPos_Left,
            WidgetPos_Right
        };

        // these are a little bit guessed as I have no way of testing this (when I was doing it I was just playing with text widget)
        enum WidgetSize : int {
            WidgetSize_Manual = 0x00,
            WidgetSize_TextureWidth = 0x10,
            WidgetSize_TextureHeight = 0x20,
            WidgetSize_TextureSize = 0x30,
        };

        enum Style : int {
            Style_Widget_Pos = 14,         // take a look at `enum WidgetPos`
            Style_Widget_Size = 15,        // take a look at `enum WidgetSize`
            Style_Widget_Rot = 24,         // rotation, untested, every 90 degrees (0,1,2,3)
            Style_Text_FontSize = 28,      // float
            Style_Text_LetterSpacing = 29, // float
            Style_Text_LineSpacing = 33,   // float
            Style_Text_Align = 37,         // take a look at `enum TextAlign`
            Style_Text_ColorUp = 38,       // vec4 RGBA, upper part of color text for gradient
            Style_Text_ColorDown = 39,     // vec4 RGBA, lower part of color text for gradient
        };

        int GetString(const paf::wstring&, int) const;
        void SetPos_ontimer(const vec4&, paf::PhWidget*);
        void UpdateCameras();

        virtual void RemoveChild(int);
        virtual void RemoveChild(const paf::PhWidget*);
        virtual void RegistChild(paf::PhWidget*);
        virtual void unk_0x54(const vec4&);
        virtual void SetMetaAlpha_ontimer(float, bool);
        virtual void SetSize_ontimer(const vec4&);
        virtual void UpdateState();
        virtual void DrawThis();
        virtual void unk_0x68();
        virtual void SetStyle(int, bool);
        virtual void SetStyle(int, int);
        virtual void SetStyle(int, float);
        virtual void SetStyle(int, const vec4&);
        virtual void SetStyle(int, int, bool);
        virtual void SetStyle(int, int, int);
        virtual void SetStyle(int, int, float);
        virtual void SetStyle(int, int, const vec4&);
        virtual void GetStyle(int, bool&);
        virtual void GetStyle(int, int&);
        virtual void GetStyle(int, float&);
        virtual void GetStyle(int, vec4&);
        virtual void GetStyle(int, int, bool&);
        virtual void GetStyle(int, int, int&);
        virtual void GetStyle(int, int, float&);
        virtual void GetStyle(int, int, vec4&);
        virtual void SetTexture(const paf::SurfaceRCPtr<paf::Surface>&, int);
        virtual void GetTexture(paf::SurfaceRCPtr<paf::Surface>&, int);
        virtual void SetText(const paf::wstring&, int); // guessed name, it's not exported so no way to even try to match it
        virtual void unk_0xB8();                        // GetText probably
        virtual void SetFocus(paf::PhEvent*, bool);
        virtual void ReleaseFocus(bool);
        virtual void SetDispatcher();
        virtual void SetAnim(int, float, float, int, const vec4&);

        paf::PhWidget* m_parent;
        int dword1D0;
        int dword1D4;
        paf::vector<paf::PhWidget*> m_childs; // ?
        paf::PhAppear* m_appearance;
        int dword1EC;
        int dword1F0;
        int dword1F4;
        int dword1F8;
        char byte1FC;
        char byte1FD;
        char byte1FE;
        int dword200;
        int dword204;
        float m_metaAlpha;
        float float210;
        float float214;
        float float218;
        float float21C;
        int dword220;
        int dword224;
        int dword228;
        float float22C;
        vec4 m_scale;
        vec4 m_rot;
        int dword250;
        int dword254;
        int dword258;
        // these int, int, int, float, float, int, float is probably some struct pasted over and over
        float float25C;
        float float260;
        int dword264;
        int dword268;
        int dword26C;
        int dword270;
        float float274;
        int dword278;
        int dword27C;
        int dword280;
        int dword284;
        float float288;
        int dword28C;
        int dword290;
        int dword294;
        int dword298;
        float float29C;
        float float2A0;
        int dword2A4;
        int dword2A8;
        int dword2AC;
        int dword2B0;
        float float2B4;
        int dword2B8;
        int dword2BC;
        int dword2C0;
        int dword2C4;
        float float2C8;
        int dword2CC;
        int dword2D0;
        int dword2D4;
        int dword2D8;
        float float2DC;
        float float2E0;
        int dword2E4;
        int dword2E8;
        int dword2EC;
        int dword2F0;
        float float2F4;
        int dword2F8;
        int dword2FC;
        int dword300;
        int dword304;
        float float308;
        int dword30C;
        int dword310;
        int dword314;
        int dword318;
        float float31C;
        vec4 m_size;
    };
}
