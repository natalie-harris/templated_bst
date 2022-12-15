#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "node.hpp"
#include "position.hpp"

const int PREORDER = 1;
const int INORDER = 2;
const int POSTORDER = 3;

template<class T> class Position;

template<class T> 
class BinarySearchTree {
    private:
        int numNodes; // number real nodes in tree, not counting superroot
        Node<T>* superRootPtr;

        // find item in tree starting at position
        Position<T> findUtility( const T&, const Position<T>& ) const;

        // insert item into tree
        Position<T> insertUtility( const T& );

        // delete all nodes in the tree
        void deleteUtility( Node<T>* );

        // erases just one node from the tree at position p
        void eraseUtility( const Position<T>& );

        // insert item data at position p
        void expandExternal( const T&, Position<T>& p );

        // remove node above fake leaf node at position p
        Position<T> removeAboveExternal( const Position<T>& p );

        Node<T>* copyTree(Node<T>* p, Node<T>* n);

    public:
        BinarySearchTree();
        BinarySearchTree( const BinarySearchTree<T>& );

        // returns num nodes in tree
        int size() const { return numNodes; };

        // returns true if tree empty of real nodes
        bool empty() const { return numNodes <= 0; }

        // superroot right pointer always points to real root node
        Position<T> root() const {
            return Position<T>( superRootPtr->rightPtr );
        }

        void traverseAndPrint( const Position<T>& p, int type ) const;

        int depth( const Position<T>& ) const;
        int height( const Position<T>& ) const;

        Position<T> begin() const;

        // return NULL iterator
        Position<T> end() const { return Position<T>(); }

        Position<T> find( const T& ) const;
        Position<T> insert( const T& );

        bool erase( const T& );
        bool erase( const Position<T>& );

        virtual ~BinarySearchTree() { deleteUtility(superRootPtr); }
        friend class Position<T>;
};

/*findUtility, recursive function
 *Parameters:
 * 	item to look for
 * 	p position to start looking from 
 * 	operator<, tree not empty of real nodes
 * Pre-condition: Class T has overloaded operator== 
 * and operator<, tree not empty of real book
 * Returns position iterator where found or NULL iterator*/
template<class T>
Position<T> BinarySearchTree<T>::findUtility(const T& item, const Position<T> &p) const{
	if(p.isExternal()){
		//Base case, first so no more checking if external.
		return p;//ends recursion
	}
	if(item == p.nodePtr->data){
		return p;//base case for recursion
	}
	if(item < p.nodePtr->data){
		//since item is smaller, we know it's in left subtree
		return findUtility(item, p.left());
	}
	return findUtility(item, p.right());
	//Otherwise, must be right. 
}

/* insertUtility: inserts at position for new node
 * Parameters: new node to insert
 * Returns iterator to newly inserted object or to superRoot
 *    fake node if item already in tree
 */

template<class T>
Position<T> BinarySearchTree<T>::insertUtility( const T& item ){
	Position<T> foundPosition = findUtility(item, root());
	if(foundPosition.nodePtr != NULL){
		expandExternal(item, foundPosition);
		return foundPosition;
	}
    return superRootPtr;
	
}

template<class T>
void BinarySearchTree<T>::deleteUtility( Node<T>* n ){
	if(n != NULL){
		deleteUtility(n->leftPtr);
		deleteUtility(n->rightPtr);
		delete n;
		//Delocate memory - free in c/delete in c++
		
	}
}

template<class T>
void BinarySearchTree<T>::eraseUtility( const Position<T>& p ){
    Position<T> w;
    Position<T> temp;
    temp.nodePtr = p.nodePtr->leftPtr;
    if (temp.isExternal()) {
        w.nodePtr = p.nodePtr->leftPtr;
    } else { 
        temp.nodePtr = p.nodePtr->rightPtr;
        if (temp.isExternal()) {
            w.nodePtr = p.nodePtr->rightPtr;
        } else {
            w.nodePtr = p.nodePtr->rightPtr;
            do {
                w.nodePtr = w.nodePtr->leftPtr;
            } while (w.isInternal());
            Position<T> u = w.nodePtr->parentPtr;
            p.nodePtr->data = u.nodePtr->data;
        } 
    } removeAboveExternal(w);
}

