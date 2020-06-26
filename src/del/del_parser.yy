%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {DEL}

%define api.parser.class {DEL_Parser}

%define parse.lac full
%define parse.error verbose

%code requires{
   
   namespace DEL 
   {
      class DEL_Driver;
      class DEL_Scanner;
      class Ast;
      class Element;
      class Function;
      class EncodedDataType;
   }

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { DEL_Scanner  &scanner  }
%parse-param { DEL_Driver  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   #include <stdint.h>
   #include <vector>
   
   #include "Ast/Ast.hpp"
   #include "Ast/Elements.hpp"
   #include "Types/Variables.hpp"

   #include "del_driver.hpp"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%type<DEL::Element*> stmt;
%type<DEL::Element*> assignment;
%type<DEL::Element*> function_stmt;

%type<DEL::Ast*> expression;
%type<DEL::Ast*> term;
%type<DEL::Ast*> factor;
%type<DEL::Ast*> primary;

%type<EncodedDataType*> variable_type;

%type<std::vector<DEL::Element*>> multiple_statements;
%type<std::vector<DEL::Element*>> block;

%type<std::string> identifiers;

%token LEFT_PAREN LEFT_BRACKET ASSIGN VAR

%token DOT COMMA COL 

%token INT DOUBLE STRING NIL OBJECT

%token LSH RSH BW_OR BW_AND BW_XOR AND OR NEGATE  MOD
%token LTE GTE GT LT EQ NE BW_NOT DIV ADD SUB MUL POW

%token <std::string> INT_LITERAL
%token <std::string> HEX_LITERAL
%token <std::string> REAL_LITERAL
%token <std::string> CHAR_LITERAL

%token <std::string> IDENTIFIER
%token <int>         RIGHT_BRACKET  // These tokens encode line numbers
%token <int>         RIGHT_PAREN    // These tokens encode line numbers
%token <int>         SEMI           // These tokens encode line numbers
%token <int>         FUNC           // These tokens encode line numbers

%token               END    0     "end of file"
%locations
%start start

%%

start 
   : END                   { driver.indicate_complete(); }
   | input END             { driver.indicate_complete(); }
   ; 

input
   : function_stmt         { driver.build($1); }
   | input function_stmt   { driver.build($2); }
   ;

identifiers
   :  IDENTIFIER                 { $$ = $1;      } 
   |  IDENTIFIER DOT identifiers { $$ = $1 + "." + $3; }
   ;


expression
   : term                        { $$ = $1;  }
   | expression ADD term         { $$ = new DEL::Ast(DEL::Ast::NodeType::ADD, $1, $3);  }
   | expression SUB term         { $$ = new DEL::Ast(DEL::Ast::NodeType::SUB, $1, $3);  }
   | expression LTE term         { $$ = new DEL::Ast(DEL::Ast::NodeType::LTE, $1, $3);  }
   | expression GTE term         { $$ = new DEL::Ast(DEL::Ast::NodeType::GTE, $1, $3);  }
   | expression GT  term         { $$ = new DEL::Ast(DEL::Ast::NodeType::GT , $1, $3);  }
   | expression LT  term         { $$ = new DEL::Ast(DEL::Ast::NodeType::LT , $1, $3);  }
   | expression EQ  term         { $$ = new DEL::Ast(DEL::Ast::NodeType::EQ , $1, $3);  }
   | expression NE  term         { $$ = new DEL::Ast(DEL::Ast::NodeType::NE , $1, $3);  }
   ;

term
   : factor                      { $$ = $1;  }
   | term MUL factor             { $$ = new DEL::Ast(DEL::Ast::NodeType::MUL,    $1, $3);  }
   | term DIV factor             { $$ = new DEL::Ast(DEL::Ast::NodeType::DIV,    $1, $3);  }
   | term POW factor             { $$ = new DEL::Ast(DEL::Ast::NodeType::POW,    $1, $3);  }
   | term MOD factor             { $$ = new DEL::Ast(DEL::Ast::NodeType::MOD,    $1, $3);  }
   | term LSH factor             { $$ = new DEL::Ast(DEL::Ast::NodeType::LSH,    $1, $3);  }
   | term RSH factor             { $$ = new DEL::Ast(DEL::Ast::NodeType::RSH,    $1, $3);  }
   | term BW_XOR factor          { $$ = new DEL::Ast(DEL::Ast::NodeType::BW_XOR, $1, $3);  }
   | term BW_OR factor           { $$ = new DEL::Ast(DEL::Ast::NodeType::BW_OR,  $1, $3);  }
   | term BW_AND factor          { $$ = new DEL::Ast(DEL::Ast::NodeType::BW_AND, $1, $3);  }
   | term OR factor              { $$ = new DEL::Ast(DEL::Ast::NodeType::OR,     $1, $3);  }
   | term AND factor             { $$ = new DEL::Ast(DEL::Ast::NodeType::AND,    $1, $3);  }
   ;

factor
   : primary                     { $$ = $1; }
   | LEFT_PAREN expression RIGHT_PAREN    { $$ = $2; }
   | BW_NOT factor               { $$ = new DEL::Ast(DEL::Ast::NodeType::BW_NOT, $2, nullptr);}
   | NEGATE factor               { $$ = new DEL::Ast(DEL::Ast::NodeType::NEGATE, $2, nullptr);}
   ;

primary
    : INT_LITERAL                { $$ = new DEL::Ast(DEL::Ast::NodeType::VALUE,      DEL::DataType::INT,       $1, nullptr, nullptr); }
    | REAL_LITERAL               { $$ = new DEL::Ast(DEL::Ast::NodeType::VALUE,      DEL::DataType::DOUBLE,    $1, nullptr, nullptr); }
    | identifiers                { $$ = new DEL::Ast(DEL::Ast::NodeType::IDENTIFIER, DEL::DataType::ID_STRING, $1, nullptr, nullptr); }
    ;

variable_type
   : INT    { $$ = new EncodedDataType(DEL::DataType::INT,    "int"   ); }
   | DOUBLE { $$ = new EncodedDataType(DEL::DataType::DOUBLE, "double"); }
   | STRING { $$ = new EncodedDataType(DEL::DataType::STRING, "string"); }
   | NIL    { $$ = new EncodedDataType(DEL::DataType::NIL,    "nil"   ); }
   | OBJECT LT identifiers GT { $$ = new EncodedDataType(DEL::DataType::USER_DEFINED, $3); }
   ;

assignment
   : VAR identifiers COL variable_type ASSIGN expression SEMI 
      { 
         // Create an assignment. Make a tree with root node as "=", lhs is an identifier node with the var name
         // and the rhs is the expression
         $$ = new DEL::Assignment(
               new DEL::Ast(DEL::Ast::NodeType::ROOT, DEL::DataType::NONE, "=", 
                  new DEL::Ast(DEL::Ast::NodeType::IDENTIFIER, DEL::DataType::ID_STRING, $2, nullptr, nullptr), /* Var name */
                  $6),  /* Expression AST    */
               $4,      /* Encoded Data type */
            $7);        /* Line Number       */
      }
   ;

stmt
   : assignment { $$ = $1; }
   ;

multiple_statements
   : stmt                     { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | multiple_statements stmt { $1.push_back($2); $$ = $1; }
   ;

block 
   : LEFT_BRACKET multiple_statements RIGHT_BRACKET { $$ = $2; }
   | LEFT_BRACKET RIGHT_BRACKET                     { $$ = std::vector<DEL::Element*>(); }
   ;

function_stmt 
   : FUNC identifiers LEFT_PAREN RIGHT_PAREN block { $$ = new DEL::Function($2, std::vector<std::string>(), $5, $1); }
   ;
%%

void DEL::DEL_Parser::error( const location_type &l, const std::string &err_message )
{
   /*
   DEL::Errors       & error_man = driver.get_error_man_ref();
   DEL::Preprocessor & preproc   = driver.get_preproc_ref();

   // Report the error
   error_man.report_syntax_error(
         l.begin.line,                     // Line where issue appeared
         l.begin.column,                   // Column where issue appeared
         err_message,                      // Bison error information
         preproc.fetch_line(l.begin.line)  // The user line where issue appeared
   );
   */
   std::cerr << "Syntax error" << std::endl;
}