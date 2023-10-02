cmake_minimum_required(VERSION 3.9)
project(fbr)
set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)

include_directories(
        ./include
        ./src
        ./lib
)

add_library( ftrScanAPI SHARED IMPORTED )

set_target_properties( ftrScanAPI PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libScanAPI.so )

add_library(fbr SHARED
        library.cpp
        src/Bozorth.cpp
        src/Cwsq.cpp
        src/Mindtct.cpp
        src/Scanner.cpp
        src/Utils.cpp
)

install(
        TARGETS fbr
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

target_link_libraries(fbr ftrScanAPI)

set_target_properties(fbr PROPERTIES PUBLIC_HEADER include/library.h)

ADD_CUSTOM_COMMAND(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar "cfv" "FBR.zip" --format=zip
        "${CMAKE_CURRENT_SOURCE_DIR}/external/setup.sh"
        "${CMAKE_CURRENT_SOURCE_DIR}/external/bin/bozorth3"
        "${CMAKE_CURRENT_SOURCE_DIR}/external/bin/cwsq"
        "${CMAKE_CURRENT_SOURCE_DIR}/external/bin/mindtct"
        "${CMAKE_CURRENT_SOURCE_DIR}/external/lib/libScanAPI.so"
        "${CMAKE_CURRENT_SOURCE_DIR}/build/libfbr.so"
)
