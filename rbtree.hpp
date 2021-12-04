/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:30:05 by tayamamo          #+#    #+#             */
/*   Updated: 2021/12/04 10:56:56 by tayamamo         ###   ########.fr       */
/* ************************************************************************** */

#ifndef RBTREE_HPP_
#define RBTREE_HPP_

#include <cstdlib>
#include <iostream>

namespace ft {

#define NIL NULL

enum Color { kBLACK, kRED };

template <typename T>
struct rbtNode {
    rbtNode*   parent;
    rbtNode*   left;
    rbtNode*   right;
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
    rbtNode<T>* m_root_;

    rbtNode<T>* createNewNode(T key);
    void        deleteAllNode();
    void        deleteAllNodeHelper(rbtNode<T>* node);
    void        inorderHelper(rbtNode<T>* node) const;
    void        preorderHelper(rbtNode<T>* node) const;
    void        postorderHelper(rbtNode<T>* node) const;
    void        rotateLeft(rbtNode<T>* node);
    void        rotateRight(rbtNode<T>* node);
    void        fixInsert(rbtNode<T>* newNode);
    void        fixDelete(rbtNode<T>* node);
    enum Color  getColor(rbtNode<T>* node) const;
    void        setColor(rbtNode<T>* node, enum Color color);
    rbtNode<T>* getParent(rbtNode<T>* node) const;
    rbtNode<T>* getGrandParent(rbtNode<T>* node) const;
    rbtNode<T>* getRoot() const;
    void        setRoot(rbtNode<T>* node);

