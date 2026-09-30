#include <cstdarg>
#include <cstddef>
#include <cstdint>

extern "C" unsigned int sctrlModuleTextAddr(const char* modname);
extern "C" void* sctrlHENSetStartModuleHandler(void*);

extern "C" void kuKernelIcacheInvalidateAll();

extern "C" void Heap_QueryInfo(void* data, void* data1);
extern "C" void paf_memcpy(void* dest, const void* src, size_t n);
extern "C" int sce_paf_private_strcmp(const char* str1, const char* str2);
extern "C" int sce_paf_private_vsnprintf(char* s, size_t n, const char* format, va_list arg);
extern "C" int sce_paf_private_sprintf(char* buffer, const char* format, ...);
extern "C" void* sce_paf_private_malloc(int);
extern "C" void sce_paf_private_free(void* ptr);
extern "C" void* sce_paf_private_malloc2(int size);
extern "C" void sce_paf_private_free2(void* ptr);
extern "C" int sce_paf_private_wcslen(const unsigned short* str);
extern "C" int sce_paf_private_strlen(const char* str);
extern "C" bool PAF_Resource_DOMGetNodeTable(uintptr_t, int, uintptr_t*);
extern "C" bool PAF_Resource_DOMGetNodeFirstChild(uintptr_t, uintptr_t*);