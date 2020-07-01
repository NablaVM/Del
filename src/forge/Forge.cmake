set(FORGE_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FORGE_DIR}/reporting
)

set(FORGE_HEADERS
    ${FORGE_DIR}/Forge.hpp
    ${FORGE_DIR}/ForgeSignal.hpp
    ${FORGE_DIR}/SystemSettings.hpp

    ${FORGE_DIR}/framework/DataTypes.hpp
    ${FORGE_DIR}/framework/Primitives.hpp

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

    ${FORGE_DIR}/reporting/Reporter.cpp
    ${FORGE_DIR}/reporting/CustomReport.cpp
    ${FORGE_DIR}/reporting/InternalReport.cpp
    ${FORGE_DIR}/reporting/SemanticReport.cpp
    ${FORGE_DIR}/reporting/SyntaxReport.cpp
)