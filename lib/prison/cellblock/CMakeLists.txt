
SET (base_engine_SRC
      indexedregexp.cpp
      abstractbarcodeengine.cpp)
add_library(base_engine SHARED ${base_engine_SRC})

add_subdirectory(product)