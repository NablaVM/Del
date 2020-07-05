set(FORGE_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FORGE_DIR}
    ${FORGE_DIR}/reporting
)

set(FORGE_HEADERS
    ${FORGE_DIR}/Aggregator.hpp
    ${FORGE_DIR}/Forge.hpp
    ${FORGE_DIR}/ForgeFwd.hpp
    ${FORGE_DIR}/ForgeSignal.hpp
    ${FORGE_DIR}/GeneratorIf.hpp
    ${FORGE_DIR}/SystemSettings.hpp
    
    ${FORGE_DIR}/constructs/Expression.hpp
    ${FORGE_DIR}/constructs/Function.hpp
    ${FORGE_DIR}/constructs/Variable.hpp

    ${FORGE_DIR}/datatypes/DataType.hpp

    ${FORGE_DIR}/instructions/Instruction.hpp
    ${FORGE_DIR}/instructions/Assignment.hpp
    ${FORGE_DIR}/instructions/Call.hpp
    ${FORGE_DIR}/instructions/Continuable.hpp
    ${FORGE_DIR}/instructions/Breakable.hpp
    ${FORGE_DIR}/instructions/If.hpp
    ${FORGE_DIR}/instructions/Ressignment.hpp
    ${FORGE_DIR}/instructions/Return.hpp
    ${FORGE_DIR}/instructions/For.hpp
    ${FORGE_DIR}/instructions/While.hpp

    ${FORGE_DIR}/managers/Memory.hpp
    ${FORGE_DIR}/managers/SymbolTable.hpp

    ${FORGE_DIR}/reporting/Report.hpp
    ${FORGE_DIR}/reporting/Reporter.hpp
    ${FORGE_DIR}/reporting/ReportTypes.hpp
    ${FORGE_DIR}/reporting/CustomReport.hpp
    ${FORGE_DIR}/reporting/InternalReport.hpp
    ${FORGE_DIR}/reporting/SemanticReport.hpp
    ${FORGE_DIR}/reporting/SyntaxReport.hpp


    ${FORGE_DIR}/codegen/Codegen.hpp
    ${FORGE_DIR}/codegen/Generator.hpp
    ${FORGE_DIR}/codegen/asm/AsmMath.hpp
    ${FORGE_DIR}/codegen/asm/AsmStoreLoad.hpp
    ${FORGE_DIR}/codegen/asm/AsmSupport.hpp
    ${FORGE_DIR}/codegen/codeblocks/Alloc.hpp
    ${FORGE_DIR}/codegen/codeblocks/BlockAggregator.hpp
    ${FORGE_DIR}/codegen/codeblocks/Codeblock.hpp
    ${FORGE_DIR}/codegen/codeblocks/ConditionalContext.hpp
    ${FORGE_DIR}/codegen/codeblocks/ForLoopContext.hpp
    ${FORGE_DIR}/codegen/codeblocks/Function.hpp
    ${FORGE_DIR}/codegen/codeblocks/LoadStore.hpp
    ${FORGE_DIR}/codegen/codeblocks/Operations.hpp
    ${FORGE_DIR}/codegen/codeblocks/Primitives.hpp
)

set(FORGE_SOURCES
    ${FORGE_DIR}/Forge.cpp


    ${FORGE_DIR}/constructs/Function.cpp
    
    ${FORGE_DIR}/instructions/Assignment.cpp
    ${FORGE_DIR}/instructions/Call.cpp
    ${FORGE_DIR}/instructions/If.cpp
    ${FORGE_DIR}/instructions/Reassignment.cpp
    ${FORGE_DIR}/instructions/Return.cpp
    ${FORGE_DIR}/instructions/For.cpp
    ${FORGE_DIR}/instructions/While.cpp

    ${FORGE_DIR}/managers/Memory.cpp
    ${FORGE_DIR}/managers/SymbolTable.cpp

    ${FORGE_DIR}/reporting/Reporter.cpp
    ${FORGE_DIR}/reporting/CustomReport.cpp
    ${FORGE_DIR}/reporting/InternalReport.cpp
    ${FORGE_DIR}/reporting/SemanticReport.cpp
    ${FORGE_DIR}/reporting/SyntaxReport.cpp


    ${FORGE_DIR}/codegen/Codegen.cpp
    ${FORGE_DIR}/codegen/Generator.cpp
    ${FORGE_DIR}/codegen/asm/AsmSupport.cpp

)