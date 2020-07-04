set(FORGE_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FORGE_DIR}
    ${FORGE_DIR}/reporting
)

set(FORGE_HEADERS
    ${FORGE_DIR}/Forge.hpp
    ${FORGE_DIR}/ForgeFwd.hpp
    ${FORGE_DIR}/ForgeSignal.hpp
    ${FORGE_DIR}/Generator.hpp
    ${FORGE_DIR}/SystemSettings.hpp

    ${FORGE_DIR}/constructs/Expression.hpp
    ${FORGE_DIR}/constructs/Function.hpp
    ${FORGE_DIR}/constructs/Variable.hpp

    ${FORGE_DIR}/datatypes/DataType.hpp

    ${FORGE_DIR}/instructions/Assignment.hpp
    ${FORGE_DIR}/instructions/Ressignment.hpp
    ${FORGE_DIR}/instructions/Instruction.hpp

    ${FORGE_DIR}/reporting/Report.hpp
    ${FORGE_DIR}/reporting/Reporter.hpp
    ${FORGE_DIR}/reporting/ReportTypes.hpp
    ${FORGE_DIR}/reporting/CustomReport.hpp
    ${FORGE_DIR}/reporting/InternalReport.hpp
    ${FORGE_DIR}/reporting/SemanticReport.hpp
    ${FORGE_DIR}/reporting/SyntaxReport.hpp
)

set(FORGE_SOURCES
    ${FORGE_DIR}/Forge.cpp
    ${FORGE_DIR}/constructs/Function.cpp
    ${FORGE_DIR}/instructions/Assignment.cpp
    ${FORGE_DIR}/instructions/Reassignment.cpp

    ${FORGE_DIR}/reporting/Reporter.cpp
    ${FORGE_DIR}/reporting/CustomReport.cpp
    ${FORGE_DIR}/reporting/InternalReport.cpp
    ${FORGE_DIR}/reporting/SemanticReport.cpp
    ${FORGE_DIR}/reporting/SyntaxReport.cpp
)