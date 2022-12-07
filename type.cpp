#include <iostream>
#include <fstream>
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
        std::map<std::string, Type *> members;
        Struct_Type(std::string name, std::vector<std::pair<std::string, Type *>> member_vector){
            
        }
   
    };
}
