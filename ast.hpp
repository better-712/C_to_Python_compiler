//
// Created by 10578 on 10/9/2019.
//

#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <variant>
#include "parser.tab.hpp"
#include "utils.hpp"
#include "scanner.hpp"

/**
 * Abstract syntax tree of SUSTech Programming Language.
 */

namespace SPL {
    /* Class pre-definition */

    class AST_Node;

    class Leaf_Node;

    class Program_Node;

    class ExtDefList_Node;

    class ExtDef_Node;

    class ExtDecList_Node;

    class Specifier_Node;

    class StructSpecifier_Node;

    class VarDec_Node;

    class FunDec_Node;

    class VarList_Node;

    class ParamDec_Node;

    class CompSt_Node;

    class StmtList_Node;

    class Stmt_Node;

    class DefList_Node;

    class Def_Node;

    class DecList_Node;

    class Dec_Node;

    class Exp_Node;

    class Args_Node;

    /* Alias */

    using token_type = SPL_Parser::token_type;
    using token = SPL_Parser::token;

    /**
     * Base AST Node.
     */
    class AST_Node {
    public:
        int line_no = INT32_MAX;

        int propagate_line_no();

        virtual bool is_leaf();

        virtual bool is_empty();

        virtual ~AST_Node() = default;

        virtual std::string to_string() = 0;

        virtual std::vector<AST_Node *> get_child() = 0;
    };

    /**
     * Leaf terminal node.
     */
    class Leaf_Node : public AST_Node {
    public:
        token_type leaf_type;
        Scan_Info *info;
        std::variant<unsigned int, float, char> value;

        Leaf_Node(token_type leaf_type, Scan_Info *info);

        bool is_leaf() override;

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    /* High-level definition */

    class Program_Node : public AST_Node {
    public:
        ExtDefList_Node *ext_def_list;

