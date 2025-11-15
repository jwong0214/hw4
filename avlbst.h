#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
  // Constructor/destructor.
  AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
  virtual ~AVLNode();

  // Getter/setter for the node's height.
  int8_t getBalance () const;
  void setBalance (int8_t balance);
  void updateBalance(int8_t diff);

  // Getters for parent, left, and right. These need to be redefined since they
  // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
  // for more information.
  virtual AVLNode<Key, Value>* getParent() const override;
  virtual AVLNode<Key, Value>* getLeft() const override;
  virtual AVLNode<Key, Value>* getRight() const override;

protected:
  int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
  return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
  balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
  balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
  return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
  return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
  return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
  virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
  virtual void remove(const Key& key);  // TODO
protected:
  virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

  // Add helper functions here
  void rotateLeft(AVLNode<Key, Value>* x);
  void rotateRight(AVLNode<Key, Value>* x);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item) {
  // TODO
  // base case
  if (this->root_ == nullptr) {
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr); // key, value, parent
    return;
  }
  AVLNode<Key,Value>* current = static_cast<AVLNode<Key,Value>*>(this->root_);
  AVLNode<Key,Value>* parent = nullptr;
  // BST search
  while (current != nullptr) {
    parent = current;
    // if new key is smaller -> go left
    if (new_item.first < current->getKey()) {
      current = current->getLeft();
    // if new key is greater -> go right
    } else if (new_item.first > current->getKey()) {
      current = current->getRight();
    // if key already exists -> overwrite
    } else {
      current->setValue(new_item.second);
      return;
    }
  }

  // insert new node under parent
  AVLNode<Key,Value>* child = new AVLNode<Key,Value>(new_item.first, new_item.second, parent);
  if (new_item.first < parent->getKey()) {
    parent->setLeft(child);
  } else {
    parent->setRight(child);
  }
  // fix AVL balances upward
  while (parent != nullptr) {
    // update balance depending on which side the child was on
    if (child == parent->getLeft()) {
      parent->updateBalance(-1);
    } else {
      parent->updateBalance(1);
    }
    // if height of parent did not change -> stop
    if (parent->getBalance() == 0) {
      break;
    }
    // if parent is now + or -1 -> continue going up
    if (parent->getBalance() == 1 || parent->getBalance() == -1) {
      child = parent;
      parent = parent->getParent();
      continue;
    }
    // if parent is now + or -2 -> unbalanced, so rotation is needed
    if (parent->getBalance() == 2) {
      // right is larger
      AVLNode<Key, Value>* right = parent->getRight();
      if (right->getBalance() >= 0) {
        rotateLeft(parent); // right-right (zig-zig)
      } else {
        rotateRight(right); // right-left (zig-zag)
        rotateLeft(parent); // right-left (zig-zag)
      }
      break;
    }
    if (parent->getBalance() == -2){
      // left is larger
      AVLNode<Key, Value>* left = parent->getLeft();
      if (left->getBalance() <= 0) {
        rotateRight(parent); // left-left (zig-zig)
      } else {
        rotateLeft(left); // left-right (zig-zag)
        rotateRight(parent); // left-right (zig-zag)
      }
      break;
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  // TODO
  // find node to remove
  AVLNode<Key,Value>* rem = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  // no key
  if(rem == nullptr) {
    return;
  }

  // if has two children -> swap with predecessor
  if(rem->getLeft() != nullptr && rem->getRight() != nullptr) {
    AVLNode<Key,Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(rem));
    nodeSwap(rem, pred);
  }

  // after swapping above, find rem's at most one child (if any)
  AVLNode<Key,Value>* parent = rem->getParent();
  AVLNode<Key,Value>* child;
  if (rem->getLeft() != nullptr) {
    child = rem->getLeft();
  } else {
    child = rem->getRight();
  }

  int8_t balanceChange = 0;

  // remove node from tree via BST removal
  if(child != nullptr)
    child->setParent(parent);

  if(parent == nullptr) {
    this->root_ = child;
  } else if(rem == parent->getLeft()) {
    parent->setLeft(child);
    balanceChange = 1;   // left subtree shrank
  } else {
    parent->setRight(child);
    balanceChange = -1;   // right subtree shrank
  }

  delete rem; // delete remove node for valgrind

  // fix AVL balances upwards
  while (parent != nullptr) {
    parent->updateBalance(balanceChange);
    // if balance is now + or -1 -> height did not change -> stop
    if (parent->getBalance() == 1 || parent->getBalance() == -1) {
      break;
    }
    // if balance is now 0 -> unbalanced and decreased
    if (parent->getBalance() == 0) {
      // new balance change
      AVLNode<Key,Value>* grandparent = parent->getParent();
      if(grandparent == nullptr) {
        break;
      }
      if(parent == grandparent->getLeft()) {
        balanceChange = +1;
      } else {
        balanceChange = -1;
      }
      parent = grandparent;
      continue;
    }
    // if balance is now + or -2 -> unbalanced, so rotation is needed
    if (parent->getBalance() == 2) {
      // right is larger
      AVLNode<Key, Value>* right = parent->getRight();
      if (right->getBalance() >= 0) {
        rotateLeft(parent); // right-right (zig-zig)
      } else {
        rotateRight(right); // right-left (zig-zag)
        rotateLeft(parent); // right-left (zig-zag)
      }
    } else {
      // left is larger
      AVLNode<Key, Value>* left = parent->getLeft();
      if (left->getBalance() <= 0) {
        rotateRight(parent); // left-left (zig-zig)
      } else {
        rotateLeft(left); // left-right (zig-zag)
        rotateRight(parent); // left-right (zig-zag)
      }
    }
    AVLNode<Key,Value>* grandparent = parent->getParent();

    // if grandparent's balance == 0 -> subtree height decreased -> keep going up
    if (grandparent->getBalance() == 0) {
      parent = grandparent->getParent();
      if (parent == nullptr) {
        break;
      }
      if (grandparent == parent->getLeft()) {
        balanceChange = 1;
      } else {
        balanceChange = -1;
      }
      continue;
    }
    break;
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
  BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
  int8_t tempB = n1->getBalance();
  n1->setBalance(n2->getBalance());
  n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* x) {
  AVLNode<Key,Value>* y = x->getRight();   // new root of subtree
  AVLNode<Key,Value>* b = y->getLeft();    // b subtree (middle)

  // move y up above x
  y->setParent(x->getParent());
  if (x->getParent() == nullptr) {
    this->root_ = y;
  } else if (x == x->getParent()->getLeft()) {
    x->getParent()->setLeft(y);
  } else {
    x->getParent()->setRight(y);
  }

  // rotate
  y->setLeft(x);
  x->setParent(y);

  // attach b under x
  x->setRight(b);
  if (b != nullptr) {
    b->setParent(x);
  }

  // update balances
  int8_t yb = y->getBalance(); 
  int8_t xb = x->getBalance(); 
  x->setBalance(xb - 1 - std::max<int8_t>(0, yb)); 
  y->setBalance(yb - 1 + std::min<int8_t>(0, x->getBalance()));
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* x) {
  AVLNode<Key,Value>* y = x->getLeft();   // new root of subtree
  AVLNode<Key,Value>* b = y->getRight();    // b subtree (middle)

  // move y up above x
  y->setParent(x->getParent());
  if (x->getParent() == nullptr) {
      this->root_ = y;
  } else if (x == x->getParent()->getLeft()) {
      x->getParent()->setLeft(y);
  } else {
      x->getParent()->setRight(y);
  }

  // rotate
  y->setRight(x);
  x->setParent(y);

  // attach b under x
  x->setLeft(b);
  if (b != nullptr) {
    b->setParent(x);
  }

  // update balances
  int8_t yb = y->getBalance(); 
  int8_t xb = x->getBalance(); 
  x->setBalance(xb + 1 - std::min<int8_t>(0, yb)); 
  y->setBalance(yb + 1 + std::max<int8_t>(0, x->getBalance()));
}

#endif