
SET (base_engine_SRC 
    symbolnode.cpp
    symbol.cpp
    abstractsymbology.cpp
    abstractbarcodeengine.cpp)
add_library(base_engine SHARED ${base_engine_SRC})
add_subdirectory(tests)

#include_directories(product)
add_subdirectory(product)