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
      class Parameter;
      class Call;
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
%type<DEL::Element*> return_stmt;
%type<DEL::Element*> function_stmt;
%type<DEL::Element*> direct_function_call;

%type<DEL::Element*> if_stmt;
%type<DEL::Element*> elif_stmt;
%type<DEL::Element*> else_stmt;

%type<DEL::Element*> loops_only_stmts;
%type<DEL::Element*> named_loops_only_stmts;
%type<DEL::Element*> loop_stmt;

%type<DEL::Ast*> expression;
%type<DEL::Ast*> assignment_allowed_expression;
%type<DEL::Ast*> string_expr;
%type<DEL::Ast*> term;
%type<DEL::Ast*> factor;
%type<DEL::Ast*> primary;
%type<DEL::Ast*> expr_function_call;

%type<EncodedDataType*> assignable_type;
%type<EncodedDataType*> returnable_type;

%type<std::vector<DEL::Element*>> multiple_statements;
%type<std::vector<DEL::Element*>> multiple_loop_statements;
%type<std::vector<DEL::Element*>> multiple_named_loop_statements;
%type<std::vector<DEL::Element*>> block;
%type<std::vector<DEL::Element*>> loop_block;
%type<std::vector<DEL::Element*>> named_loop_block;

%type<Parameter*> single_func_param;
%type<std::vector<DEL::Parameter*>> function_params;

%type<Parameter*> single_call_param;
%type<std::vector<DEL::Parameter*>> call_params;

%type<std::string> identifiers;

%token LEFT_PAREN LEFT_BRACKET ASSIGN VAR LET

%token DOT COMMA COL ARROW RETURN REF

%token INT DOUBLE STRING NIL

%token LSH RSH BW_OR BW_AND BW_XOR AND OR NEGATE  MOD
%token LTE GTE GT LT EQ NE BW_NOT DIV ADD SUB MUL POW

%token IF ELIF

%token WHILE CONT FOR LOOP BREAK;

%token TYPE VALUE INDEX DEST 

%token <std::string> INT_LITERAL
%token <std::string> HEX_LITERAL
%token <std::string> DOUB_LITERAL
%token <std::string> CHAR_LITERAL
%token <std::string> STRING_LITERAL

%token <std::string> IDENTIFIER
%token <int>         RIGHT_BRACKET  // These tokens encode line numbers
%token <int>         RIGHT_PAREN    // These tokens encode line numbers
%token <int>         SEMI           // These tokens encode line numbers
%token <int>         FUNC           // These tokens encode line numbers
%token <int>         ELSE           // These tokens encode line numbers
%token <int>         KEY            // These tokens encode line numbers
%token <int>         UNIT           // These tokens encode line numbers

%token               END    0     "end of file"
%locations
%start start

%%

start 
   : END                   { driver.indicate_complete(); }
   | input END             { driver.indicate_complete(); }
   ; 

input
   : function_stmt            { driver.build($1); }
   | input function_stmt      { driver.build($2); }
   ;

identifiers
   :  IDENTIFIER                 { $$ = $1;      } 
   |  IDENTIFIER DOT identifiers { $$ = $1 + "." + $3; }
   ;

string_expr
   : STRING_LITERAL                 { $$ = new DEL::Ast(DEL::Ast::NodeType::VALUE, DEL::DataType::STRING, $1, nullptr, nullptr);  }
   | string_expr ADD STRING_LITERAL { $$ = new DEL::Ast(DEL::Ast::NodeType::ADD, 
                                                        $1, 
                                                        new DEL::Ast(DEL::Ast::NodeType::VALUE, 
                                                                     DEL::DataType::STRING, 
                                                                     $3, 
                                                                     nullptr, 
                                                                     nullptr)
                                                       ); 
                                    }
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
   | expr_function_call          { $$ = $1; }
   | LEFT_PAREN expression RIGHT_PAREN    { $$ = $2; }
   | BW_NOT factor               { $$ = new DEL::Ast(DEL::Ast::NodeType::BW_NOT, $2, nullptr);}
   | NEGATE factor               { $$ = new DEL::Ast(DEL::Ast::NodeType::NEGATE, $2, nullptr);}
   ;

primary
    : INT_LITERAL                { $$ = new DEL::Ast(DEL::Ast::NodeType::VALUE,      DEL::DataType::INT,       $1, nullptr, nullptr); }
    | DOUB_LITERAL               { $$ = new DEL::Ast(DEL::Ast::NodeType::VALUE,      DEL::DataType::DOUBLE,    $1, nullptr, nullptr); }
    | identifiers                { $$ = new DEL::Ast(DEL::Ast::NodeType::IDENTIFIER, DEL::DataType::ID_STRING, $1, nullptr, nullptr); }
    ;
    
assignable_type
   : INT    { $$ = new EncodedDataType(DEL::DataType::INT,    "int"   ); }
   | DOUBLE { $$ = new EncodedDataType(DEL::DataType::DOUBLE, "double"); }
   | STRING { $$ = new EncodedDataType(DEL::DataType::STRING, "string"); }
   ;

