#include <cctype>
#include <fstream>
#include <cassert>
#include <iostream>
#include <string>
#include "del_driver.hpp"

namespace DEL
{
   DEL_Driver::DEL_Driver()
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

      std::ifstream ifs(filename);

      if(!ifs.is_open())
      {
         std::cerr << "Unable to open : " << filename << std::endl;
      }

      parse_helper( ifs );

      ifs.close();
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
         std::cerr << "Failed to allocate scanner: (" << ba.what() << ")\n";
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
         std::cerr << "Failed to allocate parser: (" <<  ba.what() << ")\n";
         exit( EXIT_FAILURE );
      }
      const int accept( 0 );
      if( parser->parse() != accept )
      {
         std::cerr << "Parse failed!!\n";
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
}