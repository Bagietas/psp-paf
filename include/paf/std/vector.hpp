#pragma once

#include <imports_defs.hpp>
#include <stddef.h>

// this class is completly guessed, and "imaginary"
namespace paf {
    template <class T>
    class vector {
    public:
        T* _data = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;
        void* unk_0xC;

        T& operator[](size_t index) {
            return _data[index];
        }

        const T& operator[](size_t index) const {
            return _data[index];
        }

        size_t size() const {
            return _size;
        }

        size_t capacity() const {
            return _capacity;
        }

        bool empty() const {
            return _size == 0;
        }

        void clear() {
            _size = 0;
        }

        T* begin() { return _data; }
        T* end() { return _data + _size; }

        const T* begin() const { return _data; }
        const T* end() const { return _data + _size; }

        void reallocate(size_t new_cap) {
            if (_data) {
                T* new_data = static_cast<T*>(sce_paf_private_malloc2(sizeof(T) * new_cap));

                if (_size > 0) {
                    for (size_t i = 0; i < _size; ++i) {
                        new_data[i] = _data[i];
                    }
                }

                sce_paf_private_free2(_data);
                _capacity = new_cap;
                _data = new_data;
            } else {
                _data = static_cast<T*>(sce_paf_private_malloc2(sizeof(T) * new_cap));
                _capacity = new_cap;
            }
        }

        void push_back(const T& value) {
            if (_capacity < _size + 1) {
                reallocate(_capacity + 10);
            }

            if (_data) {
                _data[_size] = value;
            }
            _size++;
        }

        T* erase(T* first, T* last) {
            size_t first_idx = first - _data;
            size_t last_idx = last - _data;

            size_t items_to_shift = _size - last_idx;
            size_t items_removed = last_idx - first_idx;

            if (items_to_shift > 0) {
                T* dst = first;
                T* src = last;
                for (size_t i = 0; i < items_to_shift; ++i) {
                    *dst++ = *src++;
                }
            }

            _size -= items_removed;

            if (_size < _capacity - 20) {
                reallocate(_capacity - 10);
            }

            return _data + first_idx;
        }
    };
}
