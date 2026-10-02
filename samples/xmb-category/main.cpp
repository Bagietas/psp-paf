#include "cfwmacros.h"
#include "my_utils.hpp"
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

// #include <paf/widget/PhHandler.hpp>
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

int pspsh_printf(const char* fmt, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, fmt);

    int len = sce_paf_private_vsnprintf(buffer, sizeof(buffer), fmt, args);

    va_end(args);

    if (len < 0)
        return len;

    if (len >= (int)sizeof(buffer))
        len = sizeof(buffer) - 1;

    return sceIoWrite(1, buffer, len);
}

SceUID thid;

// system_plugin_fg -> xmb probably
struct my_data {
    const char16_t* text;
    const char* tex_name;
};

static my_data buttons[4] = {
    {u"Daxter Trophies", ""},
    {u"AP", "AP"},
    {u"AQ", "AQ"},
    {u"BI", "BI"},
    //{u"CA", "system_plugin", "CA"},
};

paf::PhText* debugInfoText;

DEFINE_HOOK(void, OnXmbScrollIn, vsh_ctx* ctx, int cat_id, int item_id, int a4) {
    if (!g_vsh_ctx) g_vsh_ctx = ctx;

    pspsh_printf("[OnXmbScrollIn] 0x%x, %i, 0x%x, 0x%x\n", ctx, cat_id, item_id, a4);

    if (cat_id == 8) {
        pspsh_printf("0x%x, %i, 0x%x, 0x%x\n", ctx, cat_id, item_id, a4);

        int global_item_id = (cat_id << 24) | (item_id << 16);

        ctx->m_xmb_bar->SetText(buttons[item_id].text, global_item_id | 0x4);

        if (item_id == 0) {
            // gim I taken from PS3, I'm making this comment like 2 months after I've done this so I don't even remember if I converted the gim somehow, or if it's stock PS3 gim
            int trophy_gim_fd = sceIoOpen("ms0:/trophy.gim", PSP_O_RDONLY, 0777);
            pspsh_printf("gim_fd: %i\n", trophy_gim_fd);
            if (trophy_gim_fd >= 0) {
                SceOff current = sceIoLseek(trophy_gim_fd, 0, PSP_SEEK_CUR);
                SceOff size = sceIoLseek(trophy_gim_fd, 0, PSP_SEEK_END);
                sceIoLseek(trophy_gim_fd, current, PSP_SEEK_SET);

                void* gim_buf = sce_paf_private_malloc(size);
                sceIoRead(trophy_gim_fd, gim_buf, size);

                ctx->m_xmb_bar->SetTexture(GetTextureFromGim(gim_buf, size, ctx->m_vsh_view->m_surface_pool), global_item_id | 0x0);
                sceIoClose(trophy_gim_fd);
            } else {
                ctx->m_xmb_bar->SetTexture(GetXmbCachedTexture((void*)(sctrlModuleTextAddr("vsh_module") + 0x5692C), buttons[item_id].tex_name), global_item_id | 0x0);
            }
        } else {
            ctx->m_xmb_bar->SetTexture(GetXmbCachedTexture((void*)(sctrlModuleTextAddr("vsh_module") + 0x5692C), buttons[item_id].tex_name), global_item_id | 0x0);
        }
        ctx->m_xmb_bar->m_sub_lists[cat_id]->SetAnim(0x100000E, 1.0f, 1.0f, 4, {1.0f, 0.0f, 0.0f, 1.0f});
        return;
    }

    orig_OnXmbScrollIn(ctx, cat_id, item_id, a4);
}

DEFINE_HOOK(int, sub_211B4, vsh_ctx* ctx) {
    if (!g_vsh_ctx) g_vsh_ctx = ctx;

    if (ctx->m_xmb_bar->m_focused_cat_id == 8)
        return 0;

    return orig_sub_211B4(ctx);
}

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

void addItem(paf::PhXmList* xmlist, const char16_t* text /*, const paf::SurfaceRCPtr<paf::Surface>& tex*/) {
    int new_item_id = xmlist->m_items_count;
    xmlist->SetItemNum(xmlist->m_items_count + 1);

    xmlist->SetText(text, (new_item_id << 16) | 0x28);
    // xmlist->SetTexture(tex, (new_item_id << 16) | paf::PhXmList::Type_Icon);
}

