#include <vector>
#include <unordered_map>

#include "ast.hpp"

#include "symbol.hpp"

namespace SPL {
  using namespace std;
  void Symbol_Table::insert(Symbol *entry) {
    cout << "insert! " << endl;
    string name = entry->name;
    if (find_symbol(name)!=nullptr) {
            cout << "Semantic Error! " << name << " has been in symbol table at line: " << entry->line_no<< endl;
    } else {
            table[name] = entry;
    }
    
  }
  
  Symbol* Symbol_Table::find_symbol(std::string name) {
    auto it=table.find(name) ;
    if(it!= table.end())
      return it->second;
    else
      return nullptr;    
    }
  void Symbol_Table::print_table(){
    for (auto it = table.begin(); it != table.end(); ++it) {
        std::cout << "name: " << it->first << ", type: " << it->second->symbol_type.type << std::endl;
    }
  }
}
