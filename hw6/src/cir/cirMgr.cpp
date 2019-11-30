/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
   // 1. READ IN ENTIRE FILE
   ifstream infile;
   infile.open("./" + fileName);
   if (infile.fail()) {
      cerr<<"Cannot open design \""<<fileName<<"\"!!"<<endl;
      return false;
   }
   stringstream strStream;
   strStream << infile.rdbuf();
   string aagstring = strStream.str();

   // 2. SPLIT THE STRING WITH \n
   vector<string> l;
   string::size_type pos1, pos2;
   string c = "\n";
   pos2 = aagstring.find(c);
   pos1 = 0;
   while(string::npos != pos2)
   {
      l.push_back(aagstring.substr(pos1, pos2-pos1));
      pos1 = pos2 + c.size();
      pos2 = aagstring.find(c, pos1);
   }
   if(pos1 != aagstring.length())
      l.push_back(aagstring.substr(pos1));
   // for(vector<string>::size_type i = 0; i != l.size(); ++i)
      // cout << "l["<<i<<"]" << l[i] << endl;

   // 3. SPLIT THE FIRST LINE WITH " ", AND STORE IN miloa[]
   vector<string> header;
   string::size_type pos1_, pos2_;
   string d = " ";
   pos2_ = l[0].find(d);
   pos1_ = 0;
   while(string::npos != pos2_)
   {
      header.push_back(l[0].substr(pos1_, pos2_-pos1_));
      pos1_ = pos2_ + d.size();
      pos2_ = l[0].find(d, pos1_);
   }
   if(pos1_ != l[0].length())
      header.push_back(l[0].substr(pos1_));
   miloa[0] = atof(header[1].c_str()); // store in miloa
   miloa[1] = atof(header[2].c_str());
   miloa[2] = atof(header[3].c_str());
   miloa[3] = atof(header[4].c_str());
   miloa[4] = atof(header[5].c_str());

   // 4. ADD THE GATE
   // CONST_GATE
   _Gatelist[0] = new CirConstGate();

   // PI_GATE
   for (unsigned i = 0; i < miloa[1]; i++) {
      unsigned id = atof(l[i+1].c_str())/2;
      unsigned lineNo = i+2;
      _in[i] = new CirPiGate(id, lineNo);
      _Gatelist[id] = _in[i];
   }

   // PO_GATE
   for (unsigned i = 0; i < miloa[3]; i++) {
      unsigned id = miloa[0]+i+1;
      unsigned lineNo = i+2+miloa[1];
      _out[i] = new CirPoGate(id, lineNo);
      _Gatelist[id] = _out[i];
   }

   // AIG_GATE
   for (unsigned i = 0; i < miloa[4]; i++) {
      vector<string> Aigs; //parse AIG | INPUT1 | INPUT2
      if (!lexOptions(l[i+1+miloa[1]+miloa[3]], Aigs)) return false;
      unsigned id = atof(Aigs[0].c_str())/2;
      unsigned lineNo = i+2+miloa[1]+miloa[3];
      _aig[i] = new CirAigGate(id, lineNo);
      _Gatelist[id] = _aig[i];
   }

   // 5. DEAL WITH AIG_GATE's FAN_IN FAN_OUT
   for (unsigned i = 0; i < miloa[4]; i++) {
      vector<string> Aigs; //parse AIG | INPUT1 | INPUT2
      if (!lexOptions(l[i+1+miloa[1]+miloa[3]], Aigs)) return false;
      for (unsigned count = 1; count != 3; count++) {
         if ((int)atof(Aigs[count].c_str()) % 2 != 0)
         { // inverting
            unsigned id = (atof(Aigs[count].c_str())-1)/2;
            map<unsigned, CirGate*>::iterator it = _Gatelist.find(id);
            if (it == _Gatelist.end())
               _Gatelist[id] = new CirUndefGate(id);
            else {
               _aig[i]->_fanin.push_back(_Gatelist[id]);
               _Gatelist[id]->_fanout.push_back(_aig[i]);
            }
         }
         else
         {
            unsigned id = atof(Aigs[count].c_str())/2;
            map<unsigned, CirGate*>::iterator it = _Gatelist.find(id);
            if (it == _Gatelist.end())
               _Gatelist[id] = new CirUndefGate(id);
            else {
               _aig[i]->_fanin.push_back(_Gatelist[id]);
               _Gatelist[id]->_fanout.push_back(_aig[i]);
            }
         }
      }
   }

   



   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
   int sum = miloa[1] + miloa[3] + miloa[4];
   cout << "Circuit Statistics" << endl;
   cout << "==================" << endl;
   cout << "  PI    " << setw(8) << right << miloa[1] << endl;
   cout << "  PO    " << setw(8) << right << miloa[3] << endl;
   cout << "  AIG   " << setw(8) << right << miloa[4] << endl;
   cout << "------------------" << endl;
   cout << "  Total " << setw(8) << right << sum << endl;
}

void
CirMgr::printNetlist() const
{
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
}

void
CirMgr::writeAag(ostream& outfile) const
{
}


bool
CirMgr::lexOptions(const string& option, vector<string>& tokens) const
{
   string token;
   size_t n = myStrGetTok(option, token);
   while (token.size()) {
      tokens.push_back(token);
      n = myStrGetTok(option, token, n);
   }
   return true;
}
