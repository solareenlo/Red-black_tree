/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:30:05 by tayamamo          #+#    #+#             */
/*   Updated: 2021/12/11 18:20:17 by tayamamo         ###   ########.fr       */
/* ************************************************************************** */

#ifndef RBTREE_HPP_
#define RBTREE_HPP_

#include <cstdlib>
#include <iostream>

namespace ft {

enum Color { BLACK, RED };

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
    rbtNode<T>* NIL;

    rbtNode<T>* createNewNode(T key);
    void        deleteAllNode();
    void        deleteAllNodeHelper(rbtNode<T>* node);
    void        inorderHelper(rbtNode<T>* node) const;
    void        preorderHelper(rbtNode<T>* node) const;
    void        postorderHelper(rbtNode<T>* node) const;
    void        rotateLeft(rbtNode<T>* node);
    void        rotateRight(rbtNode<T>* node);
    void        balanceAfterInsert(rbtNode<T>* newNode);
    void        deleteKeyHelper(rbtNode<T>* node, T key);
    void        balanceAfterDelete(rbtNode<T>* node);
    void        transplantNode(rbtNode<T>* u, rbtNode<T>* v);
    rbtNode<T>* minKeyNode(rbtNode<T>* node);
    rbtNode<T>* maxKeyNode(rbtNode<T>* node);
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
rbtree<T>::rbtree() {
    NIL = new rbtNode<T>;
    NIL->parent = NULL;
    NIL->color = BLACK;
    NIL->left = NULL;
    NIL->right = NULL;
    m_root_ = NIL;
}

template <typename T>
rbtree<T>::~rbtree() {
    deleteAllNode();
    delete NIL;
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
    std::cout << "pre: ";
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
    newNode->parent = NULL;
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->color = RED;
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

    rbtNode<T>* leaf = NULL;
    rbtNode<T>* root = getRoot();
    while (root != NIL) {
        leaf = root;
        if (newNode->key < root->key) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    newNode->parent = leaf;

    if (leaf == NULL) {
        setRoot(newNode);
    } else if (newNode->key < leaf->key) {
        leaf->left = newNode;
    } else {
        leaf->right = newNode;
    }

    if (getParent(newNode) == NULL) {
        setColor(newNode, BLACK);
        return;
    }
    if (getGrandParent(newNode) == NULL) {
        return;
    }

    balanceAfterInsert(newNode);
}

template <typename T>
void rbtree<T>::balanceAfterInsert(rbtNode<T>* newNode) {
    rbtNode<T>* parent = NULL;
    rbtNode<T>* aunt = NULL;
    rbtNode<T>* grandParent = NULL;
    while (newNode != getRoot() && getColor(getParent(newNode)) == RED) {
        parent = getParent(newNode);
        grandParent = getGrandParent(newNode);
        if (parent == grandParent->left) {
            aunt = grandParent->right;
            if (aunt->color == RED) {
                aunt->color = BLACK;
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            } else {
                if (newNode == newNode->parent->right) {
                    newNode = newNode->parent;
                    rotateLeft(newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                rotateRight(newNode->parent->parent);
            }
        } else if (parent == grandParent->right) {
            aunt = newNode->parent->parent->left;
            if (aunt->color == RED) {
                aunt->color = BLACK;
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            } else {
                if (newNode == newNode->parent->left) {
                    newNode = newNode->parent;
                    rotateRight(newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                rotateLeft(newNode->parent->parent);
            }
        }
    }
    setColor(getRoot(), BLACK);
}

template <typename T>
enum Color rbtree<T>::getColor(rbtNode<T>* node) const {
    if (node == NULL) {
        return BLACK;
    }
    return node->color;
}

template <typename T>
void rbtree<T>::setColor(rbtNode<T>* node, enum Color color) {
    if (node == NULL) {
        return;
    }
    node->color = color;
}

template <typename T>
rbtNode<T>* rbtree<T>::getParent(rbtNode<T>* node) const {
    if (node == NULL) {
        return NULL;
    }
    return node->parent;
}

template <typename T>
rbtNode<T>* rbtree<T>::getGrandParent(rbtNode<T>* node) const {
    if (node == NULL || node->parent == NULL) {
        return NULL;
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
    if (node == NULL) {
        return;
    }
    rbtNode<T>* right = node->right;
    node->right = right->left;
    if (node->right != NIL) {
        node->right->parent = node;
    }
    right->parent = node->parent;
    if (node->parent == NULL) {
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
    if (node == NULL) {
        return;
    }
    rbtNode<T>* left = node->left;
    node->left = left->right;
    if (node->left != NIL) {
        node->left->parent = node;
    }
    left->parent = node->parent;
    if (node->parent == NULL) {
        setRoot(left);
    } else if (node == node->parent->right) {
        node->parent->right = left;
    } else {
        node->parent->left = left;
    }
    left->right = node;
    node->parent = left;
}

template <typename T>
void rbtree<T>::deleteKey(T key) {
    deleteKeyHelper(getRoot(), key);
}

template <typename T>
void rbtree<T>::deleteKeyHelper(rbtNode<T>* node, T key) {
    rbtNode<T>* nodeToBeDeleted = NIL;
    while (node != NIL) {
        if (node->key == key) {
            nodeToBeDeleted = node;
        }
        if (node->key <= key) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    if (nodeToBeDeleted == NIL) {
        return;
    }

    rbtNode<T>* y = nodeToBeDeleted;
    rbtNode<T>* x;
    int         original_color = y->color;
    if (nodeToBeDeleted->left == NIL) {
        x = nodeToBeDeleted->right;
        transplantNode(nodeToBeDeleted, x);
    } else if (nodeToBeDeleted->right == NIL) {
        x = nodeToBeDeleted->left;
        transplantNode(nodeToBeDeleted, x);
    } else {
        y = minKeyNode(nodeToBeDeleted->right);
        original_color = y->color;
        x = y->right;
        if (y->parent == nodeToBeDeleted) {
            x->parent = y;
        } else {
            transplantNode(y, y->right);
            y->right = nodeToBeDeleted->right;
            y->right->parent = y;
        }
        transplantNode(nodeToBeDeleted, y);
        y->left = nodeToBeDeleted->left;
        y->left->parent = y;
        y->color = nodeToBeDeleted->color;
    }
    delete nodeToBeDeleted;
    if (original_color == BLACK) {
        balanceAfterDelete(x);
    }
}

template <typename T>
void rbtree<T>::transplantNode(rbtNode<T>* u, rbtNode<T>* v) {
    if (u->parent == NULL) {
        setRoot(v);
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

template <typename T>
void rbtree<T>::balanceAfterDelete(rbtNode<T>* x) {
    while (x != getRoot() && x->color == BLACK) {
        if (x == x->parent->left) {
            rbtNode<T>* aunt = x->parent->right;
            if (aunt->color == RED) {
                setColor(aunt, BLACK);
                setColor(x->parent, RED);
                rotateLeft(x->parent);
                aunt = x->parent->right;
            }
            if (aunt->left->color == BLACK && aunt->right->color == BLACK) {
                setColor(aunt, RED);
                x = x->parent;
            } else {
                if (aunt->right->color == BLACK) {
                    setColor(aunt->left, BLACK);
                    setColor(aunt, RED);
                    rotateRight(aunt);
                    aunt = x->parent->right;
                }
                setColor(aunt, x->parent->color);
                setColor(x->parent, BLACK);
                setColor(aunt->right, BLACK);
                rotateLeft(x->parent);
                x = getRoot();
            }
        } else if (x == x->parent->right) {
            rbtNode<T>* aunt = x->parent->left;
            if (aunt->color == RED) {
                setColor(aunt, BLACK);
                setColor(x->parent, RED);
                rotateRight(x->parent);
                aunt = x->parent->left;
            }
            if (aunt->right->color == BLACK) {
                setColor(aunt, RED);
                x = x->parent;
            } else {
                if (aunt->left->color == BLACK) {
                    setColor(aunt->right, BLACK);
                    setColor(aunt, RED);
                    rotateLeft(aunt);
                    aunt = x->parent->left;
                }
                setColor(aunt, x->parent->color);
                setColor(x->parent, BLACK);
                setColor(aunt->left, BLACK);
                rotateRight(x->parent);
                x = getRoot();
            }
        }
    }
    setColor(x, BLACK);
}

template <typename T>
rbtNode<T>* rbtree<T>::minKeyNode(rbtNode<T>* node) {
    while (node->left != NIL) {
        node = node->left;
    }
    return node;
}

template <typename T>
rbtNode<T>* rbtree<T>::maxKeyNode(rbtNode<T>* node) {
    while (node->right != NIL) {
        node = node->right;
    }
    return node;
}

}  // namespace ft

#endif  // RBTREE_HPP_
