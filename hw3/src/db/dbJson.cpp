/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream &
operator<<(ostream &os, const DBJsonElem &j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream &operator>>(istream &is, DBJson &j)
{
   // TODO: to read in data from Json file and store them in a DB
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format
   assert(j._obj.empty());

   stringstream strStream;
   strStream << is.rdbuf();
   string jsonstring = strStream.str();

   // remove specific symbol
   jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), ' '), jsonstring.end());
   jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '\"'), jsonstring.end());
   jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '{'), jsonstring.end());
   jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '}'), jsonstring.end());
   jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '\n'), jsonstring.end());
   jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '\t'), jsonstring.end());

   // split jsonstring with ,
   vector<string> u;
   string::size_type pos1, pos2;
   string c = ",";
   pos2 = jsonstring.find(c);
   pos1 = 0;
   while (string::npos != pos2)
   {
      u.push_back(jsonstring.substr(pos1, pos2 - pos1));
      pos1 = pos2 + c.size();
      pos2 = jsonstring.find(c, pos1);
   }
   if (pos1 != jsonstring.length())
      u.push_back(jsonstring.substr(pos1));

   // split key and value with :
   for (vector<string>::size_type i = 0; i != u.size(); ++i)
   {
      vector<string> v;
      string::size_type pos1_, pos2_;
      string c_ = ":";
      pos2_ = u[i].find(c_);
      pos1_ = 0;
      while (string::npos != pos2_)
      {
         v.push_back(u[i].substr(pos1_, pos2_ - pos1_));
         pos1_ = pos2_ + c_.size();
         pos2_ = u[i].find(c_, pos1_);
      }
      if (pos1_ != u[i].length())
         v.push_back(u[i].substr(pos1_));

      istringstream iss(v[1]);
      int number;
      iss >> number;

      DBJsonElem jsonelem(v[0], number);
      j.add(jsonelem);
   }
   j.jsonread = true;
   return is;
}

// print all the json element
ostream &operator<<(ostream &os, const DBJson &j)
{
   // TODO
   os << "{\n";
   for (vector<string>::size_type i = 0; i != j.size() - 1; ++i)
      os << "  " << j[i] << ",\n";
   os << "  " << j._obj.back() << endl;
   os << "}\n";
   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void DBJson::reset()
{
   // TODO
   _obj.clear();
   jsonread = false;
}

// return false if key is repeated
bool DBJson::add(const DBJsonElem &elm)
{
   // TODO
   _obj.push_back(elm);
   return true;
}

// return NAN if DBJson is empty
float DBJson::ave() const
{
   // TODO
   if (!_obj.empty())
   {
      vector<string>::size_type i;
      int sum = 0;
      float ave = 0;
      for (i = 0; i != _obj.size(); ++i)
         sum += _obj[i].value();
      ave = (float)((float)sum / (float)_obj.size());
      return ave;
   }
   else
      return NAN;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int DBJson::max(size_t &idx) const
{
   // TODO
   // int maxN = INT_MIN;
   if (!_obj.empty())
   {
      int max = _obj[0].value();
      vector<string>::size_type i;
      for (i = 1; i != _obj.size(); ++i)
      {
         if (max < _obj[i].value())
         {
            max = _obj[i].value();
            idx = i;
         }
      }
      return max;
   }
   else
   {
      idx = _obj.size();
      return INT_MIN;
   }
}

// If DBJson is empty, set idx to size() and return INT_MAX
int DBJson::min(size_t &idx) const
{
   // TODO
   // int minN = INT_MAX;
   if (!_obj.empty())
   {
      int min = _obj[0].value();
      vector<string>::size_type i;
      for (i = 1; i != _obj.size(); ++i)
      {
         if (min > _obj[i].value())
         {
            min = _obj[i].value();
            idx = i;
         }
      }
      return min;
   }
   else
   {
      idx = _obj.size();
      return INT_MAX;
   }
}

void DBJson::sort(const DBSortKey &s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void DBJson::sort(const DBSortValue &s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int DBJson::sum() const
{
   // TODO
   int s = 0;
   if (!_obj.empty())
   {
      vector<string>::size_type i;
      for (i = 0; i != _obj.size(); ++i)
         s += _obj[i].value();
   }
   else
      s = 0;
   return s;
}
