/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data[0]; }
   void insert(const Data& d)
   { 
      _data.push_back(d);
      size_t t = _data.size() - 1;
      while( t > 0)
      {
         size_t p = (t-1) / 2; // parent = (child-1) /2
         if(d < _data[p])
         {
            _data[t] = _data[p];
            t = p;
         }
         else break;
      }
      _data[t] = d;
   }
   void delMin() { delData(0); }
   void delData(size_t i) 
   { 
      _data[0] = _data[size()-1];
      _data.pop_back();
      size_t n = i;
      size_t c = 2*n + 1;
      while(c < size())    
      {
         if (c+1 < size()) // has right child
         {
            if ( _data[c+1] < _data[c] )
               c++; // to the smaller child
         }
         if (_data[c] < _data[n] ) // parent is lager
            swap(_data[n], _data[c]);
         n = c;
         c = 2*n + 1;
      }
   }

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
