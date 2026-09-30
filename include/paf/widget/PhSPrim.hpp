#pragma once

// this and appear thing should have better place lol, either moved out of the widget completely or create some sub one as render or some shit
namespace paf {
    class PhWidget;
    class vec4;

    class PhSPrim {
    public:
        virtual ~PhSPrim();
        virtual void Render(const paf::PhWidget*, const vec4&);
        virtual void SetStyle(int, bool);
        virtual void SetStyle(int, int);
        virtual void SetStyle(int, float);
        virtual void SetStyle(int, const vec4&);
        virtual void GetStyle(int, bool&);
        virtual void GetStyle(int, int&);
        virtual void GetStyle(int, float&);
        virtual void GetStyle(int, vec4&);
        virtual void SetupMatrix(const paf::PhWidget*);
        virtual void Sprite(const paf::PhWidget*, int, int, const vec4&);
    };
}