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
    void insert_fix( AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void remove_fix( AVLNode<Key, Value>* n, int diff); 
    void rotateLeft( AVLNode<Key, Value>* n);
    void rotateRight( AVLNode<Key, Value>* n);
};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft( AVLNode<Key, Value>* n){
    if (n == nullptr || n -> getRight() == nullptr){
        return; 
    }
    if (n -> getParent() == nullptr){
        this -> root_ = n -> getRight();
    }
    AVLNode<Key, Value>* p = n -> getParent();
    AVLNode<Key, Value>* the_left = n -> getRight() -> getLeft(); 
    AVLNode<Key, Value>* right_c = n -> getRight(); 
    if(the_left != nullptr){
        the_left -> setParent(n); 
    }
    if( p != nullptr){
        if(p -> getLeft() == n){
            p -> setLeft(n -> getRight());
        }
        else{
            p -> setRight(n -> getRight()); 
        }
    }
    n -> getRight() -> setLeft(n); 
    n -> setRight(the_left); 
    n -> setParent(right_c); 
    right_c -> setParent(p); 
    
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n){
    if (n == nullptr || n -> getLeft() == nullptr){
        return; 
    }
    if (n -> getParent() == nullptr){
        this -> root_ = n -> getLeft();
    }
    AVLNode<Key, Value>* p = n -> getParent();
    AVLNode<Key, Value>* the_right = n -> getLeft() -> getRight(); 
    AVLNode<Key, Value>* left_c = n -> getLeft();  
    if(the_right != nullptr){
        the_right -> setParent(n); 
    }
    if( p != nullptr){
        if(p -> getRight() == n){
            p -> setRight(n -> getLeft());
        }
        else{
            p -> setLeft(n -> getLeft()); 
        }
    }
    n -> getLeft() -> setRight(n); 
    n -> setLeft(the_right); 
    n -> setParent(left_c); 
    left_c -> setParent(p); 
    
    return;
}




template<class Key, class Value> 
void AVLTree<Key, Value>::insert_fix( AVLNode<Key, Value>* p, AVLNode<Key, Value>* n){
    if(p == nullptr || p->getParent() == nullptr){
        return; 
    }
    AVLNode<Key, Value>* g = p->getParent();
    if(g->getLeft() == p){
        g->updateBalance(-1);
        if(g->getBalance() == 0){
            return;
        }
        else if(g->getBalance() == -1){
            insert_fix(g, p);
        }
        else if(g->getBalance() == -2){
            // zig zig
            if(p->getLeft() == n){
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0); 
            }
            // zig zag
            else if (p->getRight() == n){
                rotateLeft(p);
                rotateRight(g);
                if(n->getBalance() == -1){
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0); 
                }
                else if (n->getBalance() == 0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0); 
                }
                else if (n->getBalance() == 1){
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);                    
                }
            }
        }
    }
    else {
        g->updateBalance(1);
        if(g->getBalance() == 0){
            return;
        }
        else if(g->getBalance() == 1){
            insert_fix(g, p);
        }
        else if(g->getBalance() == 2){
            // zig zig
            if(p->getRight() == n){
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0); 
            }
            // zig zag
            else if (p->getLeft() == n){
                rotateRight(p);
                rotateLeft(g);
                if(n->getBalance() == 1){
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0); 
                }
                else if (n->getBalance() == 0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0); 
                }
                else if (n->getBalance() == -1){
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);                    
                }
            }
        }
    }

}







