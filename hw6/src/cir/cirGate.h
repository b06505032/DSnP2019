/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
public:
  friend class CirMgr;

  CirGate(GateType type, unsigned id, unsigned lineNo): _type(type), _id(id), _lineNo(lineNo) {}
  virtual ~CirGate() {}

  // Basic access methods
  string getTypeStr() const { 
  switch (_type) {
    case UNDEF_GATE: return "UNDEF";
    case PI_GATE:    return "PI";
    case PO_GATE:    return "PO";
    case AIG_GATE:   return "AIG";
    case CONST_GATE: return "CONST";
    default:         return "";
    }
  }
  unsigned getLineNo() const { return _lineNo; }

  // Printing functions
  // virtual void printGate() const = 0;
  void reportGate() const;
  void reportFanin(int level) const;
  void reportFanout(int level) const;

private:

protected:
  GateType _type;
  unsigned _id;
  unsigned _lineNo;
  GateList _fanin;
  GateList _fanout;
  string _name;
};

class CirConstGate: public CirGate  {
public:
  CirConstGate(): CirGate(CONST_GATE, 0, 0) {}
};

class CirPiGate: public CirGate  {
public:
  CirPiGate(unsigned id, unsigned lineNo): CirGate(PI_GATE, id, lineNo) {}
};

class CirPoGate: public CirGate  {
public:
  CirPoGate(unsigned id, unsigned lineNo): CirGate(PO_GATE, id, lineNo) {}
};

class CirAigGate: public CirGate {
public:
  CirAigGate(unsigned id, unsigned lineNo): CirGate(AIG_GATE, id, lineNo) {}
};

class CirUndefGate: public CirGate  {
public:
  CirUndefGate(unsigned id): CirGate(UNDEF_GATE, id, 0) {}
};

#endif // CIR_GATE_H
