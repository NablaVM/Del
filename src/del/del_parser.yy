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

%type<DEL::Element*> object_assignment;
%type<DEL::Element*> object_definition;
%type<DEL::Element*> object_stmt;
%type<DEL::Element*> member_definition;

%type<DEL::Element*> if_stmt;
%type<DEL::Element*> elif_stmt;
%type<DEL::Element*> else_stmt;

%type<DEL::Element*> loops_only_stmts;
%type<DEL::Element*> named_loops_only_stmts;
%type<DEL::Element*> loop_stmt;

%type<DEL::Element*> dyn_stmt;
%type<DEL::Element*> unit_space;

%type<DEL::Ast*> expression;
%type<DEL::Ast*> assignment_allowed_expression;
%type<DEL::Ast*> string_expr;
%type<DEL::Ast*> term;
%type<DEL::Ast*> factor;
%type<DEL::Ast*> primary;
%type<DEL::Ast*> expr_function_call;

%type<EncodedDataType*> assignable_type;
%type<EncodedDataType*> returnable_type;

%type<std::vector<DEL::Element*>> multiple_object_stmts;
%type<std::vector<DEL::Element*>> multiple_statements;
%type<std::vector<DEL::Element*>> multiple_assignments;
%type<std::vector<DEL::Element*>> multiple_loop_statements;
%type<std::vector<DEL::Element*>> multiple_named_loop_statements;
%type<std::vector<DEL::Element*>> object_instantiation;
%type<std::vector<DEL::Element*>> block;
%type<std::vector<DEL::Element*>> loop_block;
%type<std::vector<DEL::Element*>> named_loop_block;
%type<std::vector<DEL::Element*>> object_block;

%type<std::vector<DEL::Element*>> unit_space_multiple_stmts;
%type<std::vector<DEL::Element*>> unit_block;

%type<Parameter*> single_func_param;
%type<std::vector<DEL::Parameter*>> function_params;

%type<Parameter*> single_call_param;
%type<std::vector<DEL::Parameter*>> call_params;

%type<std::string> identifiers;

%token LEFT_PAREN LEFT_BRACKET ASSIGN VAR LET

%token DOT COMMA COL ARROW RETURN PUB PRIV REF

%token INT DOUBLE STRING NIL

%token LSH RSH BW_OR BW_AND BW_XOR AND OR NEGATE  MOD
%token LTE GTE GT LT EQ NE BW_NOT DIV ADD SUB MUL POW

%token IF ELIF

%token WHILE CONT FOR LOOP BREAK;

%token TYPE VALUE INDEX DEST 
%token DYN_CREATE DYN_EXPAND DYN_INSERT DYN_APPEND DYN_CLEAR 
%token DYN_DELETE DYN_GET DYN_SIZE DYN_FRONT DYN_BACK

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
%token <int>         OBJECT         // These tokens encode line numbers
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
   : unit_space            { driver.build($1); }
   | input unit_space      { driver.build($2); }
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
   | OBJECT LT identifiers GT { $$ = new EncodedDataType(DEL::DataType::USER_DEFINED, $3); }
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
   | object_assignment { $$ = $1; }
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
   | dyn_stmt             { $$ = $1; }
   ;

