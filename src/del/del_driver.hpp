#ifndef __DELDRIVER_HPP__
#define __DELDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>
#include <vector>

#include "Ast/Ast.hpp"
#include "Ast/Elements.hpp"

#include "Analysis/Analyzer.hpp"

#include "Preprocessor/Preprocessor.hpp"

#include "del_scanner.hpp"
#include "del_parser.tab.hh"

#include "Forge.hpp"
#include "reporting/Reporter.hpp"
#include "framework/SymbolTable.hpp"

namespace DEL
{
   class DEL_Parser;

   //!\brief The DEL Driver object
   class DEL_Driver
   {
   public:
      DEL_Driver();

      //! \brief Deconstruct
      virtual ~DEL_Driver();
      
      //! \brief Parse from a file
      void parse( const char * const filename );

      //! \brief Inc line count
      void inc_line();

      //! \brief Build an element
      void build(Element * element);

      //! \brief Indicate that parsing is completed
      void indicate_complete();

      //! \brief Indicate a directive from the preprocessor was parsed
      void preproc_file_directive(std::string directive);

      friend DEL_Parser;
      friend Analyzer;

   private:

      Analyzer analyzer;
      Preprocessor preprocessor;

      //! \brief Parse from an istream - Not currently used
      void parse( std::istream &iss );

      std::string current_file_from_directive;

      std::ostream& print(std::ostream &stream);
      
      void parse_helper( std::istream &stream );

      DEL::DEL_Parser  *parser  = nullptr;
      DEL::DEL_Scanner *scanner = nullptr;

      // The code forge
      FORGE::Forge code_forge;
      FORGE::SymbolTable symbol_table;
   };

}
#endif
