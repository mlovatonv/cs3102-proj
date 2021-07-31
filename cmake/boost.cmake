# Boost

find_package(Boost 1.75 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