/* expandExternal - inserts item by copying its data to 
 *   position p and setting position's left and right ptrs 
 *   to end nodes
 * Parameters
 *   item to be inserted
 *   p fake end node where item will be inserted
 * Precondition: p is an external node (a fake leaf) and 
 *    class T has overloaded = operator
 */
template<class T>
void BinarySearchTree<T>::expandExternal( const T& item, Position<T>& p){
	p.nodePtr->leftPtr = new Node<T>;
	p.nodePtr->rightPtr = new Node<T>;
	p.nodePtr->leftPtr->parentPtr = p.nodePtr;
	p.nodePtr->rightPtr->parentPtr = p.nodePtr;
	p.nodePtr->data = item;
	numNodes++;
}

template<class T>
Position<T> BinarySearchTree<T>::removeAboveExternal( const Position<T>& p ){
	Node<T>* w = p.nodePtr;
	Node<T>* v = w->parentPtr;
	Node<T>* siblingPtr;
	if( w = v->leftPtr) {
		//set sibling of w
		siblingPtr = v->rightPtr; 
	}
	else{
		siblingPtr = v->leftPtr;
	}
	if(v == superRootPtr->rightPtr) {
		superRootPtr->rightPtr = siblingPtr;
		siblingPtr->parentPtr = superRootPtr;
	}
	else{
		//removing leaf that isnt actual root
		if (v->parentPtr->leftPtr == v){
			v->parentPtr->leftPtr = siblingPtr;
		}
		else{//v is to right of parent
			v->parentPtr->rightPtr = siblingPtr;
		}
		siblingPtr->parentPtr = v->parentPtr;
	}
	delete w;
	delete v;
	numNodes--;
	return siblingPtr;
}

/* copyTree utility function that copies one tree to this' tree
 * Parameters
 *    p pointer to parent in this' tree that new node's 
         parentPtr should point to
 *    n pointer to new node in tree to copy from
 * Returns pointer to node just allocated for this' tree or NULL 
 *   (if at the end of a branch) and no new node to copy
 */
template<class T>
Node<T>* BinarySearchTree<T>::copyTree(Node<T>* p, Node<T>* n){

	if (n != NULL){
		Node<T>* newNode = new Node(*n);
		newNode->parentPtr = p;
		newNode->leftPtr = copyTree(newNode, n->leftPtr);
		newNode->rightPtr = copyTree(newNode, n->rightPtr);
		return newNode;
	}
	else{
		return NULL;
	}
}

/* default ctor sets up an empty tree with two nodes:
 * fake root whose right ptr points to fake end
 */
template<class T>
BinarySearchTree<T>::BinarySearchTree(){
	
	Node<T>* temp = new Node<T>();
	superRootPtr = new Node<T>(); //create superroot
	superRootPtr->rightPtr = temp;//create tree root 
	temp->parentPtr = superRootPtr; //parentPtr of tree root assigned as super ptr
	superRootPtr->leftPtr = NULL; //superRootPtr's left PTR NULL.
	numNodes = 0;//Empty tree
}

/*Copy ctor*/
template<class T>
BinarySearchTree<T>::BinarySearchTree( const BinarySearchTree& b){
	this->numNodes = b.numNodes;
	this->superRootPtr = new Node<T>;
	if(b.root() == NULL){
		this->superRootPtr->rightPtr = new Node<T>;
		this->superRootPtr->rightPtr->parentPtr = superRootPtr;
	}
	else{
		superRootPtr->rightPtr = copyTree(superRootPtr, b.superRootPtr->rightPtr);
	}
}

