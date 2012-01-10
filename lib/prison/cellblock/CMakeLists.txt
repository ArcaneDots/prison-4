#include_directories ("${PROJECT_SOURCE_DIR}/product" "${PROJECT_SOURCE_DIR}/linear")

#file( GLOB ean13engine_SRC *.cpp )
SET (base_engine_SRC
      indexedregexp.cpp
      abstractbarcodeengine.cpp)
add_library(base_engine SHARED ${base_engine_SRC})

add_subdirectory(product)
#add_subdirectory(linear)