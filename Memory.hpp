#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <unordered_map>
#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <iomanip>

using namespace std;

struct Id {
  int memory_loc;      // ex : 5000
  string type;         // ex: bool, string
};


class SymbolTable
{
    private:
      //SymbolTable() {};
      unordered_map<string, Id> table;

    public:
      SymbolTable() {};
      void add_to_table(int memory_location, string identifier, string type) {
        if (!exists_in_table(identifier)) {
          table[identifier] = {memory_location, type};
        } else {
          cout << "Error: Identifier already in Symbol Table" << endl;
        }
        //mapcontainer_name[ key name ] = mapped value;
      }
      bool exists_in_table(string identifier){
        if(table.find(identifier) != table.end()){
          //(table.count(identifier)>0)
          return true;
        }
        else{
          return false;
        }
      }
      unordered_map<string, Id> getTable() {
        return table;
      }
      void print_table() {
        unordered_map<string, Id>::iterator it;
        cout << "==========================================================" << endl;
        cout << "Symbol Table\n";
        cout << "==========================================================" << endl;
        cout << left << setw(14) << "Identifier" << setw(22) 
             << "Memory Location" << setw(18) << "Type" << endl;
        for (auto entry : table) {
          cout << left << setw(18) <<entry.first 
               << setw(18) << entry.second.memory_loc 
               << setw(18) << entry.second.type << endl;
        }
      }
};


class Memory
{
  private:
    unordered_map<unsigned int, int> mem;

  public:
    int get_value_at_address(int address);
    bool update_value_at_address(int address, int value);
    bool set_value_at_address(int address, int value);
};

#endif


//className objectName(parameters);
//Car myCar("Honda"); //passing in Honda into the make variable
