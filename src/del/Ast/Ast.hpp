#ifndef DEL_AST_HPP
#define DEL_AST_HPP

#include <string>
#include "Types/Variables.hpp"

namespace DEL
{

    class Ast
    {
    public:

        //! \brief The node types
        enum class NodeType
        {
            ROOT, IDENTIFIER, VALUE,
            ADD, SUB, LTE, GTE, 
            GT , LT , EQ , NE , 
            MUL, DIV, POW, MOD,
            LSH, RSH, BW_XOR, BW_OR,  
            BW_AND, OR, AND, BW_NOT, NEGATE,
            CALL
        };

        //! \brief Ast Node
        class Node
        {
        public: 
            Node(NodeType node_type, FORGE::DataType data_type, std::string data) : 
                    node_type(node_type), data_type(data_type), data(data){}
            NodeType node_type;
            FORGE::DataType data_type;
            std::string data;
        };

        //! \brief AST Construct - Root
        Ast() : node(NodeType::ROOT, FORGE::DataType::UNDEFINED, ""), left(nullptr), right(nullptr) {}

        //! \brief AST Construct - Tree 
        Ast(NodeType node_type, FORGE::DataType data_type, std::string data, Ast * left, Ast * right) : 
            node(node_type, data_type, data), left(left), right(right) {} 


        Ast(NodeType node_type, Ast * left, Ast * right) : 
            node(node_type, FORGE::DataType::UNDEFINED, ""), left(left), right(right){} 

        ~Ast(){ if(left)  { delete left; } 
                if(right) { delete right;} }

        Node node;

        Ast * left;
        Ast * right;
    };
}


#endif