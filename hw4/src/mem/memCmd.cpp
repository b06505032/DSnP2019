/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest; // defined in memTest.cpp

bool initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)))
   {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}

//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string &option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size())
   {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj))))
      {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
#ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
#else
      mtest.reset();
#endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void MTResetCmd::usage(ostream &os) const
{
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void MTResetCmd::help() const
{
   cout << setw(15) << left << "MTReset: "
        << "(memory test) reset memory manager" << endl;
}

//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string &option)
{
   // TODO
   int num_obj;
   int size_arr;
   vector<string> options;
   if (!lexOptions(option, options))
      return CmdExec::errorOption(CMD_OPT_MISSING, "");
   bool isarray = false;
   int array_pos;
   for (size_t i = 0, n = options.size(); i < n; ++i)
   {
      if (myStrNCmp("-Array", options[i], 2) == 0)
      {
         isarray = true;
         array_pos = i;
         break;
      }
   }
   if (options.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, "");
   else if (options.size() > 3)
      return CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);
   else if (options.size() == 1)
   {
      if (myStrNCmp("-Array", options[0], 2) == 0)
         return CmdExec::errorOption(CMD_OPT_MISSING, options[0]);
      else if (!myStr2Int(options[0], num_obj))
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
      else if (num_obj <= 0)
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
      else
      {
         cout << "add object!" << endl;
         try
         {
            mtest.newObjs(num_obj);
         }
         catch (bad_alloc)
         {
            return CMD_EXEC_ERROR;
         }
      }
   }
   else if (options.size() == 2)
   {
      if (!isarray) // if "-array" doesn't exist
      {
         if (!myStr2Int(options[0], num_obj))  // mtn <non-number> <...>
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         else // mtn <number> <extra>
         {
            return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);
         }
      }
      else
      { // if "-array" exist
         if (array_pos == 0) // options[0] is "-array"
         { 
            if (myStr2Int(options[1], num_obj)) // mtn -a <number>
            {
               if (num_obj <= 0) 
                  return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
               else
                  return CmdExec::errorOption(CMD_OPT_MISSING, "");
            }
            else // mtn -a <non-number>
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
         }
         else // options[1] is "-array"
         { 
            if (myStr2Int(options[0], num_obj)) // mtn <number> -a
            { 
               if (num_obj <= 0) // mtn <illegal-number> -a
                  return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
               else // mtn <number> -a --> missing!
                  return CmdExec::errorOption(CMD_OPT_MISSING, options[1]);
            }
            else // mtn <non-number> -a
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         }
      }
   }
   else if (options.size() == 3)
   {
      bool isarray = false;
      int array_pos;
      for (size_t i = 0, n = options.size(); i < n; ++i)
      {
         if (myStrNCmp("-Array", options[i], 2) == 0)
         {
            isarray = true;
            array_pos = i;
            break;
         }
      }
      if (!isarray)
      {
         if (myStr2Int(options[0], num_obj))
         { //check options[0]
            if (num_obj <= 0)
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
            else
               return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);
         }
         else
         {
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         }
      }
      else
      {
         if (array_pos == 0)
         {
            if (myStr2Int(options[1], size_arr))
            { //check options[1]
               if (size_arr <= 0)
                  return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
               else
               {
                  if (myStr2Int(options[2], num_obj))
                  { //check options[2]
                     if (num_obj <= 0)
                        return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
                     else
                     {
                        cout << "add array list!" << endl;
                        try
                        {
                           mtest.newArrs(num_obj, size_arr);
                        }
                        catch (bad_alloc)
                        {
                           return CMD_EXEC_ERROR;
                        }
                     }
                  }
                  else
                     return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
               }
            }
            else
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
         }
         else if (array_pos == 1)
         {
            if (myStr2Int(options[0], num_obj)) // check options[0]
            {
               if (num_obj <= 0)
                  return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
               else
               {
                  if (myStr2Int(options[2], size_arr))
                  { //check options[0]
                     if (size_arr <= 0)
                        return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
                     else
                     {
                        cout << "add array list!" << endl;
                        try
                        {
                           mtest.newArrs(num_obj, size_arr);
                        }
                        catch (bad_alloc)
                        {
                           return CMD_EXEC_ERROR;
                        }
                     }
                  }
                  else
                     return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
               }
            }
            else
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         }
         else
         { // array_pos == 3
            if (myStr2Int(options[0], num_obj))
            { //check options[0]
               if (num_obj <= 0)
                  return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
               else
                  return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);
            }
            else
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         }
      }
   }

   // Use try-catch to catch the bad_alloc exception
   return CMD_EXEC_DONE;
}

void MTNewCmd::usage(ostream &os) const
{
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void MTNewCmd::help() const
{
   cout << setw(15) << left << "MTNew: "
        << "(memory test) new objects" << endl;
}

//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string &option)
{
   // TODO
   bool index = false;
   bool random = false;
   bool array = false;
   int index_pos;
   int random_pos;
   int array_pos;
   int num;
   int num_random;
   vector<string> options;
   if (!lexOptions(option, options))
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   if (options.empty())
   {
      return CmdExec::errorOption(CMD_OPT_MISSING, "");
   }

   for (size_t i = 0; i < options.size(); ++i)
   {
      if (myStrNCmp("-Index", options[i], 2) == 0)
      {
         index = true;
         index_pos = i;
         break;
      }
   }
   for (size_t i = 0; i < options.size(); ++i)
   {
      if (myStrNCmp("-Random", options[i], 2) == 0)
      {
         random = true;
         random_pos = i;
         break;
      }
   }
   for (size_t i = 0; i < options.size(); ++i)
   {
      if (myStrNCmp("-Array", options[i], 2) == 0)
      {
         array = true;
         array_pos = i;
         break;
      }
   }
   if(!index&&!random&&!array) // if none of -Index -Random -Array exist
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
   if (options.size() == 1)
   {
      if (random || index) // if options[0] is -Index or -Array
         return CmdExec::errorOption(CMD_OPT_MISSING, options[0]);
      else
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
   }
   else if (options.size() == 2)
   {
      // CHECK: options[0] must be "-Random" or "-Index"
      if (myStrNCmp("-Random", options[0], 2) == 0)
      {
         if (myStr2Int(options[1], num_random))
         { // check options[1]
            if (num_random <= 0)
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
            else //legal mtd -r <number>
            {
               cout<<"delete! "<<options[random_pos]<<num_random<<endl;
            }
         }
         else
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
      }
      else if (index_pos==0)
      {
         if (myStr2Int(options[1], num))
         { // check options[1]
            if (num <= 0)
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
            else
            {
               if (mtest.getObjListSize() < num) {
                  cerr << "Size of object list ("<<mtest.getObjListSize()<<") is <= " <<num<<"!!"<< endl;
                  return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
               } else {
                  mtest.deleteObj(num);
               }
            }
         }
         else
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
      }
      else
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
   }
   else if (options.size() == 3)
   {
      
   }
   else
   { // options.size() > 4
   }

   return CMD_EXEC_DONE;
}

void MTDeleteCmd::usage(ostream &os) const
{
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void MTDeleteCmd::help() const
{
   cout << setw(15) << left << "MTDelete: "
        << "(memory test) delete objects" << endl;
}

//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string &option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void MTPrintCmd::usage(ostream &os) const
{
   os << "Usage: MTPrint" << endl;
}

void MTPrintCmd::help() const
{
   cout << setw(15) << left << "MTPrint: "
        << "(memory test) print memory manager info" << endl;
}
