#based on KDAB / Charm
#https://github.com/KDAB/Charm/blob/master/Tests/CMakeLists.txt


INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/tests")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}) # ${Charm_SOURCE_DIR} )

SET( TEST_LIBRARIES symbol_table_engine ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )
SET( TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../)

# Test : class lookupTableEntry
SET( lookupTableTests_SRCS lookupTableEntry_test.cpp )
QT4_AUTOMOC( ${lookupTableTests_SRCS} ${TEST_SRC} )
ADD_EXECUTABLE( lookupTableTests ${lookupTableTests_SRCS} )
TARGET_LINK_LIBRARIES( lookupTableTests ${TEST_LIBRARIES} )
ADD_TEST( NAME lookupTableTests COMMAND lookupTableTests )

# Test : class BarcodeSymbolSet
SET( BarcodeSetTests_SRCS barcodeSymbolSet_test.cpp )
QT4_AUTOMOC( ${BarcodeSetTests_SRCS} ${TEST_SRC} )
ADD_EXECUTABLE( BarcodeSetTests ${BarcodeSetTests_SRCS})
TARGET_LINK_LIBRARIES( BarcodeSetTests ${TEST_LIBRARIES} )
ADD_TEST( NAME BarcodeSetTests COMMAND BarcodeSetTests )

# Test : class SymbolDatum
SET( SymbolDatumTests_SRCS symbolDatum_test.cpp )
QT4_AUTOMOC( ${SymbolDatumTests_SRCS} ${TEST_SRC} )
ADD_EXECUTABLE( SymbolDatumTests ${SymbolDatumTests_SRCS})
TARGET_LINK_LIBRARIES( SymbolDatumTests ${TEST_LIBRARIES} )
ADD_TEST( NAME SymbolDatumTests COMMAND SymbolDatumTests )

# Test : class Symbol
SET( SymbolTests_SRCS symbol_test.cpp )
QT4_AUTOMOC( ${SymbolTests_SRCS} ${TEST_SRC} )
ADD_EXECUTABLE( SymbolTests ${SymbolTests_SRCS})
TARGET_LINK_LIBRARIES( SymbolTests ${TEST_LIBRARIES} )
ADD_TEST( NAME SymbolTests COMMAND SymbolTests )