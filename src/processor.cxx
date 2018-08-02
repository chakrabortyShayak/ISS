#include "../header/types.h"
#include "../header/memory.cxx"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include <unordered_map>

using namespace std;

class Processor{
   unsigned int registers[NO_GPRS];
   unordered_map<string, int> reg_map;
   unsigned int pc;
   int load(string dst, string src);
   int store(string dst, string src);
   int add(string dst, string op1, string op2);
 public:
   Processor(){
     string r = "r";
     for(int i=0;i<NO_GPRS;i++){
       string reg = r + to_string(i);
       reg_map[reg] = i;

       registers[i] = 0;
     }
   }
   int run(string program);
};

int Processor::load(string dst, string src){
  int op1,op2;
  try{
    op1 = reg_map.at(dst);
  }catch(...){
    return 1;
  }

  if(op1 < 0 || op1 > NO_GPRS-1) return 1;
  if(src[0] == '#'){
    op2 = stoi(src.substr(1));
    registers[op1] = op2;
  }
  else{
    op2 = stoi(src);
    if(op2 < 0 || op2 > MEM_SIZE-1) return -1;
    registers[op1] = memory[op2];
  }
  return 0;
}

int Processor::add(string s_dst, string s_op1, string s_op2){
  bool is_reg_op1 = (s_op1[0] == 'r')? true : false;
  bool is_reg_op2 = (s_op2[0] == 'r')? true : false;

  unsigned int op1,op2;
  int dst;

  if(is_reg_op1 && is_reg_op2){
    try{
      op1 = reg_map.at(s_op1);
      op2 = reg_map.at(s_op2);
      op1 = registers[op1];
      op2 = registers[op2];
    } catch(...){
      return 1;
    }
  }

  else if(is_reg_op1 && !is_reg_op2){
    try{
      op1 = reg_map.at(s_op1);
      op1 = registers[op1];
    }catch(...){
      return 1;
    }
    if(s_op2[0] == '#'){
      op2 = stoi(s_op2.substr(1));
    }
    else{
      op2 = stoi(s_op2);
      if(op2 < 0 || op2 > MEM_SIZE-1) return -1;
      op2 = memory[op2];
    }
  }

  else if(!is_reg_op1 && is_reg_op2){
    try{
      op2 = reg_map.at(s_op2);
      op2 = registers[op2];
    }catch(...){
      return 1;
    }
    if(s_op1[0] == '#'){
      op2 = stoi(s_op2.substr(1));
    }
    else{
      op2 = stoi(s_op2);
      if(op2 < 0 || op2 > MEM_SIZE-1) return -1;
      op2 = memory[op2];
    }
  }
  else{
    if(s_op1[0] == '#'){
      op1 = stoi(s_op1.substr(1));
    }
    else{
      op1 = stoi(s_op1);
      if(op1 < 0 || op1 > MEM_SIZE-1) return -1;
      op1 = memory[op1];
    }
    if(s_op2[0] == '#'){
      op2 = stoi(s_op2.substr(1));
    }
    else{
      op2 = stoi(s_op2);
      if(op2 < 0 || op2 > MEM_SIZE-1) return -1;
      op2 = memory[op2];
    }
  }

  if(s_dst[0] == 'r'){
    try{
      dst = reg_map.at(s_dst);
    }catch(...){
      return 1;
    }
    cout << registers[dst] << " " << op1 << " " << op2 << endl;
    registers[dst] = op1 + op2;
  }
  else{
    dst = stoi(s_dst);
    if(dst < 0 || dst > MEM_SIZE-1) return -1;
    cout << memory[dst] << " " << op1 << " " << op2 << endl;
    memory[dst] = op1 + op2;
  }
  return 0;
}

int Processor::store(string dst, string src){
  int op1,op2;
  //cout << "hello\n";
  op1 = stoi(dst);
  //cout << "hello\n";
  if(op1 < 0 || op1 > MEM_SIZE-1) return -1;
  if(src[0] == '#'){
    op2 = stoi(src.substr(1));
    memory[op1] = op2;
  }
  else{
    try{
      op2 = reg_map.at(src);
    }catch(...){
      return 1;
    }
    memory[op1] = registers[op2];
  }
  return 0;
}

/******************************************************/
int Processor::run(string program){
  ifstream out ;
  out.open(program, ios::in);
  string instruction;
  string op1,op2;
  int status = 0;

  //cout << "intial memory : " << endl;
  //printMemory();

  while(out >> instruction){
    cout << instruction << endl;
    out >> op1;
    out >> op2;
    if(instruction.compare(ADD) == 0){
      string dst;
      out >> dst;
      cout << op1 << " " << op2 << " " << dst << endl;
      status = add(dst,op1,op2);
    }
    else if(instruction.compare(LD) == 0){
      cout << op1 << " " << op2 << " " << endl;
      status = load(op1,op2);
    }
    else if(instruction.compare(STR) == 0){
      cout << op1 << " " << op2 << " " << endl;
      status = store(op1,op2);
    }
    else{
       cout << "Operation does not exists";
       exit(0);
    }
    if(status != 0){
      if(status == 1) cout << "Illegal Register Access \nAborted!!";
      else cout << "Illegal Memory Access \nAborted!!";
      exit(0);
    }
  }

  cout << "after execution memory :" << endl;
  printMemory();

  out.close();

}

main(int argc, char *args[]){
  Processor p;
  string program = args[1];
  p.run(program);
}
