#ifndef POSITION_H
#define POSITION_H

#include "node.hpp"

template<class T> class BinarySearchTree; // required so BinarySearchTree class can create position from node

template<class T>
class Position {
    private:
        Node<T>* nodePtr;
        Position(): nodePtr(NULL) {}
        Position(Node<T>*);
    public:
        Node<T>& operator*() { return *nodePtr; }
        Position parent() const { return Position<T>( this->nodePtr->parentPtr ); }
        Position left() const { return Position<T>( this->nodePtr->leftPtr ); }
        Position right() const { return Position<T>( this->nodePtr->rightPtr ); }
        bool isRoot() const { return this->nodePtr->parentPtr->parentPtr == NULL; }
        bool isSuperRoot() const { return this->nodePtr->parentPtr == NULL; }
        bool isExternal() const { return this->nodePtr->leftPtr == NULL && this->nodePtr->rightPtr == NULL; }
        bool isInternal() const { return !(isExternal()); }
        bool operator==(const Position& p) { return this->nodePtr == p.nodePtr; }
        bool operator!=(const Position& p) { return this->nodePtr != p.nodePtr; }
        Position<T>& operator++(); // inorder traversal
        T getItem() { return nodePtr->data; }
        friend class BinarySearchTree<T>;
};

template<class T>
Position<T>::Position(Node<T>* oldNodePtr) { nodePtr = oldNodePtr; }

template<class T>
Position<T>& Position<T>::operator++() {
    Position w = this->right();

    if (w.isInternal()) {
        do {
            *this = w.nodePtr;
            w.nodePtr = w.nodePtr->leftPtr;
        } while (w.isInternal());
    } else {
        w.nodePtr = this->nodePtr->parentPtr;
        while (w.nodePtr != NULL && w.nodePtr->rightPtr == this->nodePtr) {
            *this = w.nodePtr;
            w.nodePtr = w.nodePtr->parentPtr;
        }
        *this = w.nodePtr;
    }

    return *this;
}

#endif