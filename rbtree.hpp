/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:30:05 by tayamamo          #+#    #+#             */
/*   Updated: 2021/12/04 04:11:21 by tayamamo         ###   ########.fr       */
/* ************************************************************************** */

#ifndef RBTREE_HPP_
#define RBTREE_HPP_

#include <cstdlib>
#include <iostream>

namespace ft {

#define NIL NULL

enum Color { kBLACK, kRED };

template <typename T>
struct rbnode {
    rbnode*    parent;
    rbnode*    left;
    rbnode*    right;
    enum Color color;
    T          key;
};

template <typename T>
class rbtree {
 public:
    rbtree();
    ~rbtree();
    rbtree(rbtree const& src);
    rbtree& operator=(rbtree const& rhs);

 private:
    rbnode<T>* root;

    rbnode<T>* createNewNode(T key);
    void       inorderHelper(rbnode<T>* node) const;
    void       preorderHelper(rbnode<T>* node) const;
    void       postorderHelper(rbnode<T>* node) const;
    void       rotateLeft(rbnode<T>* node);
    void       rotateRight(rbnode<T>* node);
    void       fixInsert(rbnode<T>* node);
    void       fixDelete(rbnode<T>* node);

 public:
    void inorder() const;
    void preorder() const;
    void postorder() const;
    void insertKey(T key);
    void deleteKey(T key);
};

template <typename T>
rbtree<T>::rbtree() : root(NIL) {}

template <typename T>
rbtree<T>::~rbtree() {}

template <typename T>
rbtree<T>::rbtree(rbtree const& src) : root(new rbtree) {
    root->parent = src->parent;
    root->left = src->left;
    root->right = src->right;
    root->color = src->color;
    root->key = src->key;
}

template <typename T>
rbtree<T>& rbtree<T>::operator=(rbtree<T> const& rhs) {
    if (this != &rhs) {
        root->parent = rhs->parent;
        root->left = rhs->left;
        root->right = rhs->right;
        root->color = rhs->color;
        root->key = rhs->key;
    }
    return *this;
}

template <typename T>
void rbtree<T>::inorder() const {
    inorderHelper(this->root);
    std::cout << std::endl;
}

template <typename T>
void rbtree<T>::inorderHelper(rbnode<T>* node) const {
    if (node == NIL) {
        return;
    }
    inorderHelper(node->left);
    std::cout << node->key << " ";
    inorderHelper(node->right);
}

template <typename T>
void rbtree<T>::preorder() const {
    preorderHelper(this->root);
    std::cout << std::endl;
}

template <typename T>
void rbtree<T>::preorderHelper(rbnode<T>* node) const {
    if (node == NIL) {
        return;
    }
    std::cout << node->key << " ";
    inorderHelper(node->left);
    inorderHelper(node->right);
}

template <typename T>
void rbtree<T>::postorder() const {
    postorderHelper(this->root);
    std::cout << std::endl;
}

template <typename T>
void rbtree<T>::postorderHelper(rbnode<T>* node) const {
    if (node == NIL) {
        return;
    }
    inorderHelper(node->left);
    inorderHelper(node->right);
    std::cout << node->key << " ";
}

template <typename T>
rbnode<T>* rbtree<T>::createNewNode(T key) {
    rbnode<T>* newNode = new rbnode<T>;
    newNode->parent = NIL;
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->color = kRED;
    newNode->key = key;
    return newNode;
}

template <typename T>
void rbtree<T>::insertKey(T key) {
    rbnode<T>* newNode = createNewNode(key);

    if (this->root == NIL) {
        newNode->color = kBLACK;
        this->root = newNode;
        return;
    }

    rbnode<T>* leaf = NIL;
    rbnode<T>* root = this->root;

    while (root != NIL) {
        leaf = root;
        if (newNode->key > root->key) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    newNode->parent = leaf->parent;

    if (leaf->key > newNode->key) {
        leaf->right = newNode;
    } else {
        leaf->left = newNode;
    }
}

}  // namespace ft

#endif  // RBTREE_HPP_
