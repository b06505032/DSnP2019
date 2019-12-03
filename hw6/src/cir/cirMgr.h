/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr(){}
   ~CirMgr() {}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const { 
      map<unsigned, CirGate*>::const_iterator it = _Gatelist.find(gid);
      if (it == _Gatelist.end()) return 0;
      return it->second;
   }

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

private:
   // m, maximum index
   // i, #inputs
   // l, #latches = 0
   // o, #outputs
   // a, #AND gates
   vector<string> l;
   unsigned miloa[5];
   GateList _in;
   GateList _out;
   GateList _aig;
   map<unsigned, CirGate*> _Gatelist;
   GateList _dfsList;
   vector<string> _comments;
   
   unsigned num_vertex;
   int *color;           // 0:white, 1:gray, 2:black
   int *predecessor;
   int *discover;
   int *finish;
   void DFS(unsigned Start);
   void DFSVisit(unsigned vertex, int &time);

   // Helper function
   void readHeader();
   void readInput();
   void readOutput();
   void readAig();
   void connection();
   bool lexOptions(const string& option, vector<string>& tokens) const;
   void genDFSList();
};

#endif // CIR_MGR_H