        explicit Program_Node(ExtDefList_Node *ext_def_list) : ext_def_list{ext_def_list} {
            line_no = propagate_line_no();
        };

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class ExtDefList_Node : public AST_Node {
    public:
        ExtDef_Node *ext_def;
        ExtDefList_Node *ext_def_list;

        ExtDefList_Node(ExtDef_Node *ext_def, ExtDefList_Node *ext_def_list)
                : ext_def{ext_def}, ext_def_list{ext_def_list} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class Empty_ExtDefList_Node : public ExtDefList_Node {
    public:
        Empty_ExtDefList_Node() : ExtDefList_Node(nullptr, nullptr) {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class ExtDef_Node : public AST_Node {
    public:
        Specifier_Node *specifier;
        ExtDecList_Node *ext_dec_list;
        Leaf_Node *semi;

        FunDec_Node *fun_dec;
        CompSt_Node *comp_st;

        ExtDef_Node(Specifier_Node *specifier, ExtDecList_Node *ext_dec_list, Leaf_Node *semi)
                : specifier{specifier}, ext_dec_list{ext_dec_list}, semi{semi}, fun_dec{nullptr},
                  comp_st{nullptr} {};

        ExtDef_Node(Specifier_Node *specifier, Leaf_Node *semi)
                : specifier{specifier}, ext_dec_list{nullptr}, semi{semi}, fun_dec{nullptr},
                  comp_st{nullptr} {};

        ExtDef_Node(Specifier_Node *specifier, FunDec_Node *fun_dec, CompSt_Node *comp_st)
                : specifier{specifier}, ext_dec_list{nullptr}, semi{nullptr}, fun_dec{fun_dec},
                  comp_st{comp_st} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class ExtDecList_Node : public AST_Node {
    public:
        VarDec_Node *var_dec;
        Leaf_Node *comma;
        ExtDecList_Node *ext_dec_list;

        explicit ExtDecList_Node(VarDec_Node *var_dec, Leaf_Node *comma = nullptr,
                                 ExtDecList_Node *ext_dec_list = nullptr)
                : var_dec{var_dec}, comma{comma}, ext_dec_list{ext_dec_list} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    /* specifier */

    class Specifier_Node : public AST_Node {
    public:
        Leaf_Node *type;
        StructSpecifier_Node *struct_specifier;

        explicit Specifier_Node(Leaf_Node *type) : type{type}, struct_specifier{nullptr} {};

        explicit Specifier_Node(StructSpecifier_Node *struct_specifier) : type{nullptr},
                                                                          struct_specifier{struct_specifier} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class StructSpecifier_Node : public AST_Node {
    public:
        Leaf_Node *_struct;
        Leaf_Node *id;
        Leaf_Node *lc;
        DefList_Node *def_list;
        Leaf_Node *rc;

        StructSpecifier_Node(Leaf_Node *_struct, Leaf_Node *id,
                             Leaf_Node *lc = nullptr, DefList_Node *def_list = nullptr, Leaf_Node *rc = nullptr)
                : _struct{_struct}, id{id}, lc{lc}, def_list{def_list}, rc{rc} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };


    /* declarator */

    class VarDec_Node : public AST_Node {
    public:
        std::string to_string() override;
    };

    class ID_VarDec_Node : public VarDec_Node {
    public:
        Leaf_Node *id;

        explicit ID_VarDec_Node(Leaf_Node *id) : id{id} {};

        std::vector<AST_Node *> get_child() override;
    };

    class Array_VarDec_Node : public VarDec_Node {
    public:
        VarDec_Node *var_dec;
        Leaf_Node *lb;
        Leaf_Node *_int;
        Leaf_Node *rb;

        explicit Array_VarDec_Node(VarDec_Node *var_dec, Leaf_Node *lb, Leaf_Node *_int, Leaf_Node *rb)
                : var_dec{var_dec}, lb{lb}, _int{_int}, rb{rb} {};

        std::vector<AST_Node *> get_child() override;
    };

    class FunDec_Node : public AST_Node {
    public:
        Leaf_Node *id;
        Leaf_Node *lp;
        VarList_Node *var_list;
        Leaf_Node *rp;

        FunDec_Node(Leaf_Node *id, Leaf_Node *lp, VarList_Node *var_list, Leaf_Node *rp)
                : id{id}, lp{lp}, var_list{var_list}, rp{rp} {};

        FunDec_Node(Leaf_Node *id, Leaf_Node *lp, Leaf_Node *rp)
                : id{id}, lp{lp}, var_list{nullptr}, rp{rp} {};

        std::vector<AST_Node *> get_child() override;

        std::string to_string() override;
    };

    class VarList_Node : public AST_Node {
    public:
        ParamDec_Node *param_dec;
        Leaf_Node *comma;
        VarList_Node *var_list;

        explicit VarList_Node(ParamDec_Node *param_dec, Leaf_Node *comma = nullptr, VarList_Node *var_list = nullptr)
                : param_dec{param_dec}, comma{comma}, var_list{var_list} {};

        std::vector<AST_Node *> get_child() override;

        std::string to_string() override;
    };

    class ParamDec_Node : public AST_Node {
    public:
        Specifier_Node *specifier;
        VarDec_Node *var_dec;

        ParamDec_Node(Specifier_Node *specifier, VarDec_Node *var_dec)
                : specifier{specifier}, var_dec{var_dec} {};

        std::vector<AST_Node *> get_child() override;

        std::string to_string() override;
    };

    /* statement */

    class CompSt_Node : public AST_Node {
    public:
        Leaf_Node *lc;
        DefList_Node *def_list;
        StmtList_Node *stmt_list;
        Leaf_Node *rc;

        CompSt_Node(Leaf_Node *lc, DefList_Node *def_list, StmtList_Node *stmt_list, Leaf_Node *rc)
                : lc{lc}, def_list{def_list}, stmt_list{stmt_list}, rc{rc} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class StmtList_Node : public AST_Node {
    public:
        Stmt_Node *stmt;
        StmtList_Node *stmt_list;

        explicit StmtList_Node(Stmt_Node *stmt, StmtList_Node *stmt_list)
                : stmt{stmt}, stmt_list{stmt_list} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class Empty_StmtList_Node : public StmtList_Node {
    public:
        Empty_StmtList_Node() : StmtList_Node(nullptr, nullptr) {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class Stmt_Node : public AST_Node {
    public:
        std::string to_string() override;
    };

    class Exp_Stmt_Node : public Stmt_Node {
    public:
        Exp_Node *exp;
        Leaf_Node *semi;

        Exp_Stmt_Node(Exp_Node *exp, Leaf_Node *semi) : exp{exp}, semi{semi} {};

        std::vector<AST_Node *> get_child() override;
    };

    class CompSt_Stmt_Node : public Stmt_Node {
    public:
        CompSt_Node *comp_st;

        explicit CompSt_Stmt_Node(CompSt_Node *comp_st) : comp_st{comp_st} {};

        std::vector<AST_Node *> get_child() override;
    };

    class Return_Stmt_Node : public Stmt_Node {
    public:
        Leaf_Node *ret;
        Exp_Node *exp;
        Leaf_Node *semi;

        explicit Return_Stmt_Node(Leaf_Node *ret, Exp_Node *exp, Leaf_Node *semi)
                : ret{ret}, exp{exp}, semi{semi} {};

        std::vector<AST_Node *> get_child() override;
    };

    class If_Stmt_Node : public Stmt_Node {
    public:
        Leaf_Node *_if;
        Leaf_Node *lp;
        Exp_Node *exp;
        Leaf_Node *rp;
        Stmt_Node *stmt_if;

        Leaf_Node *_else;
        Stmt_Node *stmt_else;

        explicit If_Stmt_Node(Leaf_Node *if_, Leaf_Node *lp, Exp_Node *exp, Leaf_Node *rp, Stmt_Node *stmt_if,
                              Leaf_Node *_else = nullptr, Stmt_Node *stmt_else = nullptr)
                : _if{if_}, lp{lp}, exp{exp},
                  rp{rp}, stmt_if{stmt_if},
                  _else{_else},
                  stmt_else{stmt_else} {};

        std::vector<AST_Node *> get_child() override;
    };

    class While_Stmt_Node : public Stmt_Node {
    public:
        Leaf_Node *_while;
        Leaf_Node *lp;
        Exp_Node *exp;
        Leaf_Node *rp;
        Stmt_Node *stmt;

        explicit While_Stmt_Node(Leaf_Node *_while, Leaf_Node *lp, Exp_Node *exp, Leaf_Node *rp, Stmt_Node *stmt)
                : _while{_while}, lp{lp}, exp{exp}, rp{rp}, stmt{stmt} {};

        std::vector<AST_Node *> get_child() override;
    };


    /* local definition */

    class DefList_Node : public AST_Node {
    public:
        Def_Node *def;
        DefList_Node *def_list;

        DefList_Node(Def_Node *def, DefList_Node *def_list) : def{def}, def_list{def_list} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class Empty_DefList_Node : public DefList_Node {
    public:
        Empty_DefList_Node() : DefList_Node(nullptr, nullptr) {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class Def_Node : public AST_Node {
    public:
        Specifier_Node *specifier;
        DecList_Node *dec_list;
        Leaf_Node *semi;

        Def_Node(Specifier_Node *specifier, DecList_Node *dec_list, Leaf_Node *semi) : specifier{specifier},
                                                                                       dec_list{dec_list},
                                                                                       semi{semi} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class DecList_Node : public AST_Node {
    public:
        Dec_Node *dec;
        Leaf_Node *comma;
        DecList_Node *dec_list;

        explicit DecList_Node(Dec_Node *dec, Leaf_Node *comma = nullptr, DecList_Node *dec_list = nullptr)
                : dec{dec}, comma{comma}, dec_list{dec_list} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class Empty_DecList_Node : public DecList_Node {
    public:
        Empty_DecList_Node() : DecList_Node(nullptr, nullptr) {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    class Dec_Node : public AST_Node {
    public:
        VarDec_Node *var_dec;
        Leaf_Node *assign;
        Exp_Node *exp;

        explicit Dec_Node(VarDec_Node *var_dec, Leaf_Node *assign = nullptr, Exp_Node *exp = nullptr)
                : var_dec{var_dec}, assign{assign}, exp{exp} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    /* Expression */

    class Exp_Node : public AST_Node {
    public:
        std::string to_string() override;
    };

    class Parentheses_Exp_Node : public Exp_Node {
    public:
        Leaf_Node *lp;
        Exp_Node *exp;
        Leaf_Node *rp;

        Parentheses_Exp_Node(Leaf_Node *lp, Exp_Node *exp, Leaf_Node *rp) : lp{lp}, exp{exp}, rp{rp} {};

        std::vector<AST_Node *> get_child() override;
    };

    class ID_Parentheses_Exp_Node : public Exp_Node {
    public:
        Leaf_Node *id;
        Leaf_Node *lp;
        Args_Node *args;
        Leaf_Node *rp;

        ID_Parentheses_Exp_Node(Leaf_Node *id, Leaf_Node *lp, Args_Node *args, Leaf_Node *rp)
                : id{id}, lp{lp}, args{args}, rp{rp} {};

        ID_Parentheses_Exp_Node(Leaf_Node *id, Leaf_Node *lp, Leaf_Node *rp)
                : id{id}, lp{lp}, args{nullptr}, rp{rp} {};

        std::vector<AST_Node *> get_child() override;
    };

    class Bracket_Exp_Node : public Exp_Node {
    public:
        Exp_Node *exp1;
        Leaf_Node *lb;
        Exp_Node *exp2;
        Leaf_Node *rb;

        Bracket_Exp_Node(Exp_Node *exp1, Leaf_Node *lb, Exp_Node *exp2, Leaf_Node *rb)
                : exp1{exp1}, lb{lb}, exp2{exp2}, rb{rb} {};

        std::vector<AST_Node *> get_child() override;
    };

    class Dot_Exp_Node : public Exp_Node {
    public:
        Exp_Node *exp;
        Leaf_Node *dot;
        Leaf_Node *id;

        Dot_Exp_Node(Exp_Node *exp, Leaf_Node *dot, Leaf_Node *id) : exp{exp}, dot{dot}, id{id} {};

        std::vector<AST_Node *> get_child() override;
    };

    class Binary_Exp_Node : public Exp_Node {
    public:
        Exp_Node *left;
        Leaf_Node *op_node;
        Exp_Node *right;

        Binary_Exp_Node(Exp_Node *left, Leaf_Node *op_type, Exp_Node *right)
                : op_node{op_type}, left{left}, right{right} {};

        std::vector<AST_Node *> get_child() override;
    };

    class Unary_Exp_Node : public Exp_Node {
    public:
        Leaf_Node *op_node;
        Exp_Node *node;

        Unary_Exp_Node(Leaf_Node *op_node, Exp_Node *node) : op_node{op_node}, node{node} {};

        std::vector<AST_Node *> get_child() override;
    };

    class Leaf_Exp_Node : public Exp_Node {
    public:
        Leaf_Node *leaf;

        explicit Leaf_Exp_Node(Leaf_Node *leaf) : leaf{leaf} {};

        std::vector<AST_Node *> get_child() override;
    };

    class Args_Node : public AST_Node {
    public:
        Exp_Node *exp;
        Leaf_Node *comma;
        Args_Node *args;

        explicit Args_Node(Exp_Node *exp, Leaf_Node *comma = nullptr, Args_Node *args = nullptr)
                : exp{exp}, comma{comma}, args{args} {};

        std::string to_string() override;

        std::vector<AST_Node *> get_child() override;
    };

    /* Useful Function */

    Leaf_Node *make_leaf(token_type leaf_type, Scan_Info *info);

    Leaf_Node *make_leaf(token_type leaf_type, std::string lexeme, int line_no);

    void print_ast(AST_Node *node, int indent_level = 0);
}


#endif //__AST_HPP__
