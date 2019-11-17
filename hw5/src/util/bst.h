/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;

   // BSTreeNode(){}
   BSTreeNode(const T& d, BSTreeNode<T>* p = NULL, BSTreeNode<T>* l = NULL, BSTreeNode<T>* r = NULL):
      _data(d), _parent(p), _left(l), _right(r) {}
   
   T               _data;
   BSTreeNode<T>*  _parent;
   BSTreeNode<T>*  _left;
   BSTreeNode<T>*  _right;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
public:
   BSTree() {
      // _root = NULL;
      _root = 0; _dummy = 0;
   }
   ~BSTree() {
      clear();
      delete _dummy;
   }
   class iterator
   { 
      friend class BSTree;

   public:
      iterator(BSTreeNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { 
         _node = BSTree::successor(_node);
         return *(this);
      }
      iterator operator ++ (int) { iterator tmp = *(this); ++*(this); return tmp; }
      iterator& operator -- () { return *(this); } // not done yet
      iterator operator -- (int) { iterator tmp = *(this); --*(this); return tmp; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return (_node != i._node); }
      bool operator == (const iterator& i) const { return (_node == i._node); }
     

   private:
      BSTreeNode<T>* _node;
   };
   
   iterator begin() const { 
      if (size()==0) 
         return end();
      else 
         return iterator(leftmost(_root)); 
   }
   iterator end() const { return iterator(_dummy); }
   bool empty() const { return size() == 0;}
   size_t size() const { return _size; }
   void insert(const T& x) { 
      if (size()==0) { // set the root first
         cout<<"build up the first node!"<<endl;
         BSTreeNode<T>* newNode = new BSTreeNode<T>(x);
         _root = newNode;
         _dummy = new BSTreeNode<T>(x);
         _dummy->_left = _root;
         _root->_right = _dummy;
         // _root->_parent = _dummy;
      } else {
         insertHelper(x, _root);
      }
      _size+=1;
      // InorderPrint ();
   }
   void pop_front(){ // leftmost node (i.e. begin()) will be removed.
      if(empty()) return;
      BSTreeNode<T>* temp = leftmost(_root);
      if(size()==1) {
         cout<< "delete the only node!"<<endl;
         _root = 0;
         _dummy = 0;
         _size = 0;
         return;
      }
      if(temp == _root) {
         cout<< "left most is the root!"<<endl;
         _root = temp->_right;
      }
      else {
         cout<<"else!"<<endl;
         if(temp->_right == NULL) {
            temp->_parent->_left = NULL;
         }
         else {
            temp->_parent->_left = temp->_right;
         }
      }
      delete temp;
      _size--;
      return;
   } 
   void pop_back() { // remove the last (rightmost for BSTree)
      
   } 
   bool erase(const T& x) { return true;}
   bool erase(iterator pos) { return true;}
   iterator find(const T& x) { return end();}
   void clear() { }
   void sort() const { }
   void print() const { }

private:
   BSTreeNode<T>* _root;
   BSTreeNode<T>* _dummy;
   int _size = 0;

   void insertHelper(const T data, BSTreeNode<T>* rootnode) {
      BSTreeNode<T>* t = new BSTreeNode<T>(data, rootnode, NULL, NULL);
      if (data<=(rootnode->_data)) { 
         if (rootnode->_left == NULL) {  
            rootnode->_left = t;
         }
         else 
            insertHelper(data, rootnode->_left);
      } 
      else { 
         if (rootnode->_right == _dummy) {
            rootnode->_right = t;
            t->_parent = rootnode;
            t->_right = _dummy;
            _dummy->_left = t;
         }
         else if (rootnode->_right == NULL) { 
            rootnode->_right = t;
         }
         else
            insertHelper(data, rootnode->_right);
      }
   }

   // The function to find the leftmost node
   static BSTreeNode<T>* leftmost(BSTreeNode<T> *current){
      while (current->_left != NULL) {
        current = current->_left;
      }
      return current;
   }

   static BSTreeNode<T>* rightmost(BSTreeNode<T> *current){
      while (current->_right != NULL) {
        current = current->_right;
      }
      return current;
   }

   // The function to find the minimum node of right descendant
   static BSTreeNode<T>* successor(BSTreeNode<T> *current) {
      if (current->_right != NULL) {
         if(current->_right->_left != current) {
            current =  leftmost(current->_right);
            return current;
         } else {
           current = current->_right;
           return current;
         }
      }
      BSTreeNode<T>* successor = current->_parent;   
      while (successor != NULL && current == successor->_right) {
         current = successor;
         successor = successor->_parent;
      }
      return successor;
   }

   void InorderPrint (){
      BSTreeNode<T>* current = new BSTreeNode<T>();
      current = leftmost(_root);
      while(current) {
         cout <<"(" << current->_data << ")" << " ";
         current = successor(current);
      }
   }

};

#endif // BST_H
