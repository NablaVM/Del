BISON_TARGET(del_parser
            ${DEL_COMPILER_DIR}/del_parser.yy
            ${CMAKE_CURRENT_BINARY_DIR}/del_parser.tab.cc)

FLEX_TARGET(del_lexer
            ${DEL_COMPILER_DIR}/del_lexer.l
            ${CMAKE_CURRENT_BINARY_DIR}/del_lexer.yy.cc)

ADD_FLEX_BISON_DEPENDENCY(del_lexer del_parser)

set(DEL_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    
    ${FLEX_INCLUDE_DIRS}
)

set(DEL_COMPILER_HEADERS
    ${DEL_COMPILER_DIR}

    ${DEL_COMPILER_DIR}/del_driver.hpp
    ${DEL_COMPILER_DIR}/del_scanner.hpp

    ${DEL_COMPILER_DIR}/Analysis/Analyzer.hpp
    
    ${DEL_COMPILER_DIR}/Ast/Ast.hpp
    ${DEL_COMPILER_DIR}/Ast/Elements.hpp

    ${DEL_COMPILER_DIR}/Preprocessor/Preprocessor.hpp

    ${DEL_COMPILER_DIR}/Types/Variables.hpp
)

set(DEL_COMPILER_SOURCES

    ${DEL_COMPILER_DIR}/del_driver.cpp
    
    ${DEL_COMPILER_DIR}/Analysis/Analyzer.cpp
    ${DEL_COMPILER_DIR}/Ast/Elements.cpp

    ${DEL_COMPILER_DIR}/Preprocessor/Preprocessor.cpp

    ${FLEX_del_lexer_OUTPUTS}
    ${BISON_del_parser_OUTPUTS}
)

