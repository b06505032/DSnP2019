/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () {  _node = _node->_next; return *(this); }
      iterator operator ++ (int) { iterator tmp = *(this); ++*(this); return tmp; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { iterator tmp = *(this); --*(this); return tmp;  }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return (this->_node != i._node); }
      bool operator == (const iterator& i) const { return (this->_node == i._node); }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_head); }
   iterator end() const { return iterator(_head->_prev); }
   iterator lastnode() const { return iterator(_head->_prev->_prev); }
   bool empty() const { return (_head->_prev == _head); }
   size_t size() const {
      size_t count = 0;
      for (iterator li = begin(); li != end(); li++)
         count++;
      return count;
   }

   void push_back(const T& x) { // NO PROBLEM!
      if(empty()) {
         DListNode<T>* temp = new DListNode<T>(x, _head, _head);
         _head->_next = _head->_prev = temp;
         _head = temp;
      }
      else { //insert a node before (head->prev)
         DListNode<T>* temp = new DListNode<T>(x, (_head->_prev)->_prev, (_head->_prev));
	      (_head->_prev)->_prev->_next = temp;
	      (_head->_prev)->_prev = temp;
         _isSorted = false;
      }
   }

   void pop_front() {
      if (empty()) { 
         // cout<<"empty!"<<endl;
         return;
      }
      else if(size()==1) {
         // cout << "pop_front the only node!" << endl;
         DListNode<T>* t = _head;
         _head = _head->_prev;
         _head->_prev = _head->_next = _head;
         delete t;
         return;
      }
      else {
         // cout << "pop_front!" << endl;
         DListNode<T>* temp = _head; // delete head, set temp=head
         _head = _head->_next;       // make the second node be the head
         temp->_prev->_next = temp->_next;
         temp->_next->_prev = temp->_prev;
         delete temp;
      }
		
   }
   void pop_back() { // NO PROBLEM!
      if (empty()) { 
         // cout<<"empty!"<<endl;
         return;
      }
      else if(size()==1) {
         // cout << "pop_back the only node!" << endl;
         DListNode<T>* temp = _head;
         _head = _head->_prev;
         _head->_prev = _head->_next = _head;
         delete temp;
         return;
      }
      else {
         // cout << "pop_back!" << endl;
         DListNode<T>* temp = _head->_prev->_prev;
         temp->_prev->_next = temp->_next;
         temp->_next->_prev = temp->_prev;
         delete temp;
      }
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      if (empty()) return false;
      if(pos==begin() && pos== lastnode() && size()==1) {
         // cout << "erase the only node!" << endl;
         DListNode<T>* t = _head;
         _head = _head->_prev;
         _head->_prev = _head->_next = _head;
         delete t;
         return true;
      }
      if(pos == begin()) { 
         // cout<<"erase the first node!"<<endl;
         pop_front();
         return true;
      }
      else if(pos == lastnode()) {
         // cout<<"erase the last node!"<<endl;
         pop_back();
         return true;
      }
      else {
         // cout<<"erase the middle node!"<<endl;
         pos._node->_prev->_next = pos._node->_next;
         pos._node->_next->_prev = pos._node->_prev;
         delete pos._node;
         return true; 
      }
   }
   bool erase(const T& x) { 
      if (empty())
         return false;
      iterator pos = find(x);
      if(pos==end()) {
         return false;
      }
      erase(pos);
      return true;
    }

   iterator find(const T& x) {
      // DListNode<T>* temp = _head;
      // while(_head->_next!=_head->_prev && _head->_next!=_head){
      //    temp = temp->_next;
      //    if(temp->_data == x){
      //       return (iterator)temp;
      //    }
      // }
      for (iterator i = begin(); i != end(); ++i)
         if (*i == x) return i;
      return end(); 
   }

   void clear() {
      if(empty()) return;
      while(!empty()) {
         pop_back();
      }
    }  // delete all nodes except for the dummy node

   void sort() const { }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
