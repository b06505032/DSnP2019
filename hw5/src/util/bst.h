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

   // BSTreeNode(const T& d): 
      // _data(d) {_left = _right = _parent = NULL; }
   // BSTreeNode(const T& d, BSTreeNode<T>* p = NULL): 
      // _data(d), _parent(p), _left(NULL), _right(NULL) {}
   BSTreeNode(){}
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
   BSTree(){
      _root = NULL;
   }
   ~BSTree() {
      clear();
      delete _root;
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
      iterator& operator -- () { return *(this); }
      iterator operator -- (int) { iterator tmp = *(this); --*(this); return tmp; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return (_node != i._node); }
      bool operator == (const iterator& i) const { return (_node == i._node); }
     

   private:
      BSTreeNode<T>* _node;
   };
   
   iterator begin() const { return iterator(leftmost(_root)); }
   iterator end() const { return iterator(_dummy); }
   bool empty() const { return size() == 0;}
   size_t size() const { return _size; }
   void insert(const T& x) { 
      if (_root == NULL) {
         _root = new BSTreeNode<T>(x,NULL, NULL, NULL);
      } else {
         insertHelper(x, _root);
      }
      _size++;
      // InorderPrint (); // check if it is Inorder
   }
   void pop_front(){ } // leftmost node (i.e. begin()) will be removed.
   void pop_back() { } // remove the last (rightmost for BSTree)
   bool erase(const T& x) { return true;}
   bool erase(iterator pos) { return true;}
   iterator find(const T& x) { return end();}
   void clear() { }
   void sort() const { }
   void print() const { }

private:
   BSTreeNode<T>* _root;
   BSTreeNode<T>* _dummy;
   int _size;
   mutable bool _isSorted;

   void insertHelper(const T data, BSTreeNode<T>* rootnode) {
      if (data<=(rootnode->_data)) {
         if (rootnode->_left == NULL) {
            rootnode->_left = new BSTreeNode<T>(data, rootnode, NULL, NULL);
         } else {
            insertHelper(data, rootnode->_left);
         }
      } else {
         if (rootnode->_right == NULL) {
            rootnode->_right = new BSTreeNode<T>(data, rootnode, NULL, NULL);
         } else {
            insertHelper(data, rootnode->_right);
         }
      }
   }

   static BSTreeNode<T>* leftmost(BSTreeNode<T> *current){
      while (current->_left != NULL) {
        current = current->_left;
      }
      return current;
   }

   static BSTreeNode<T>* successor(BSTreeNode<T> *current){
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
