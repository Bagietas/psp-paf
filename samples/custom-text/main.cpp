#include "cfwmacros.h"
#include "paf/Surface.hpp"
#include "paf/std/vector.hpp"
#include "paf/widget/PhHandler.hpp"
#include "pspiofilemgr.h"
#include "pspkdebug.h"
#include "pspkerneltypes.h"
#include "pspmoduleinfo.h"
#include "psprtc.h"
#include "pspthreadman.h"
#include <cstdint>
#include <imports_defs.hpp>
#include <pspkernel.h>

#include <paf/View.hpp>
#include <paf/std/string.hpp>
#include <paf/std/wstring.hpp>
#include <paf/widget/PhText.hpp>
#include <paf/widget/PhWidget.hpp>
#include <paf/widget/PhXmBar.hpp>
#include <paf/widget/PhXmList.hpp>

#define DEFINE_HOOK(ret_type, name, ...)         \
    static ret_type (*orig_##name)(__VA_ARGS__); \
    static ret_type hook_##name(__VA_ARGS__)

static u64 last_tick = 0;
static u32 tick_res = 0;
static int frame_count = 0;
static float current_fps = 0.0f;

struct vsh_ctx {
    char fill_0[2664];
    paf::View* m_main_view; // system_plugin
    paf::PhXmBar* m_xmb_bar;
    char fill_1[1044];
    paf::View* m_vsh_view;
};

vsh_ctx* g_vsh_ctx;

PSP_MODULE_INFO(psplugin, PSP_MODULE_USER | PSP_MODULE_NO_STOP, 1, 0);

void* operator new(size_t size) {
    return sce_paf_private_malloc(size);
}

void* operator new[](size_t size) {
    return sce_paf_private_malloc(size);
}

void operator delete(void* ptr) noexcept {
    sce_paf_private_free(ptr);
}

void operator delete[](void* ptr) noexcept {
    sce_paf_private_free(ptr);
}

void operator delete(void* ptr, size_t) noexcept {
    sce_paf_private_free(ptr);
}

void operator delete[](void* ptr, size_t) noexcept {
    sce_paf_private_free(ptr);
}

paf::PhText* debugInfoText;

DEFINE_HOOK(void*, sceGuSwapBuffers, uint32_t a1) {
    void* ret = orig_sceGuSwapBuffers(a1);

    // if debugInfoText was created then vsh_ctx exists for sure but yk
    if (!debugInfoText || !g_vsh_ctx)
        return ret;

    u64 now = 0;
    if (sceRtcGetCurrentTick(&now) < 0)
        return ret;

    if (tick_res == 0)
        tick_res = sceRtcGetTickResolution();

    if (last_tick == 0) {
        last_tick = now;
        frame_count = 0;
        return ret;
    }

    frame_count++;

    u64 elapsed = now - last_tick;

    if (elapsed >= tick_res) {
        current_fps = (float)frame_count * (float)tick_res / (float)elapsed;
        paf::wstring text =
            u"FPS:" + paf::wstring::to_wstring(current_fps) + u"\n";
        text += u"Current Item: " + paf::wstring::to_wstring(g_vsh_ctx->m_xmb_bar->m_sub_lists[g_vsh_ctx->m_xmb_bar->m_focused_cat_id]->m_focused_item_id) + u" (" + paf::wstring::to_wstring(g_vsh_ctx->m_xmb_bar->m_focused_cat_id) + u")";

        debugInfoText->SetText(text, 0);

        frame_count = 0;
        last_tick = now;
    }

    return ret;
}

DEFINE_HOOK(void, sub_20654, vsh_ctx* ctx) {
    if (!g_vsh_ctx) g_vsh_ctx = ctx;

    orig_sub_20654(ctx);

    // the Q is some widget centered to center of the screen, maybe cursor
    debugInfoText = new paf::PhText(ctx->m_vsh_view->FindWidget("Q"), nullptr);
    debugInfoText->SetSize(0.0f, 0.0f, 0.0f);
    debugInfoText->SetStyle(paf::PhWidget::Style_Text_Align, paf::PhWidget::TextAlign_Right);
    debugInfoText->SetStyle(paf::PhWidget::Style_Widget_Pos, paf::PhWidget::WidgetPos_Right);
    debugInfoText->SetStyle(paf::PhWidget::Style_text_LineSpacing, 1.5f);
    debugInfoText->SetStyle(paf::PhWidget::Style_Text_FontSize, 7.605f);
    debugInfoText->SetStyle(paf::PhWidget::Style_Widget_Size, paf::PhWidget::WidgetSize_TextureSize);
    debugInfoText->SetPos_ontimer({240, 0, 0, 0}, nullptr);

    paf::PhText* text_2 = new paf::PhText(ctx->m_vsh_view->FindWidget("Q"), nullptr);
    text_2->SetSize(0.0f, 0.0f, 0.0f);

    // reading the PhAppear and PhSText (guessed name), is just hell but fiugred out that you can set the gradient
    // color on text, couldn't figure out if there's just one type to set the color lol
    text_2->SetStyle(paf::PhWidget::Style_Text_ColorUp, {1.0f, 0.0f, 0.0f, 1.0f});
    text_2->SetStyle(paf::PhWidget::Style_Text_ColorDown, {1.0f, 0.0f, 0.0f, 1.0f});

    text_2->SetStyle(paf::PhWidget::Style_Text_Align, paf::PhWidget::TextAlign_Left);
    text_2->SetStyle(paf::PhWidget::Style_Widget_Pos, paf::PhWidget::WidgetPos_Left);
    text_2->SetStyle(paf::PhWidget::Style_text_LineSpacing, 2.5f);
    text_2->SetStyle(paf::PhWidget::Style_Text_LetterSpacing, 2.5f);
    text_2->SetStyle(paf::PhWidget::Style_Text_FontSize, 7.605f);
    text_2->SetStyle(paf::PhWidget::Style_Widget_Size, paf::PhWidget::WidgetSize_TextureSize);
    text_2->SetPos_ontimer({-240, -30, 0, 0}, nullptr);
    text_2->SetText(u"that's a coool text\npadded to left, with red color\nand crazy spacings", 0);
}

static int (*g_previous)(SceModule*) = nullptr;

// doing this since paf is not yet loaded
int my_strcmp(const char* s1, const char* s2) {
    if (s1 == nullptr || s2 == nullptr) {
        if (s1 == s2)
            return 0;

        return s1 ? 1 : -1;
    }

    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;

    while (*p1 == *p2) {
        if (*p1 == '\0')
            return 0;

        ++p1;
        ++p2;
    }

    return *p1 - *p2;
}

int OnModuleStart(SceModule* mod) {
    if (my_strcmp(mod->modname, "vsh_module") == 0) {
        HIJACK_FUNCTION(mod->text_addr + 0x20654, hook_sub_20654, orig_sub_20654)

        sceKernelDcacheWritebackAll();
        sceKernelIcacheInvalidateAll();
    } else if (my_strcmp(mod->modname, "scePaf_Module") == 0) {
        HIJACK_FUNCTION(mod->text_addr + 0x142658, hook_sceGuSwapBuffers, orig_sceGuSwapBuffers)

        sceKernelDcacheWritebackAll();
        sceKernelIcacheInvalidateAll();
    }

    if (g_previous) {
        g_previous(mod);
    }

    return 0;
}

extern "C" int module_start(SceSize args, void* argp) {
    g_previous = (int (*)(SceModule*))sctrlHENSetStartModuleHandler((void*)OnModuleStart);
    return 0;
}

extern "C" int module_stop(SceSize args, void* argp) {
    return 0;
}