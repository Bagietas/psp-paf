#pragma once

#include <imports_defs.hpp>

// this is also Geminied
namespace paf {

    class string {
    private:
        // I should make this being resolved dynamically to paf's empty_str instance
        // to be safe if string is going to get freed/changed to avoid mem leaks
        static inline char empty_str[1] = {0};

    public:
        char* buffer;   // 0x0
        int length;     // 0x4
        int unk_unused; // 0x8, I'm not even sure this exists as it doesn't seem to be used but View::InitParam would imply that this has to be here

        string() {
            this->buffer = empty_str;
            this->length = 0;
        }

        string(const char* text) {
            this->buffer = nullptr;
            this->assign(text);
        }

        string(const string& other) {
            this->buffer = empty_str;
            this->length = 0;
            this->assign(other.buffer, other.length);
        }

        ~string() {
            if (this->buffer != empty_str) {
                sce_paf_private_free2(this->buffer);
            }
        }

        const char* c_str() {
            return buffer;
        }

        string& assign(const char* str, int len) {
            if (this->buffer != empty_str) {
                sce_paf_private_free2(this->buffer);
            }

            if (str != nullptr && *str != 0) {
                char* new_buf = (char*)sce_paf_private_malloc2(len + 1);

                this->buffer = new_buf;
                this->length = len;

                paf_memcpy(new_buf, str, len); // on 3.40 paf uses it's own private, but on later it uses sceKernelMemcpy for whatever reason

                new_buf[len] = 0;
            } else {
                this->buffer = empty_str;
                this->length = 0;
            }
            return *this;
        }

        string& assign(const char* str) {
            int len = 0;
            if (str != nullptr) {
                len = sce_paf_private_strlen(str);
            }
            return this->assign(str, len);
        }

        string& operator=(const char* str) {
            return this->assign(str);
        }

        string& operator=(const string& other) {
            if (this != &other) {
                this->assign(other.buffer, other.length);
            }
            return *this;
        }

        string& append(const char* str, int append_len) {
            if (append_len <= 0 || str == nullptr)
                return *this;

            int current_len = this->length;
            int new_len = current_len + append_len;

            char* new_buf = (char*)sce_paf_private_malloc2(new_len + 1);

            if (current_len > 0) {
                paf_memcpy(new_buf, this->buffer, current_len);
            }

            paf_memcpy(&new_buf[current_len], str, append_len);

            new_buf[new_len] = 0;

            if (this->buffer != empty_str) {
                sce_paf_private_free2(this->buffer);
            }

            this->buffer = new_buf;
            this->length = new_len;

            return *this;
        }

        string& append(const char* str) {
            if (str == nullptr)
                return *this;
            return this->append(str, sce_paf_private_strlen(str));
        }

        string& operator+=(const char* str) {
            return this->append(str);
        }

        string& operator+=(const string& other) {
            return this->append(other.buffer, other.length);
        }
    };
}