#pragma once

#include <imports_defs.hpp>

// maybe instead of writing this pseudo crap, I should pull wstring class from some ass old GCC
namespace paf {
    // I decided not to resolve the paf's nullstr (as that would require some hooking and shit) and just use my own (?) I hope that no code is "taking ownership" and free/change the string
    // extern unsigned short var_scePaf_E9FDE3C4[];

    class wstring {
    private:
        static unsigned short* GetEmptyString() {
            static unsigned short empty_str[1] = {0};
            return empty_str;
        }

    public:
        unsigned short* buffer; // 0x0
        int length;             // 0x4

        wstring() {
            this->buffer = GetEmptyString();
            this->length = 0;
        }

        wstring(const char16_t* text) {
            this->buffer = nullptr;
            this->assign(text);
        }

        wstring(const wstring& other) {
            this->buffer = GetEmptyString();
            this->length = 0;
            this->assign(other.buffer, other.length);
        }

        ~wstring() {
            if (this->buffer != GetEmptyString()) {
                sce_paf_private_free2(this->buffer);
            }
        }

        wstring& assign(const unsigned short* str, int len) {
            if (this->buffer != GetEmptyString()) {
                sce_paf_private_free2(this->buffer);
            }

            if (str != nullptr && *str != 0) {
                unsigned short* new_buf = (unsigned short*)sce_paf_private_malloc2(2 * (len + 1));

                this->buffer = new_buf;
                this->length = len;

                paf_memcpy(new_buf, str, 2 * len);

                new_buf[len] = 0;
            }

            else {
                this->buffer = GetEmptyString();
                this->length = 0;
            }
            return *this;
        }

        wstring& assign(const unsigned short* str) {
            int len = 0;
            if (str != nullptr) {
                len = sce_paf_private_wcslen(str);
            }
            return this->assign(str, len);
        }

        wstring& assign(const char16_t* text) {
            return this->assign((const unsigned short*)text);
        }

        wstring& operator=(const unsigned short* str) {
            return this->assign(str);
        }

        wstring& operator=(const wstring& other) {
            if (this != &other) {
                this->assign(other.buffer, other.length);
            }
            return *this;
        }

        wstring& append(const unsigned short* str, int append_len) {
            if (append_len <= 0 || str == nullptr)
                return *this;

            int current_len = this->length;
            int new_len = current_len + append_len;

            unsigned short* new_buf = (unsigned short*)sce_paf_private_malloc2(2 * (new_len + 1));

            if (current_len > 0) {
                paf_memcpy(new_buf, this->buffer, 2 * current_len);
            }

            paf_memcpy(&new_buf[current_len], str, 2 * append_len);

            new_buf[new_len] = 0;

            if (this->buffer != GetEmptyString()) {
                sce_paf_private_free2(this->buffer);
            }

            this->buffer = new_buf;
            this->length = new_len;

            return *this;
        }

        wstring& append(const unsigned short* str) {
            if (str == nullptr)
                return *this;
            return this->append(str, sce_paf_private_wcslen(str));
        }

        wstring& operator+=(const unsigned short* str) {
            return this->append(str);
        }

        wstring& operator+=(const wstring& other) {
            return this->append(other.buffer, other.length);
        }

        // Thanks Gemini :((
        static wstring to_wstring(int value) {
            wstring result;
            unsigned short buf[16]; // 32-bit INT_MIN takes 11 chars + 1 for null terminator
            int i = 0;

            // Handle 0 explicitly
            if (value == 0) {
                buf[0] = '0';
                buf[1] = 0;
                result.assign(buf, 1);
                return result;
            }

            bool is_negative = (value < 0);

            // Safely get absolute value (avoids Undefined Behavior with INT_MIN)
            unsigned int uvalue = is_negative ? (~((unsigned int)value) + 1) : (unsigned int)value;

            // Extract digits backwards
            while (uvalue > 0) {
                buf[i++] = '0' + (uvalue % 10);
                uvalue /= 10;
            }

            // Add minus sign if negative
            if (is_negative) {
                buf[i++] = '-';
            }

            buf[i] = 0;

            // Reverse the characters in the buffer
            int start = 0;
            int end = i - 1;
            while (start < end) {
                unsigned short temp = buf[start];
                buf[start] = buf[end];
                buf[end] = temp;
                start++;
                end--;
            }

            // Use the existing assign() method to allocate and copy
            result.assign(buf, i);
            return result;
        }
    };

    inline wstring operator+(const wstring& lhs, const wstring& rhs) {
        wstring result(lhs);
        result += rhs;
        return result;
    }

    inline wstring operator+(const char16_t* lhs, const wstring& rhs) {
        wstring result(lhs);
        result += rhs;
        return result;
    }

    inline wstring operator+(const wstring& lhs, const char16_t* rhs) {
        wstring result(lhs);
        // Cast char16_t* to unsigned short* to match your append / += methods
        result += (const unsigned short*)rhs;
        return result;
    }
}