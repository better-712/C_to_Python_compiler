#include <vector>
#include <unordered_map>

#include "ast.hpp"

#include "symbol.hpp"

namespace SPL {
  using namespace std;
  void Symbol_Table::insert(Symbol *entry) {
    cout << "insert! " << endl;
    string name = entry->name;
    if (find_symbol(name)==nullptr) {
            cout << "Semantic Error! " << name << " has been in symbol table at line: " << entry->line_no<< endl;
    } else {
            table[name] = entry;
    }
    
  }
  
  Symbol* Symbol_Table::find_symbol(std::string name) {
    if(table.find(id) != table.end())
      return *(table.find(id));
    else
      return nullptr;    
    }
  
}
