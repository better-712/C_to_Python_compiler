#include <vector>
#include <unordered_map>

#include "ast.hpp"

namespace SPL {
  using namespace std;
  
  struct Symbol_Type{
    int type;
    string tag="tag";
    int size=0;
    vector<Symbol_Type> parm_type;
  };
  
  class Symbol_Table;
  
  class Symbol {
    public:
        string name;
    //funtion return type 
        Symbol_Type symbol_type;

        int line_no = -1;
    };
  
  class Symbol_Table {
    public:
        Symbol_Table(): next(nullptr) {}

        void set_next(Symbol_Table *next);

        void insert(Symbol *entry);
    
        Symbol* find_symbol(std::string name);
    

        Symbol_Table * next;
    
        unordered_map<string, Symbol *> table;
    
    };

}
