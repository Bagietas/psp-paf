#pragma once

#include <imports_defs.hpp>

// unfortunately I used LLM to help me define this shit
namespace paf {
    template <typename Key, typename Value>
    struct rbtree_node {
        rbtree_node* parent;
        rbtree_node* left;
        rbtree_node* right;

        Key key;
        Value value;

        union {
            struct {
                unsigned char color;
                unsigned char is_sentinel;
                unsigned short _pad;
            };
            unsigned short flags_word;
        };
    };

    template <typename Key, typename Value>
    class map {
    public:
        typedef rbtree_node<Key, Value> Node;

    private:
        Node* m_header;
        int m_node_count;

        void insert_and_rebalance(Node* node);

    public:
        map() {
            m_header = (Node*)sce_paf_private_malloc2(sizeof(Node));
            m_header->parent = nullptr;
            m_header->left = m_header;
            m_header->right = m_header;
            m_header->is_sentinel = 1;
            m_node_count = 0;
        }

        Value& operator[](const Key& key) {
            bool was_inserted = false;
            Node* node = insert_or_find(key, &was_inserted);

            return node->value;
        }

        Node* insert_or_find(const Key& search_key, bool* out_inserted) {
            Node* current = m_header->parent;
            Node* parent = m_header;
            int cmp_result = 0;

            while (!current->is_sentinel) {
                parent = current;

                const char* s_str = search_key.m_data;
                const char* node_str = current->key.m_data;

                if (*s_str < *node_str || sce_paf_private_strcmp(s_str, node_str) < 0) {
                    current = current->left;
                    cmp_result = -1;
                } else {
                    bool is_greater = (*node_str < *s_str || sce_paf_private_strcmp(node_str, s_str) < 0);
                    cmp_result = 1;
                    if (!is_greater) {
                        if (out_inserted) *out_inserted = false;
                        return current;
                    }
                    current = current->right;
                }
            }

            Node* new_node = (Node*)sce_paf_private_malloc2(sizeof(Node));

            if (new_node) {
                new_node->parent = parent;
                new_node->left = m_header;
                new_node->right = m_header;

                new_node->key = search_key;

                new_node->value = Value();

                new_node->flags_word = 1;
            }

            if (cmp_result != 0) {
                if (cmp_result < 0) {
                    parent->left = new_node;
                    if (parent == m_header->left) m_header->left = new_node;
                } else {
                    parent->right = new_node;
                    if (parent == m_header->right) m_header->right = new_node;
                }
            } else {
                m_header->parent = new_node;
                m_header->left = new_node;
                m_header->right = new_node;
            }

            m_node_count++;

            insert_and_rebalance(new_node);

            if (out_inserted) *out_inserted = true;
            return new_node;
        }
    };
};