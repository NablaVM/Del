set(FORGE_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FORGE_DIR}/reporting
)

set(FORGE_HEADERS
    ${FORGE_DIR}/Forge.hpp
    ${FORGE_DIR}/ForgeSignal.hpp
    ${FORGE_DIR}/reporting/Report.hpp
    ${FORGE_DIR}/reporting/Reporter.hpp
    ${FORGE_DIR}/reporting/ReportTypes.hpp
    ${FORGE_DIR}/reporting/SyntaxReport.hpp
    ${FORGE_DIR}/reporting/CustomReport.hpp
)

set(FORGE_SOURCES
    ${FORGE_DIR}/Forge.cpp
    ${FORGE_DIR}/reporting/Reporter.cpp
    ${FORGE_DIR}/reporting/SyntaxReport.cpp
    ${FORGE_DIR}/reporting/CustomReport.cpp
)