returnable_type
   : assignable_type          { $$ = $1; }
   | NIL                      { $$ = new EncodedDataType(DEL::DataType::NIL,    "nil"   ); }
   ;

assignment_allowed_expression
   : expression
   | string_expr
   ;

assignment
   : VAR identifiers COL assignable_type ASSIGN assignment_allowed_expression SEMI 
      { 
         // Create an assignment. Make a tree with root node as "=", lhs is an identifier node with the var name
         // and the rhs is the expression
         $$ = new DEL::Assignment(
               false, /* Not immutable */
               new DEL::Ast(DEL::Ast::NodeType::ROOT, DEL::DataType::NONE, "=", 
                  new DEL::Ast(DEL::Ast::NodeType::IDENTIFIER, DEL::DataType::ID_STRING, $2, nullptr, nullptr), /* Var name */
                  $6),  /* Expression AST    */
               $4,      /* Encoded Data type */
            $7);        /* Line Number       */
      }
   | LET identifiers COL assignable_type ASSIGN assignment_allowed_expression SEMI 
      { 
         // Create an assignment. Make a tree with root node as "=", lhs is an identifier node with the var name
         // and the rhs is the expression
         $$ = new DEL::Assignment(
               true, /* Is immutable */
               new DEL::Ast(DEL::Ast::NodeType::ROOT, DEL::DataType::NONE, "=", 
                  new DEL::Ast(DEL::Ast::NodeType::IDENTIFIER, DEL::DataType::ID_STRING, $2, nullptr, nullptr), /* Var name */
                  $6),  /* Expression AST    */
               $4,      /* Encoded Data type */
            $7);        /* Line Number       */
      }
   | identifiers ASSIGN assignment_allowed_expression SEMI
      {
         $$ = new DEL::Reassignment(
            new DEL::Ast(DEL::Ast::NodeType::ROOT, DEL::DataType::NONE, "=", 
               new DEL::Ast(DEL::Ast::NodeType::IDENTIFIER, DEL::DataType::ID_STRING, $1, nullptr, nullptr), /* Var name */
                  $3),  /* Expression AST    */
            $4); /* Line Number */
      }
   ;

return_stmt
   : RETURN SEMI                               { $$ = new DEL::Return(nullptr, $2); }
   | RETURN assignment_allowed_expression SEMI { $$ = new DEL::Return($2, $3);      }
   ;

stmt
   : assignment           { $$ = $1; }
   | if_stmt              { $$ = $1; }
   | return_stmt          { $$ = $1; }
   | loop_stmt            { $$ = $1; }
   | direct_function_call { $$ = $1; }
   ;