/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(BinarySearchTree<Key, Value>::empty()){
        AVLNode<Key, Value>* new_root = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        new_root -> setBalance(0); 
        this -> root_ = new_root; 
        return; 
    }
    else {
        AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this -> root_); 
        int idx = -1; 
        AVLNode<Key, Value>* the_node = nullptr; 
        while(temp != nullptr){
            if(temp -> getKey() == new_item.first){
            temp -> setValue(new_item.second); 
            return; 
            }
            else if(temp -> getKey() > new_item.first){
            the_node = temp; 
            temp = temp -> getLeft(); 
            idx = 0; 
            }
            else if(temp -> getKey() < new_item.first){
            the_node = temp; 
            temp = temp -> getRight(); 
            idx = 1; 
            }
        }
        AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr); 
        new_node -> setBalance(0); 
        new_node -> setParent(the_node); 
        if(idx == 0){
            the_node -> setLeft(new_node);

        }
        else if (idx == 1){
            the_node -> setRight(new_node); 

        }
        if(the_node -> getBalance() == -1 || the_node -> getBalance() == 1){
            the_node -> setBalance(0);
            return; 
        }
        else if(the_node -> getBalance() == 0){
            if(idx == 0){
                the_node -> setBalance(-1);
            }
            else if (idx == 1){
                the_node -> setBalance(1);
            }
            insert_fix(the_node, new_node); 
        }
        return; 
    }


}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should  with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if(static_cast<AVLNode<Key, Value>*>(this -> root_)== nullptr){
        return; 
    }
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(this -> root_) ;
    AVLNode<Key, Value>* parent = n -> getParent();
    int diff = 0; 
    while(n != nullptr){
        if(key == n -> getKey()){
            //found it, now let's see how many children it has
            if(n -> getLeft() != nullptr && n -> getRight() != nullptr){
                //two children, firnd predecessor 
                AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this -> predecessor(static_cast<Node<Key, Value>*>(n)));
                //now swap
                BinarySearchTree<Key, Value>::nodeSwap(pred, n);
            }
        parent = n -> getParent();
        AVLNode<Key, Value>* child = nullptr; 
        if( parent != nullptr){
            if( n -> getLeft() != nullptr){
                //has a left child, we promote left child and set diff to +1
                diff = 1;
                child = n -> getLeft(); 
                if (parent->getLeft() == n) {
                    parent->setLeft(child);
                } else {
                    parent->setRight(child);
                }
                child->setParent(parent);
            }
            else if(n -> getRight() != nullptr){
                //has a right child, we promote right child and set diff to -1
                diff = -1; 
                child = n -> getRight(); 
                if (parent->getLeft() == n) {
                    parent->setLeft(child);
                } else {
                    parent->setRight(child);
                }
                child->setParent(parent);
            }
            else{
                if (parent->getLeft() == n) {
                    parent->setLeft(nullptr);
                    diff = 1; 
                } else {
                    parent->setRight(nullptr);
                    diff = -1; 
                }
            }
        }
        else {
            // it is the root!!!!
            if( this -> root_ -> getLeft() != nullptr){
                this -> root_ = this -> root_ -> getLeft(); 
                this -> root_ -> setParent(nullptr); 
            }
            else if( this -> root_ -> getRight() != nullptr){
                this -> root_ = this -> root_ -> getRight(); 
                this -> root_ -> setParent(nullptr); 
            }
            else {
                this -> root_ = nullptr;        
            }
            }
            

            delete n; 
            break; 
        }
        else if(key < n -> getKey()){
            //go to the left
            n = n -> getLeft();
        }
        else if(key > n -> getKey()){
            //go to the right
            n = n -> getRight();
        }
    }
    if( diff == 0){
        return; 
    }
    remove_fix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix( AVLNode<Key, Value>* n, int diff){
    if(n == nullptr){
        return;
    }
    int ndiff = 0; 
    AVLNode<Key, Value>* p = n -> getParent();
    if(p != nullptr){
        if (n == p -> getLeft()){
            ndiff = 1;
        }
        else if(n == p -> getRight()){
            ndiff = -1;
        }
    }
    
    //now if difference is -1
    if(diff == -1){
        //case 1
        if(n -> getBalance() + diff == -2){
            AVLNode<Key, Value>* c = n -> getLeft();
            //case 1a
            if(c -> getBalance() == -1){
                rotateRight(n);
                n -> setBalance(0);
                c -> setBalance(0);
                remove_fix(p, ndiff);
            }
            else if(c -> getBalance() == 0){
                rotateRight(n);
                n -> setBalance(-1);
                c -> setBalance(1);
                return; 
            }
            else if(c -> getBalance() == 1){
                AVLNode<Key, Value>* g = c -> getRight();
                rotateLeft(c);
                rotateRight(n);
                //• removeFix(p, ndiff);
                if( g -> getBalance() == 1){
                    n -> setBalance(0);
                    c -> setBalance(-1);
                    g -> setBalance(0);
                }
                else if( g -> getBalance() == 0){
                    n -> setBalance(0);
                    c -> setBalance(0);
                    g -> setBalance(0); 
                }
                else if( g -> getBalance() == -1){
                    n -> setBalance(1);
                    c -> setBalance(0);
                    g -> setBalance(0);
                }
                remove_fix(p, ndiff); 
            }
        }
        //case 2
        else if(n -> getBalance() + diff == -1){
            n -> setBalance(-1);
            return;
        }
        //case 3
        else if(n -> getBalance() + diff == 0){
            n -> setBalance(0);
            remove_fix(p, ndiff); 
        }

    }
    else  if(diff == 1){
        //case 1
        if(n -> getBalance() + diff == 2){
            AVLNode<Key, Value>* c = n -> getRight();
            //case 1a
            if(c -> getBalance() == 1){
                rotateLeft(n);
                n -> setBalance(0);
                c -> setBalance(0);
                remove_fix(p, ndiff);
            }
            else if(c -> getBalance() == 0){
                rotateLeft(n);
                n -> setBalance(1);
                c -> setBalance(-1);
                return; 
            }
            else if(c -> getBalance() == -1){
                AVLNode<Key, Value>* g = c -> getLeft();
                rotateRight(c);
                rotateLeft(n);
                //• removeFix(p, ndiff);
                if( g -> getBalance() == -1){
                    n -> setBalance(0);
                    c -> setBalance(1);
                    g -> setBalance(0);
                }
                else if( g -> getBalance() == 0){
                    n -> setBalance(0);
                    c -> setBalance(0);
                    g -> setBalance(0); 
                }
                else if( g -> getBalance() == 1){
                    n -> setBalance(-1);
                    c -> setBalance(0);
                    g -> setBalance(0);
                }
                remove_fix(p, ndiff); 
            }
        }
        //case 2
        else if(n -> getBalance() + diff == 1){
            n -> setBalance(1);
            return;
        }
        //case 3
        else if(n -> getBalance() + diff == 0){
            n -> setBalance(0);
            remove_fix(p, ndiff); 
        }

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


#endif
