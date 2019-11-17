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
         _node = it_successor(_node);
         return *(this);
      }
      iterator operator ++ (int) { iterator tmp = *(this); ++*(this); return tmp; }
      iterator& operator -- () { 
         _node = it_predecessor(_node);
         return *(this);
      }
      iterator operator -- (int) { iterator tmp = *(this); --*(this); return tmp; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return (_node != i._node); }
      bool operator == (const iterator& i) const { return (_node == i._node); }
     

   private:
      BSTreeNode<T>* _node;
      BSTreeNode<T>* leftmost(BSTreeNode<T> *current){
         while (current->_left != NULL) {
            current = current->_left;
         }
         return current;
      }
      BSTreeNode<T>* rightmost(BSTreeNode<T>* current) {
         while(current->_right != NULL && current->_right->_left != current) {
            current = current->_right;
         } 
         return current;
      }
      BSTreeNode<T>* it_successor(BSTreeNode<T> *current) {
         if (current->_right != NULL) {
            if(current->_right->_left != current) {
               current = leftmost(current->_right);
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
      BSTreeNode<T>* it_predecessor(BSTreeNode<T> *current) {
         if (current->_left != NULL) {
            current = rightmost(current->_left);
            return current;
         }
         BSTreeNode<T>* predecessor = current->_parent;   
         while (predecessor != NULL && current == predecessor->_left) {
            current = predecessor;
            predecessor = predecessor->_parent;
         }
         return predecessor;
      }
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
      if (empty()) return;
      erase(begin());
   } 
   void pop_back() { // remove the last (rightmost for BSTree)
      if (empty()) return;
      erase(--end());
   } 
   iterator find(const T& x) { 
      for (iterator i = begin(); i != end(); ++i)
         if (*i == x) return i;
      return end();
   }
   bool erase(const T& x) { 
      if (empty()) return false; 
      iterator pos = find(x);
      if (pos==end()) return false;
      else erase(pos);
      return true;
   }
   bool erase(iterator pos) {
      if (empty()) return false; 
      if(size()==1) {
         _root = 0;
         _dummy = 0;
         _size = 0;
         return true;
      }
      BSTreeNode<T>* delete_node = pos._node;
      BSTreeNode<T>* y = 0;
      BSTreeNode<T>* x = 0;
      if (delete_node->_left == NULL || delete_node->_right == NULL) {
        y = delete_node;
      }
      else{
        y = successor(delete_node);
      } 
      if (y->_left != NULL) {
         x = y->_left;
      } 
      else{
         x = y->_right;
      }

      if (x != NULL) { 
         x->_parent = y->_parent; 
      }
      if (y->_parent == NULL) {
         this->_root = x;
      }
      else if (y == y->_parent->_left){
         y->_parent->_left = x;
      }
      else {
         y->_parent->_right = x;
      }
      if (y != delete_node) { 
         delete_node->_data = y->_data; 
      }
      delete y;
      y = 0;

      if (_root != 0) {
         BSTreeNode<T>* maxnode = rightmost(_root);
         maxnode->_right = _dummy;
         _dummy->_left = maxnode;
      }
      _size--;
      return true;
   }
   void clear() {
      if (empty()) return;
      while(!empty()) {
         pop_front();
      }
   }
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
   BSTreeNode<T>* leftmost(BSTreeNode<T>* current) const {
      while (current->_left != NULL) {
        current = current->_left;
      }
      return current;
   }

   BSTreeNode<T>* rightmost(BSTreeNode<T>* current) const {
      while (current->_right != _dummy && current->_right != NULL) {
         current = current->_right;
      }
      return current;
   }

   // The function to find the minimum node of right descendant
   BSTreeNode<T>* successor(BSTreeNode<T>* current) {
      // return leftmost(current->_right);
      if (current->_right != NULL) {
         return leftmost(current->_right);
      }
      BSTreeNode<T>* successor = current->_parent;   
      while (successor != NULL && current == successor->_right) {
         current = successor;
         successor = successor->_parent;
      }
      return successor;
   }
   


};

#endif // BST_H
