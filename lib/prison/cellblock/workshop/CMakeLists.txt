include_directories ("${PROJECT_SOURCE_DIR}/workshop")
#file( GLOB ean13engine_SRC *.cpp )
SET (symbol_table_engine_SRC
     symboldatum.cpp
     lookuptableentry.cpp
     indexedregexp.cpp
     barcodesymbolset.cpp
     symbol.cpp
     symbollist.cpp) #abstractbarcodeengine.cpp
add_library(symbol_table_engine SHARED ${symbol_table_engine_SRC})


add_subdirectory(tests)