/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_root_may_be_red.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 05:53:33 by tayamamo          #+#    #+#             */
/*   Updated: 2021/12/28 06:02:03 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RBTREE_ROOT_MAY_BE_RED_HPP_
#define RBTREE_ROOT_MAY_BE_RED_HPP_

#include <cassert>
#include <cstdlib>
#include <iostream>

template <typename Key>
class RBTREE {
    template <class U>
    inline void swap(U& a, U& b);

    template <class U>
    inline void SwapPointer(U** a, U** b);

    enum color_t { BLACK, RED };
#define NIL         NULL
#define LEFT        0
#define RIGHT       1
#define left        child[LEFT]
#define right       child[RIGHT]
#define childDir(N) ((N) == ((N)->parent)->right ? RIGHT : LEFT)

    struct RBnode {
        RBnode*      parent;
        RBnode*      child[2];
        enum color_t color;
        Key          key;

        RBnode(Key k) : parent(NULL), color(RED), key(k) {
            left = NIL;
            right = NIL;
        }
        void isNIL() {
            RBnode* node = this;
            std::cout << (node == NIL ? " is NIL. " : ": ") << node
                      << std::endl;
        }
        RBnode* GetMinNode() {
            RBnode* node = this;
            if (node == NIL) return NIL;
            while (node->left != NIL) node = node->left;
            return node;
        }
        RBnode* GetMaxNode() {
            RBnode* node = this;
            if (node == NIL) return node;
            while (node->right != NIL) node = node->right;
            return node;
        }
        RBnode* FindNode(Key k) {
            RBnode* node = this;
            if (node == NIL) return NIL;
            while (node->key != k) {
                if (k < node->key) {
                    if (node->left == NIL) break;
                    node = node->left;
                } else {
                    if (node->right == NIL) break;
                    node = node->right;
                }
            }
            return node;
        }
    };

    inline void SwapNodePointer(RBnode** a, RBnode** b) {
        SwapPointer(a, b);
        swap((*a)->key, (*b)->key);
    }

    RBnode* m_root_;

    RBnode* RotateDirRoot(RBnode* P, unsigned char dir) {
        RBnode* G = P->parent;
        RBnode* S = P->child[1 - dir];
        RBnode* C;
        assert(S != NIL);
        C = S->child[dir];
        P->child[1 - dir] = C;
        if (C != NIL) C->parent = P;
        S->child[dir] = P;
        P->parent = S;
        S->parent = G;
        if (G != NULL)
            G->child[P == G->right ? RIGHT : LEFT] = S;
        else
            m_root_ = S;
        return S;
    }

#define RotateDir(N, dir) RotateDirRoot(N, dir)
#define RotateLeft(N)     RotateDirRoot(N, LEFT)
#define RotateRight(N)    RotateDirRoot(N, RIGHT)

 public:
    bool RBinsert(Key k) {
        if (m_root_ == NIL) {
            // std::cout << "Case_I0" << std::endl;
            RBnode* N = new RBnode(k);
            m_root_ = N;
            N->color = BLACK;
            return true;
        }
        RBnode* P = m_root_->FindNode(k);
        if (P->key == k) return false;
        unsigned char dir = k < P->key ? LEFT : RIGHT;
        RBnode*       N = new RBnode(k);
        P->child[dir] = N;
        N->parent = P;
        struct RBnode* G;
        struct RBnode* U;
        do {
            if (P->color == BLACK) {
                // std::cout << "Case_I1" << std::endl;
                return true;
            }
            if ((G = P->parent) == NULL) goto Case_I4;
            dir = childDir(P);
            U = G->child[1 - dir];
            if (U == NIL || U->color == BLACK) goto Case_I56;
            P->color = BLACK;
            U->color = BLACK;
            G->color = RED;
            N = G;
        } while ((P = N->parent) != NULL);
        // std::cout << "Case_I3" << std::endl;
        return true;
    Case_I4:
        // std::cout << "Case_I4" << std::endl;
        P->color = BLACK;
        return true;
    Case_I56:
        if (N == P->child[1 - dir]) {
            // std::cout << "Case_I5" << std::endl;
            RotateDir(P, dir);
            N = P;
            P = G->child[dir];
        }
        // std::cout << "Case_I6" << std::endl;
        RotateDir(G, 1 - dir);
        P->color = BLACK;
        G->color = RED;
        return true;
    }

 public:
    void RBdelete(Key k) {
        struct RBnode* N = m_root_->FindNode(k);
        if (N == NIL || N->key != k) {
            return;
        }
        if (N == m_root_ && N->left == NIL && N->right == NIL) {
            delete N;
            m_root_ = NIL;
            return;
        }
        if (N->left != NIL && N->right != NIL) {
            RBnode* prev = N->left->GetMaxNode();
            SwapNodePointer(&N, &prev);
        }
        struct RBnode* P = N->parent;
        unsigned char  dir = childDir(N);
        if (N->color == RED) {
            if (N->left != NIL) {
                P->child[dir] = N->left;
                N->left->parent = P;
            } else if (N->right != NIL) {
                P->child[dir] = N->right;
                N->right->parent = P;
            } else {
                P->child[dir] = NIL;
            }
            delete N;
            return;
        }
        if (N->color == BLACK) {
            if (N->left != NIL && N->right == NIL && N->left->color == RED) {
                N->left->color = BLACK;
                SwapPointer(&N, &N->left);
                P->child[dir] = N;
                N->parent = P;
                delete N->left;
                N->left = NIL;
                return;
            }
            if (N->left == NIL && N->right != NIL && N->right->color == RED) {
                N->right->color = BLACK;
                SwapPointer(&N, &N->right);
                P->child[dir] = N;
                N->parent = P;
                delete N->right;
                N->right = NIL;
                return;
            }
        }
        P->child[dir] = NIL;
        struct RBnode* S;
        struct RBnode* C;
        struct RBnode* D;
        goto Start_D;
        do {
            dir = childDir(N);
        Start_D:
            S = P->child[1 - dir];
            D = S->child[1 - dir];
            C = S->child[dir];
            if (S->color == RED) {
                // std::cout << "<< Case_D3 >>" << std::endl;
                goto Case_D3;
            }
            if (D != NIL && D->color == RED) {
                // std::cout << "<< Case_D6 >>" << std::endl;
                goto Case_D6;
            }
            if (C != NIL && C->color == RED) {
                // std::cout << "<< Case_D5 >> " << std::endl;
                goto Case_D5;
            }
            if (P->color == RED) {
                // std::cout << "<< Case_D4 >> " << std::endl;
                goto Case_D4;
            }
            // std::cout << "<< Case_D1 >> " << std::endl;
            S->color = RED;
            N = P;
        } while ((P = N->parent) != NULL);
        // std::cout << "<< Case_D2 >> " << std::endl;
        return;
    Case_D3:
        RotateDir(P, dir);
        P->color = RED;
        S->color = BLACK;
        S = C;
        D = S->child[1 - dir];
        if (D != NIL && D->color == RED) goto Case_D6;
        C = S->child[dir];
        if (C != NIL && C->color == RED) goto Case_D5;
    Case_D4:
        S->color = RED;
        P->color = BLACK;
        return;
    Case_D5:
        RotateDir(S, 1 - dir);
        S->color = RED;
        C->color = BLACK;
        D = S;
        S = C;
    Case_D6:
        RotateDir(P, dir);
        S->color = P->color;
        P->color = BLACK;
        D->color = BLACK;
        return;
    }

 public:
    RBTREE() { m_root_ = NIL; }

 public:
    void inorder() const {
        std::cout << "in: ";
        InorderHelper(m_root_);
        std::cout << std::endl;
    }
    void preorder() const {
        std::cout << "pre: ";
        preorderHelper(m_root_);
        std::cout << std::endl;
    }
    void postorder() const;

 private:
    inline void InorderHelper(RBnode* node) const {
        if (node == NIL) return;
        InorderHelper(node->left);
        std::cout << "(" << node->key << ", ";
        std::cout << (node->color ? "R" : "B") << ") ";
        InorderHelper(node->right);
    }

    inline void preorderHelper(RBnode* node) const {
        if (node == NIL) {
            return;
        }
        std::cout << "(" << node->key << ", ";
        std::cout << (node->color ? "R" : "B") << ") ";
        preorderHelper(node->left);
        preorderHelper(node->right);
    }
    void postorderHelper(RBnode* node) const;
};

template <typename Key>
template <class U>
inline void RBTREE<Key>::swap(U& a, U& b) {
    U c = a;
    a = b;
    b = c;
}
template <typename Key>
template <class U>
inline void RBTREE<Key>::SwapPointer(U** a, U** b) {
    U* c = *a;
    *a = *b;
    *b = c;
}

template <typename Key>
void RBTREE<Key>::postorder() const {
    postorderHelper(m_root_);
    std::cout << std::endl;
}

template <typename Key>
void RBTREE<Key>::postorderHelper(RBnode* node) const {
    if (node == NIL) {
        return;
    }
    postorderHelper(node->left);
    postorderHelper(node->right);
    std::cout << node->key << " ";
}

#endif  // RBTREE_ROOT_MAY_BE_RED_HPP_
