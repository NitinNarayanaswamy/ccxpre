file(GLOB ccxpre_SOURCES *.cpp)
file(GLOB ccxpre_HEADERS *.hpp)

add_library(ccxpre ${ccxpre_SOURCES} ${ccxpre_HEADERS})

target_include_directories(ccxpre PUBLIC ${CMAKE_SOURCE_DIR}/ccxpre/)

