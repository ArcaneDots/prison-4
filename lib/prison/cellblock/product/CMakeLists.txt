INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET (product_engines_SRC
     productengine.cpp
     upcaengine.cpp
     upceengine.cpp
     ean2engine.cpp     
     ean5engine.cpp
     ean8engine.cpp
     ean13engine.cpp
     )
     
add_library(product_engines SHARED  ${product_engines_SRC})
target_link_libraries(product_engines base_engine)
add_subdirectory(tests)