DEFINE_HOOK(void, sub_20654, vsh_ctx* ctx) {
    if (!g_vsh_ctx) g_vsh_ctx = ctx;

    orig_sub_20654(ctx);
    pspsh_printf("[%s:interface(1)]: 0x%x\n", ctx->m_main_view->m_name.c_str(), ctx->m_main_view->GetInterface(1));
    pspsh_printf("dupa cipa: %s, %i\n", ctx->m_vsh_view->m_name.c_str(), ctx->m_vsh_view->m_name.length);

    debugInfoText = new paf::PhText(ctx->m_vsh_view->FindWidget("Q"), nullptr);
    debugInfoText->SetSize(0.0f, 0.0f, 0.0f);
    debugInfoText->SetStyle(paf::PhWidget::Style_Text_Align, paf::PhWidget::TextAlign_Right);
    debugInfoText->SetStyle(paf::PhWidget::Style_Widget_Pos, paf::PhWidget::WidgetPos_Right);
    debugInfoText->SetStyle(paf::PhWidget::Style_Text_LineSpacing, 1.5f);
    debugInfoText->SetStyle(paf::PhWidget::Style_Text_FontSize, 7.605f);
    debugInfoText->SetStyle(paf::PhWidget::Style_Widget_Size, paf::PhWidget::WidgetSize_TextureSize);
    debugInfoText->SetPos_ontimer({240, 0, 0, 0}, nullptr);

    int new_category_index = ctx->m_xmb_bar->m_categories_count;
    pspsh_printf("old count: %i\n", new_category_index);

    ctx->m_xmb_bar->SetListNum(new_category_index + 1);

    // theoretically no need to do this shit here
    int cat_id_shifted = (new_category_index) << 24;
    ctx->m_xmb_bar->SetText(u"★ hax0r stuff", cat_id_shifted | 0x28);

    // again making comment like ages after, but I think it would crash if done here due to the fact that GetTextureFromGim uses paf::Image that relies on some global thing that is not yet initalzied
    // so doing it in the OnXmbScrollIn
    /*int trophy_gim_fd = sceIoOpen("ms0:/trophy.gim", PSP_O_RDONLY, 0777);
    pspsh_printf("gim_fd: %i\n", trophy_gim_fd);
    if (trophy_gim_fd >= 0) {
        SceOff current = sceIoLseek(trophy_gim_fd, 0, PSP_SEEK_CUR);
        SceOff size = sceIoLseek(trophy_gim_fd, 0, PSP_SEEK_END);
        sceIoLseek(trophy_gim_fd, current, PSP_SEEK_SET);

        void* gim_buf = sce_paf_private_malloc(size);
        sceIoRead(trophy_gim_fd, gim_buf, size);

        ctx->m_xmb_bar->SetTexture(GetTextureFromGim(gim_buf, size, ctx->m_vsh_view->m_surface_pool), ((8) << 24) | 0x1A);
        sceIoClose(trophy_gim_fd);
    } else {*/
    ctx->m_xmb_bar->SetTexture(GetXmbCachedTexture((void*)(sctrlModuleTextAddr("vsh_module") + 0x5692C), "AJ"), ((8) << 24) | 0x1A);
    //}

    ctx->m_xmb_bar->m_sub_lists[new_category_index]->SetAnim(0x100000E, 1.0f, 1.0f, 4, {1.0f, 0.0f, 0.0f, 1.0f});

    paf::PhXmList* xmlist = ctx->m_xmb_bar->m_sub_lists[new_category_index];
    for (int i = 0; i < 4; i++) {
        addItem(xmlist, buttons[i].text /*, paf::View::Find(buttons[i].view_name)->GetTexture(buttons[i].tex_name)*/);
    }
}

DEFINE_HOOK(void*, sub_21694, vsh_ctx* ctx, int cat_id, int item_id) {
    if (cat_id == 8) {
        // should be enough lol, later on I should probably
        static char* garbage_item;
        if (!garbage_item) {
            garbage_item = (char*)sce_paf_private_malloc(0x40);
            for (int i = 0; i < 0x40; i++) {
                garbage_item[i] = -1; // hope that this doesn't pass any check :)
            }
        }
        return garbage_item;
    }

    return orig_sub_21694(ctx, cat_id, item_id);
}

// this hook is here probably to feed the vsh_ctx global var, although idk atp
DEFINE_HOOK(void, sub_16504, vsh_ctx* ctx, uint32_t a2) {
    if (!g_vsh_ctx) g_vsh_ctx = ctx;
    orig_sub_16504(ctx, a2);
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
    // pspsh_printf("started: %s\n", mod->modname);
    if (my_strcmp(mod->modname, "vsh_module") == 0) {
        // patch removing psn and network category
        _sw(0, mod->text_addr + 0x20CCC);

        // patch street, to populate psn and network
        _sw(0x24020001, mod->text_addr + 0x20E98);
        _sw(0x24020001, mod->text_addr + 0x20E4C);

        //_sw(0x24020002, mod->text_addr + 0x2068C);

        /*
        // patch region
        _sw(0x24020001, mod->text_addr + 0x207D4);
        _sw(0x24020001, mod->text_addr + 0x20840);*/

        HIJACK_FUNCTION(mod->text_addr + 0x241D8, hook_OnXmbScrollIn, orig_OnXmbScrollIn)
        HIJACK_FUNCTION(mod->text_addr + 0x211B4, hook_sub_211B4, orig_sub_211B4)
        HIJACK_FUNCTION(mod->text_addr + 0x20654, hook_sub_20654, orig_sub_20654)
        HIJACK_FUNCTION(mod->text_addr + 0x16504, hook_sub_16504, orig_sub_16504)
        // HIJACK_FUNCTION(mod->text_addr + 0x169B4, hook_OnXmbPush, orig_OnXmbPush)
        //  HIJACK_FUNCTION(mod->text_addr + 0x16F5C, hook_sub_16F5C, orig_sub_16F5C)
        //  HIJACK_FUNCTION(mod->text_addr + 0x16A70, hook_sub_16A70, orig_sub_16A70)
        HIJACK_FUNCTION(mod->text_addr + 0x21694, hook_sub_21694, orig_sub_21694)
        // HIJACK_FUNCTION(mod->text_addr + 0x1D7A4, hook_sub_1D7A4, orig_sub_1D7A4)
        //  HIJACK_FUNCTION(mod->text_addr + 0x3039C, hook_sub_3039C, orig_sub_3039C)

        sceKernelDcacheWritebackAll();
        sceKernelIcacheInvalidateAll();

        // vsh_started = true;
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