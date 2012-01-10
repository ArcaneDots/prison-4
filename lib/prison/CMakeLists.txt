include_directories ("${PROJECT_SOURCE_DIR}/cellblock")
add_subdirectory(cellblock)

# include_directories ("${PROJECT_SOURCE_DIR}/test")
# add_subdirectory(test)

include_directories(${QRENCODE_INCLUDE_DIR} ${DMTX_INCLUDE_DIR})
SET(    prison_SRC
        abstractbarcode.cpp
        barcodewidget.cpp
        barcodeitem.cpp
        datamatrixbarcode.cpp
        qrcodebarcode.cpp
        ean13barcode.cpp
        ean8barcode.cpp
        upcabarcode.cpp
        upcebarcode.cpp
        )

add_library(prison SHARED ${prison_SRC} ) 
target_link_libraries(prison product_engines base_engine  ${DMTX_LIBRARIES} ${QRENCODE_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
#  linear_engines
set_target_properties(prison PROPERTIES VERSION "0.0.0" SOVERSION "0" LINK_INTERFACE_LIBRARIES "" DEFINE_SYMBOL BUILDING_PRISON)

set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )

install( TARGETS prison DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX} )

install( FILES
         abstractbarcode.h
         AbstractBarcode
         barcodeitem.h
         BarcodeItem
         barcodewidget.h
         BarcodeWidget
         datamatrixbarcode.h
         DataMatrixBarcode
         qrcodebarcode.h
         QRCodeBarcode
         Ean13Barcode
         ean13barcode.h
         Ean8Barcode
         ean8barcode.h
         UpcABarcode
         upcabarcode.h
         UpcEBarcode
         upcebarcode.h
         prison_export.h
         DESTINATION ${CMAKE_INSTALL_PREFIX}/include/prison COMPONENT devel
         )

set(INCLUDE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
get_target_property(LIB_OUT_NAME prison LOCATION)
get_filename_component(LIB_OUT_NAME ${LIB_OUT_NAME} NAME)
set(LIBPATH ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})

configure_file(
   PrisonConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/PrisonConfig.cmake @ONLY
)

install( FILES
         ${CMAKE_CURRENT_BINARY_DIR}/PrisonConfig.cmake
	 DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/cmake/Prison
)


