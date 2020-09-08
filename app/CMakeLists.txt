include_directories(${CMAKE_SOURCE_DIR})

add_executable(ccxpre${CMAKE_PROJECT_VERSION} app.cpp)

target_link_libraries(ccxpre${CMAKE_PROJECT_VERSION} PRIVATE ccxpre)

set_target_properties(ccxpre${CMAKE_PROJECT_VERSION} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)