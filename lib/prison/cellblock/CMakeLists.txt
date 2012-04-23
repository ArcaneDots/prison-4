add_subdirectory(workshop)

 SET (base_engine_SRC
      abstractbarcodeengine.cpp)
add_library(base_engine SHARED ${base_engine_SRC})

add_subdirectory(tests)

add_subdirectory(product)
#add_subdirectory(linear)