multiple_statements
   : stmt                     { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | multiple_statements stmt { $1.push_back($2); $$ = $1; }
   ;

block 
   : LEFT_BRACKET multiple_statements RIGHT_BRACKET { $$ = $2; }
   | LEFT_BRACKET RIGHT_BRACKET                     { $$ = std::vector<DEL::Element*>(); }
   ;

// 
// ------------------------- Function Statements ------------------------
// 

single_func_param
   : INT    COL identifiers { $$ = new Parameter(DEL::DataType::INT,    $3, "int"   , false); }
   | DOUBLE COL identifiers { $$ = new Parameter(DEL::DataType::DOUBLE, $3, "double", false); }
   | STRING COL identifiers { $$ = new Parameter(DEL::DataType::STRING, $3, "string", false); }
   | REF INT    COL identifiers { $$ = new Parameter(DEL::DataType::INT,    $4, "int"   , true); }
   | REF DOUBLE COL identifiers { $$ = new Parameter(DEL::DataType::DOUBLE, $4, "double", true); }
   | REF STRING COL identifiers { $$ = new Parameter(DEL::DataType::STRING, $4, "string", true); }
   ;

function_params
   : single_func_param { $$ = std::vector<DEL::Parameter*>(); $$.push_back($1); }
   | function_params COMMA single_func_param { $1.push_back($3); $$ = $1; }
   ;

function_stmt 
   : FUNC identifiers LEFT_PAREN RIGHT_PAREN ARROW returnable_type block { $$ = new DEL::Function($2, std::vector<DEL::Parameter*>(), $7, $6, $1); }
   | FUNC identifiers LEFT_PAREN function_params RIGHT_PAREN ARROW returnable_type block 
      { 
         $$ = new DEL::Function($2, $4, $8, $7, $1); 
      }
   ;

// 
// --------------------------- Call Statements --------------------------
// 

single_call_param
   : identifiers     { $$ = new Parameter(DEL::DataType::ID_STRING, $1, "unknown"     , false); }
   | REF identifiers { $$ = new Parameter(DEL::DataType::ID_STRING, $2, "unknown ref" , true);  }
   | INT_LITERAL     { $$ = new Parameter(DEL::DataType::INT,       $1, "int literal" , false); }
   | DOUB_LITERAL    { $$ = new Parameter(DEL::DataType::DOUBLE,    $1, "doub literal", false); }
   | STRING_LITERAL  { $$ = new Parameter(DEL::DataType::STRING,    $1, "str literal" , false); }
   ;

call_params
   : single_call_param { $$ = std::vector<DEL::Parameter*>(); $$.push_back($1); }
   | call_params COMMA single_call_param { $1.push_back($3); $$ = $1; }
   ;

// Returns call as an AST 
expr_function_call
   : identifiers LEFT_PAREN RIGHT_PAREN             { $$ = new DEL::Call($1, std::vector<DEL::Parameter*>(), $3 ); }
   | identifiers LEFT_PAREN call_params RIGHT_PAREN { $$ = new DEL::Call($1, $3, $4); }
   ;

// Returns call as an Element
direct_function_call
   : identifiers LEFT_PAREN RIGHT_PAREN SEMI             { $$ = new DEL::Call($1, std::vector<DEL::Parameter*>(), $3 ); }
   | identifiers LEFT_PAREN call_params RIGHT_PAREN SEMI { $$ = new DEL::Call($1, $3, $4); }
   ;

// 
// ----------------------- Conditional Statements -----------------------
// 

if_stmt
   : IF LEFT_PAREN expression RIGHT_PAREN block elif_stmt { $$ = new DEL::If(If::Type::IF, $3, $5, $6, $4); }
   | IF LEFT_PAREN expression RIGHT_PAREN block else_stmt { $$ = new DEL::If(If::Type::IF, $3, $5, $6, $4); }
   | IF LEFT_PAREN expression RIGHT_PAREN block           { $$ = new DEL::If(If::Type::IF, $3, $5, nullptr, $4); }
   ;

elif_stmt
   : ELIF LEFT_PAREN expression RIGHT_PAREN block elif_stmt  { $$ = new DEL::If(If::Type::ELIF, $3, $5, $6, $4); }
   | ELIF LEFT_PAREN expression RIGHT_PAREN block else_stmt  { $$ = new DEL::If(If::Type::ELIF, $3, $5, $6, $4); }
   | ELIF LEFT_PAREN expression RIGHT_PAREN block            { $$ = new DEL::If(If::Type::ELIF, $3, $5, nullptr, $4); }
   ;

else_stmt
   : ELSE block   { $$ = new DEL::If(If::Type::ELSE, 
                                     new DEL::Ast(DEL::Ast::NodeType::VALUE, DEL::DataType::INT, "1", nullptr, nullptr),
                                     $2,
                                     nullptr,
                                     $1);

                     // We create an "always true" statement so we can leverage elseif code, while still
                     // treating this as an "else"
                  }
   ;

// 
// --------------------------- Loop Statements --------------------------
// 

loops_only_stmts
   : CONT SEMI { $$ = new DEL::Continue($2); }
   ;

named_loops_only_stmts
   : BREAK identifiers SEMI { $$ = new DEL::Break($2, $3); }
   ;

multiple_loop_statements
   : stmt                                 { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | loops_only_stmts                     { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | multiple_loop_statements stmt             { $1.push_back($2); $$ = $1; }
   | multiple_loop_statements loops_only_stmts { $1.push_back($2); $$ = $1; }
   ;

multiple_named_loop_statements
   : stmt                                 { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | named_loops_only_stmts               { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | loops_only_stmts                     { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | multiple_named_loop_statements stmt             { $1.push_back($2); $$ = $1; }
   | multiple_named_loop_statements loops_only_stmts { $1.push_back($2); $$ = $1; }
   | multiple_named_loop_statements named_loops_only_stmts { $1.push_back($2); $$ = $1; }
   ;

loop_block 
   : LEFT_BRACKET multiple_loop_statements RIGHT_BRACKET { $$ = $2; }
   | LEFT_BRACKET RIGHT_BRACKET                     { $$ = std::vector<DEL::Element*>(); }
   ;

named_loop_block 
   : LEFT_BRACKET multiple_named_loop_statements RIGHT_BRACKET { $$ = $2; }
   | LEFT_BRACKET RIGHT_BRACKET                     { $$ = std::vector<DEL::Element*>(); }
   ;

loop_stmt
   : WHILE LEFT_PAREN expression RIGHT_PAREN loop_block 
      { 
         $$ = new DEL::WhileLoop($3, $5, $4); 
      }
   | FOR LEFT_PAREN assignment expression SEMI assignment RIGHT_PAREN loop_block 
      {
         $$ = new DEL::ForLoop($3, $4, $6, $8, $7);
      }
   | LOOP KEY identifiers named_loop_block 
      {
         $$ = new DEL::NamedLoop($3, $4, $2);
      }
   ;

%%

void DEL::DEL_Parser::error( const location_type &l, const std::string &err_message )
{
   driver.code_forge.get_reporter().issue_report(
            new FORGE::SyntaxReport(
               FORGE::Report::Level::ERROR, 
               driver.current_file_from_directive,
               driver.preprocessor.fetch_user_line_number(l.begin.line),
               l.begin.column,
               driver.preprocessor.fetch_line(l.begin.line))
        );
}