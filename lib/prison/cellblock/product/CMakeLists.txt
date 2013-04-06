SET (product_engines_SRC
     productengine.cpp
     upcaengine.cpp
     upceengine.cpp
     ean2engine.cpp     
     ean5engine.cpp
     ean8engine.cpp
     ean13engine.cpp
     )
     
add_library(product_engines SHARED ${product_engines_SRC})