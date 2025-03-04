cmake_minimum_required(VERSION 3.10)

file(GLOB cpp_files ./*.cpp)

set(BUILD_ITEMS "${BUILD_ITEMS} backend" CACHE INTERNAL "all items to build")
set(MAIN_EXECUTE_NAME "backend")
#add_executable(${MAIN_EXECUTE_NAME} end_device_main.cpp ${BASIC_FUNC})
add_executable(${MAIN_EXECUTE_NAME} main.cpp backend.cpp ${BASIC_FUNC})
#set_target_properties(${MAIN_EXECUTE_NAME} PROPERTIES ENABLE_EXPORTS TRUE)
target_link_libraries(${MAIN_EXECUTE_NAME} kafka)
target_link_libraries(${MAIN_EXECUTE_NAME} util)
target_link_libraries(${MAIN_EXECUTE_NAME} database)

file(GLOB allHeaderFiles "./*.h")
file(COPY ${allHeaderFiles} DESTINATION ${BUILD_DIR}/include)