 public:
    void inorder() const;
    void preorder() const;
    void postorder() const;
    void insertKey(T key);
    void deleteKey(T key);
};

template <typename T>
rbtree<T>::rbtree() : m_root_(NIL) {}

template <typename T>
rbtree<T>::~rbtree() {
    deleteAllNode();
}

template <typename T>
rbtree<T>::rbtree(rbtree const& src) : m_root_(new rbtree) {
    m_root_->parent = src->parent;
    m_root_->left = src->left;
    m_root_->right = src->right;
    m_root_->color = src->color;
    m_root_->key = src->key;
}

template <typename T>
rbtree<T>& rbtree<T>::operator=(rbtree<T> const& rhs) {
    if (this != &rhs) {
        m_root_->parent = rhs->parent;
        m_root_->left = rhs->left;
        m_root_->right = rhs->right;
        m_root_->color = rhs->color;
        m_root_->key = rhs->key;
    }
    return *this;
}

template <typename T>
void rbtree<T>::inorder() const {
    inorderHelper(getRoot());
    std::cout << std::endl;
}

template <typename T>
void rbtree<T>::inorderHelper(rbtNode<T>* node) const {
    if (node == NIL) {
        return;
    }
    inorderHelper(node->left);
    std::cout << node->key << " ";
    inorderHelper(node->right);
}

template <typename T>
void rbtree<T>::preorder() const {
    preorderHelper(getRoot());
    std::cout << std::endl;
}

template <typename T>
void rbtree<T>::preorderHelper(rbtNode<T>* node) const {
    if (node == NIL) {
        return;
    }
    std::cout << node->key << " ";
    preorderHelper(node->left);
    preorderHelper(node->right);
}

template <typename T>
void rbtree<T>::postorder() const {
    postorderHelper(getRoot());
    std::cout << std::endl;
}

template <typename T>
void rbtree<T>::postorderHelper(rbtNode<T>* node) const {
    if (node == NIL) {
        return;
    }
    postorderHelper(node->left);
    postorderHelper(node->right);
    std::cout << node->key << " ";
}

template <typename T>
rbtNode<T>* rbtree<T>::createNewNode(T key) {
    rbtNode<T>* newNode = new rbtNode<T>;
    newNode->parent = NIL;
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->color = kRED;
    newNode->key = key;
    return newNode;
}

template <typename T>
void rbtree<T>::deleteAllNode() {
    deleteAllNodeHelper(getRoot());
}

template <typename T>
void rbtree<T>::deleteAllNodeHelper(rbtNode<T>* node) {
    if (node == NIL) {
        return;
    }
    deleteAllNodeHelper(node->left);
    deleteAllNodeHelper(node->right);
    delete node;
}

template <typename T>
void rbtree<T>::insertKey(T key) {
    rbtNode<T>* newNode = createNewNode(key);

    if (getRoot() == NIL) {
        newNode->color = kBLACK;
        setRoot(newNode);
        return;
    }

    rbtNode<T>* leaf = NIL;
    rbtNode<T>* root = getRoot();

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

    fixInsert(newNode);
}

template <typename T>
void rbtree<T>::fixInsert(rbtNode<T>* newNode) {
    rbtNode<T>* parent = NIL;
    rbtNode<T>* aunt = NIL;
    rbtNode<T>* grandParent = NIL;

    while (newNode != getRoot() && getColor(getParent(newNode)) == kRED) {
        parent = getParent(newNode);
        grandParent = getGrandParent(newNode);
        if (parent == grandParent->left) {
            aunt = grandParent->right;
            if (getColor(aunt) == kRED) {
                setColor(aunt, kBLACK);
                setColor(parent, kBLACK);
                setColor(grandParent, kRED);
                newNode = grandParent;
            } else {
                if (newNode == parent->right) {
                    newNode = parent;
                    rotateLeft(newNode);
                }
                setColor(parent, kBLACK);
                setColor(grandParent, kRED);
                rotateRight(grandParent);
            }
        } else if (parent == grandParent->right) {
            aunt = grandParent->left;
            if (getColor(aunt) == kRED) {
                setColor(aunt, kBLACK);
                setColor(parent, kBLACK);
                setColor(grandParent, kRED);
                newNode = grandParent;
            } else {
                if (newNode == parent->left) {
                    newNode = parent;
                    rotateRight(newNode);
                }
                setColor(parent, kBLACK);
                setColor(grandParent, kRED);
                rotateLeft(newNode);
            }
        }
    }

    setColor(getRoot(), kBLACK);
}

template <typename T>
enum Color rbtree<T>::getColor(rbtNode<T>* node) const {
    if (node == NIL) {
        return kBLACK;
    }
    return node->color;
}

template <typename T>
void rbtree<T>::setColor(rbtNode<T>* node, enum Color color) {
    if (node == NIL) {
        return;
    }
    node->color = color;
}

template <typename T>
rbtNode<T>* rbtree<T>::getParent(rbtNode<T>* node) const {
    if (node == NIL) {
        return NIL;
    }
    return node->parent;
}

template <typename T>
rbtNode<T>* rbtree<T>::getGrandParent(rbtNode<T>* node) const {
    if (node == NIL || node->parent == NIL) {
        return NIL;
    }
    return node->parent->parent;
}

template <typename T>
rbtNode<T>* rbtree<T>::getRoot() const {
    return this->m_root_;
}

template <typename T>
void rbtree<T>::setRoot(rbtNode<T>* node) {
    this->m_root_ = node;
}

template <typename T>
void rbtree<T>::rotateLeft(rbtNode<T>* node) {
    if (node == NIL) {
        return;
    }
    rbtNode<T>* right = node->right;
    node->right = right->left;
    if (node->right != NIL) {
        node->right->parent = node;
    }
    right->parent = node->parent;
    if (node->parent == NIL) {
        setRoot(right);
    } else if (node == node->parent->left) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }
    right->left = node;
    node->parent = right;
}

template <typename T>
void rbtree<T>::rotateRight(rbtNode<T>* node) {
    if (node == NIL) {
        return;
    }
    rbtNode<T>* left = node->left;
    node->left = left->right;
    if (node->left != NIL) {
        node->left->parent = node;
    }
    left->parent = node->parent;
    if (node->parent == NIL) {
        setRoot(left);
    } else if (node == node->parent->left) {
        node->parent->left = left;
    } else {
        node->parent->right = left;
    }
    left->right = node;
    node->parent = left;
}

}  // namespace ft

#endif  // RBTREE_HPP_