multiple_statements
   : stmt                     { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | multiple_statements stmt { $1.push_back($2); $$ = $1; }
   ;

multiple_assignments
   : assignment                      { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | multiple_assignments assignment { $1.push_back($2); $$ = $1; }
   ;

block 
   : LEFT_BRACKET multiple_statements RIGHT_BRACKET { $$ = $2; }
   | LEFT_BRACKET RIGHT_BRACKET                     { $$ = std::vector<DEL::Element*>(); }
   ;

// 
// ------------------------------ Unit Block ----------------------------
// 

unit_space_multiple_stmts
   : stmt                            { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | function_stmt                   { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | unit_space_multiple_stmts stmt  { $1.push_back($2); $$ = $1; }
   | unit_space_multiple_stmts function_stmt { $1.push_back($2); $$ = $1; }
   | object_definition                { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | unit_space_multiple_stmts object_definition { $1.push_back($2); $$ = $1; }
   | unit_space                           { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | unit_space_multiple_stmts unit_space { $1.push_back($2); $$ = $1; }
   ;

unit_block
   : LEFT_BRACKET unit_space_multiple_stmts RIGHT_BRACKET { $$ = $2; }
   | LEFT_BRACKET RIGHT_BRACKET                     { $$ = std::vector<DEL::Element*>(); }
   ;

unit_space
   : UNIT identifiers unit_block { $$ = new DEL::UnitSpace($2, $3, $1); }
   ;

// 
// ------------------------- Function Statements ------------------------
// 

single_func_param
   : INT    COL identifiers { $$ = new Parameter(DEL::DataType::INT,    $3, "int"   , false); }
   | DOUBLE COL identifiers { $$ = new Parameter(DEL::DataType::DOUBLE, $3, "double", false); }
   | STRING COL identifiers { $$ = new Parameter(DEL::DataType::STRING, $3, "string", false); }
   | OBJECT LT identifiers GT COL identifiers
      {
         $$ = new Parameter(DEL::DataType::USER_DEFINED, $6, $3, false);
      }
   | REF INT    COL identifiers { $$ = new Parameter(DEL::DataType::INT,    $4, "int"   , true); }
   | REF DOUBLE COL identifiers { $$ = new Parameter(DEL::DataType::DOUBLE, $4, "double", true); }
   | REF STRING COL identifiers { $$ = new Parameter(DEL::DataType::STRING, $4, "string", true); }
   | REF OBJECT LT identifiers GT COL identifiers
      {
         $$ = new Parameter(DEL::DataType::USER_DEFINED, $7, $4, true);
      }
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

// 
// ------------------ Object Definition Specific Stuff ------------------
// 

object_instantiation
   : LEFT_BRACKET multiple_assignments RIGHT_BRACKET { $$ = $2; }
   | LEFT_BRACKET RIGHT_BRACKET { $$ = std::vector<DEL::Element*>();  }
   ;

object_assignment
   : VAR identifiers COL OBJECT LT identifiers GT ASSIGN object_instantiation SEMI 
      {
         $$ = new DEL::ObjectAssignment(
                                    false, 
                                    $2, 
                                    new EncodedDataType(DEL::DataType::USER_DEFINED, $6),
                                    $9,
                                    $10);
      }
   | LET identifiers COL OBJECT LT identifiers GT ASSIGN object_instantiation SEMI 
      {
         $$ = new DEL::ObjectAssignment(
                                    true, 
                                    $2, 
                                    new EncodedDataType(DEL::DataType::USER_DEFINED, $6),
                                    $9,
                                    $10);
      }
   | identifiers ASSIGN object_instantiation SEMI 
      {
         $$ = new ObjectReassignment($1, $3, $4);
      }
   ;

member_definition
   : assignable_type COL identifiers SEMI          { $$ = new ObjectMember($1, $3, $4); }
   | OBJECT LT identifiers GT COL identifiers SEMI 
     { 
        $$ = new ObjectMember(new DEL::EncodedDataType(DEL::DataType::USER_DEFINED, $3),
                              $6,
                              $7);
     }
   ;

object_stmt
   : member_definition { $$ = $1; }
   | function_stmt     { $$ = $1; }
   | dyn_stmt             { $$ = $1; }
   ;

multiple_object_stmts
   : object_stmt                       { $$ = std::vector<DEL::Element*>(); $$.push_back($1); }
   | multiple_object_stmts object_stmt { $1.push_back($2); $$ = $1; }
   ;

object_block
   : LEFT_BRACKET multiple_object_stmts RIGHT_BRACKET { $$ = $2; }
   | LEFT_BRACKET RIGHT_BRACKET                       { $$ = std::vector<DEL::Element*>(); }
   ;

object_definition
   : OBJECT identifiers LEFT_BRACKET PUB  object_block PRIV object_block RIGHT_BRACKET 
         {
            $$ = new Object($2, $5, $7, $1);
         }
   | OBJECT identifiers LEFT_BRACKET PRIV object_block PUB  object_block RIGHT_BRACKET 
         {
            $$ = new Object($2, $7, $5, $1);
         }
   | OBJECT identifiers LEFT_BRACKET PUB object_block RIGHT_BRACKET 
         {
            $$ = new Object($2, $5, std::vector<DEL::Element*>(), $1);
         }
   ;

// 
// --------------------------- Dyn Statements ---------------------------
// 

dyn_stmt
   : DYN_CREATE LEFT_PAREN identifiers COMMA TYPE assignable_type RIGHT_PAREN SEMI
      {
         $$ = new DEL::DynCreate($3, $6, $8);
      }
   | DYN_CREATE LEFT_PAREN identifiers COMMA TYPE OBJECT LT identifiers GT RIGHT_PAREN SEMI
      {
         $$ = new DEL::DynCreate($3, 
                                 new EncodedDataType(DEL::DataType::USER_DEFINED, $8),
                                 $11);
      }
   | DYN_EXPAND LEFT_PAREN REF identifiers COMMA VALUE INT_LITERAL RIGHT_PAREN SEMI
     {
        $$ = new DEL::DynExpand($4, $7, $9);
     }

   | DYN_INSERT LEFT_PAREN REF identifiers COMMA INDEX INT_LITERAL COMMA VALUE expression RIGHT_PAREN SEMI
     {
        $$ = new DEL::DynInsert($4, $7, $10, $12);
     }
   | DYN_APPEND LEFT_PAREN REF identifiers COMMA VALUE expression RIGHT_PAREN SEMI
     {
        $$ = new DEL::DynAppend($4, $7, $9);
     }
   | DYN_CLEAR LEFT_PAREN REF identifiers RIGHT_PAREN SEMI
     {
        $$ = new DEL::DynClear($4, $6);
     }
   | DYN_DELETE LEFT_PAREN REF identifiers COMMA INDEX INT_LITERAL RIGHT_PAREN SEMI
     {
        $$ = new DEL::DynDelete($4, $7, $9);
     }
   | DYN_GET LEFT_PAREN REF identifiers COMMA INDEX INT_LITERAL COMMA DEST REF identifiers RIGHT_PAREN SEMI
     {
        $$ = new DEL::DynGet($4, $7, $11, $13);
     }
   | DYN_SIZE LEFT_PAREN REF identifiers COMMA DEST REF identifiers RIGHT_PAREN SEMI
     {
        $$ = new DEL::DynSize($4, $8, $10);
     }
   | DYN_FRONT LEFT_PAREN REF identifiers COMMA DEST REF identifiers RIGHT_PAREN SEMI
     {
        $$ = new DEL::DynFront($4, $8, $10);
     }
   | DYN_BACK LEFT_PAREN REF identifiers COMMA DEST REF identifiers RIGHT_PAREN SEMI
     {
        $$ = new DEL::DynBack($4, $8, $10);
     }
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
   std::cerr << "Syntax error >> line: " << l.begin.line << " >> col: " << l.begin.column << std::endl;
}