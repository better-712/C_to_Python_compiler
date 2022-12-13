#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
namespace SPL{
  class Type {
    public:
//         int line_no = -1;
        std::string name;
    };
    class Primitive_Type : public Type {
    public:
        Primitive_Type(std::string name){
            this->name=name;
        }
    };
    
    class Struct_Type : public Type {
    public:
        std::unordered_map<std::string, Type *> members;
        Struct_Type(std::vector<std::pair<std::string, Type *>> member_vector){
          this->name="Struct";
          for (auto &p: member_vector) 
            members.insert(p);
        }
   
    };
}
