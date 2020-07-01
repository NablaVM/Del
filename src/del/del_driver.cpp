#include <cctype>
#include <fstream>
#include <cassert>
#include <iostream>
#include <string>
#include "del_driver.hpp"

namespace DEL
{
   DEL_Driver::DEL_Driver() : analyzer(code_forge), preprocessor(code_forge)
   {
      
   }

   // ----------------------------------------------------------
   //
   // ----------------------------------------------------------

   DEL_Driver::~DEL_Driver()
   {
      delete(scanner);
      scanner = nullptr;
      delete(parser);
      parser = nullptr;
   }

   // ----------------------------------------------------------
   //
   // ----------------------------------------------------------
   
   void DEL_Driver::parse( const char * const filename )
   {
      assert( filename != nullptr );

      // If this fails, things die
      preprocessor.process(filename);

      std::string preproc_file = preprocessor.get_preprocessed_filename();

      std::ifstream in_file(preproc_file);
      if( ! in_file.good() )
      {
        code_forge.get_reporter().issue_report(
            new FORGE::InternalReport(
                FORGE::InternalReport::InternalInfo{
                    "DEL_Driver",
                    "del_driver.cpp",
                    "DEL_Driver::parse",
                    {("Developer Error : Preprocessed file : " + preproc_file + " unabel to be opened")}
                }
            )
        );
         exit( EXIT_FAILURE );
      }
      parse_helper( in_file );
      return;
   }

   // ----------------------------------------------------------
   //
   // ----------------------------------------------------------
   
   void DEL_Driver::parse( std::istream &stream )
   {
      if( ! stream.good()  && stream.eof() )
      {
         return;
      }
      //else
      parse_helper( stream ); 
      return;
   }

   // ----------------------------------------------------------
   //
   // ----------------------------------------------------------
   
   void  DEL_Driver::parse_helper( std::istream &stream )
   {
      delete(scanner);
      try
      {
         scanner = new DEL::DEL_Scanner( &stream, (*this) );
      }
      catch( std::bad_alloc &ba )
      {
         code_forge.get_reporter().issue_report(
            new FORGE::InternalReport(
                  FORGE::InternalReport::InternalInfo{
                     "DEL_Driver",
                     "del_driver.cpp",
                     "DEL_Driver::parse_helper",
                     {("Failed to allocate scanner: (" + std::string(ba.what()) + ")")}
                  }
            )
         );
         exit( EXIT_FAILURE );
      }

      delete(parser); 
      try
      {
         parser = new DEL::DEL_Parser( (*scanner) /* scanner */, 
                                       (*this)    /* driver  */);
      }
      catch( std::bad_alloc &ba )
      {
         code_forge.get_reporter().issue_report(
            new FORGE::InternalReport(
                  FORGE::InternalReport::InternalInfo{
                     "DEL_Driver",
                     "del_driver.cpp",
                     "DEL_Driver::parse_helper",
                     {("Failed to allocate parser: (" + std::string(ba.what()) + ")")}
                  }
            )
         );
         exit( EXIT_FAILURE );
      }
      const int accept( 0 );
      if( parser->parse() != accept )
      {
         code_forge.get_reporter().issue_report(
            new FORGE::InternalReport(
                  FORGE::InternalReport::InternalInfo{
                     "DEL_Driver",
                     "del_driver.cpp",
                     "DEL_Driver::parse_helper",
                     {"Parse failed!"}
                  }
            )
         );
      }
      return;
   }

   // ----------------------------------------------------------
   //
   // ----------------------------------------------------------
   
   std::ostream& DEL_Driver::print( std::ostream &stream )
   {
      //stream << "done" << "\n";
      return(stream);
   }

   // ----------------------------------------------------------
   //
   // ----------------------------------------------------------

   void DEL_Driver::indicate_complete()
   {
      std::cout << "DEL_Driver::indicate_complete()" << std::endl;
   }

   // ----------------------------------------------------------
   //
   // ----------------------------------------------------------

   void DEL_Driver::build(Element *element)
   {
      std::cout << "DEL_Driver::build()" << std::endl;

      // Visit analyzer
      element->visit(analyzer);

      // Delete element
      delete element;
   }

   // ----------------------------------------------------------
   //
   // ----------------------------------------------------------

   void DEL_Driver::preproc_file_directive(std::string directive)
   {
      current_file_from_directive = (directive.substr(6, directive.size()));
   }
}