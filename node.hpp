#ifndef NODE_H
#define NODE_H

#include <cstdlib>  // NULL definition

template<class T> class BinarySearchTree;
template<class T> class Position;

template<class T>
class Node {
    private:
        T data;
        Node<T>* leftPtr;
        Node<T>* rightPtr;
        Node<T>* parentPtr;
    public:
        Node();
        Node(const T&);
        Node(const Node&);

        Node<T>& operator=(const Node<T>&);

        T getData() const;
        Node<T>* getLeftPtr() const;
        Node<T>* getRightPtr() const;
        Node<T>* getParentPtr() const;

        void setData(const T);
        void setLeftPtr(const Node<T>*);
        void setRightPtr(const Node<T>*);
        void setParentPtr(const Node<T>*);

        friend class BinarySearchTree<T>;
        friend class Position<T>;
};

// default constructor sets all pointers to NULL
template<class T>
Node<T>::Node() {
    leftPtr = NULL;
    rightPtr = NULL;
    parentPtr = NULL;
}

// constructor sets all pointers to NULL and data to parameter data
template<class T>
Node<T>::Node(const T& data) {
    this.data = data;
    leftPtr = NULL;
    rightPtr = NULL;
    parentPtr = NULL;
}

// copy constructor sets all pointers to NULL and data to old node's data
template<class T>
Node<T>::Node(const Node& oldNode) {
    data = oldNode.data;
    leftPtr = NULL;
    rightPtr = NULL;
    parentPtr = NULL;
}

// operator= overload performs deep copy of another Node<T>
template<class T>
Node<T>& Node<T>::operator=(const Node<T>& oldNode) {
    data = oldNode.data;
    leftPtr = NULL;
    rightPtr = NULL;
    parentPtr = NULL;
    return *this;
}

// getters
template<class T>
T Node<T>::getData() const{
    return data;
}

template<class T>
Node<T>* Node<T>::getLeftPtr() const{
    return leftPtr;
}

template<class T>
Node<T>* Node<T>::getRightPtr() const{
    return rightPtr;
}

template<class T>
Node<T>* Node<T>::getParentPtr() const{
    return parentPtr;
}

// setters
template<class T>
void Node<T>::setData(const T data){
    this.data = data;
}

template<class T>
void Node<T>::setLeftPtr(const Node<T>* oldPtr){
    leftPtr = oldPtr;
}

template<class T>
void Node<T>::setRightPtr(const Node<T>* oldPtr){
    rightPtr = oldPtr;
}

template<class T>
void Node<T>::setParentPtr(const Node<T>* oldPtr){
    parentPtr = oldPtr;
}

#endif