template<class T>
void BinarySearchTree<T>::traverseAndPrint(const Position<T>& p, int c) const {
    if (c == PREORDER) {
        if (p.nodePtr == NULL) {
            return;
        }
        else {
            printf("%d ", p.nodePtr->data);
            if (p.nodePtr->leftPtr->leftPtr != NULL)
            traverseAndPrint(p.nodePtr->leftPtr, c);
            if (p.nodePtr->rightPtr->leftPtr != NULL)
            traverseAndPrint(p.nodePtr->rightPtr, c);
        }
    } else if (c == INORDER) {
        if (p.nodePtr == NULL) {
            return;
        }
        else {
            if (p.nodePtr->leftPtr->leftPtr != NULL)
            traverseAndPrint(p.nodePtr->leftPtr, c);
            printf("%d ", p.nodePtr->data);
            if (p.nodePtr->rightPtr->leftPtr != NULL)
            traverseAndPrint(p.nodePtr->rightPtr, c);
        }
    } else if (c == POSTORDER) {
        if (p.nodePtr == NULL) {
            return;
        }
        else {
            if (p.nodePtr->leftPtr->leftPtr != NULL)
            traverseAndPrint(p.nodePtr->leftPtr, c);
            if (p.nodePtr->rightPtr->leftPtr != NULL)
            traverseAndPrint(p.nodePtr->rightPtr, c);
            printf("%d ", p.nodePtr->data);
        } 
    } else return;
}

/* depth returns distance from root to node at parameter
 * position
 * Parameter:
 *   p position to compute depth of
 * Returns depth or -1 if position p is superroot
*/
template<class T>
int BinarySearchTree<T>::depth( const Position<T>& p) const{
	if ( p.isSuperRoot()) return -1;
	if ( p.isRoot()) return 0;
	else {
		return 1+depth( p.parent());
	}
}

template<class T>   
int BinarySearchTree<T>::height( const Position<T>& p) const {
	//if leaf
	if (p.isExternal()) return 0;
	int h = 0; 
	//h = max of 0 and height of left subtree
	h = max(h, height( Position<T>( p.nodePtr->leftPtr)));
	//h = max of height of left subtree and 
	//height of right subtree
	h = max( h, height( Position<T>(p.nodePtr->rightPtr)));
	return 1 + h;
}

// Function begin:
// Pre-condition = intialized tree that is not empty
// Post-Condition = return of the leftmost node
template<class T>
Position<T> BinarySearchTree<T>::begin() const {
	if (numNodes > 0){ // if the tree isn't empty
		Position<T> p = root();// start at real root
		while (p.isInternal()){//while not fake leaf
			p = p.left();//move p all the way to fake leaf
		}
		return p.parent();//return p's parent
	} return NULL;
}

/* finds a T object in tree
 * Parameters: the object to find
 * Pre-condition: class T has overloaded operator==
 * Returns iterator to object, superroot iterator if tree
 *   is empty, fake leaf postion if not found
 */

template<class T> 
Position<T> BinarySearchTree<T>::find(const T& data) const{
	//check for empty tree
	if( this->numNodes > 0 ){
		Position<T> v = findUtility( data, root());
		return v;
	}
	else return Position<T>(superRootPtr);
}

template<class T>
Position<T> BinarySearchTree<T>::insert( const T& item ) {
   // root() = superroot's right ptr 
   Position<T> p = insertUtility( item ); 
   // return iterator to position where new node inserted
   return p; 
}

template<class T>
bool BinarySearchTree<T>::erase( const T& data) {
    if ( this->numNodes > 0 ) {
        Position<T> v = findUtility( data, root() );
        eraseUtility(v);
        return true;
    } else return false;
}

template<class T>
bool BinarySearchTree<T>::erase( const Position<T>& p){
	
	if(!p.isSuperRoot() && !p.isExternal()){
		eraseUtility(p);
		return true;
	}
	else
		return false;
}

#endif