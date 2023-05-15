#include <vector>
#include <unordered_map>
#include "utils.hpp"
#include "type.hpp"
#include "ast.hpp"

namespace SPL {
  
  class Symbol_Table;
  
  class Symbol {
    public:
        std::string name;
    //funtion return type 
        int type;
    
    //fun or struct
        vector<int> parm_type;

        int line_no = -1;
    };
  
  class Symbol_Table {
    public:
        Symbol_Table(): next(nullptr),size(0) {}

        void set_next(Symbol_Table *next);

        void insert(Symbol *entry);

        std::Symbol_Table * next;
    
        std::unordered_map<std::string, Symbol *> table;
    
    };

}
