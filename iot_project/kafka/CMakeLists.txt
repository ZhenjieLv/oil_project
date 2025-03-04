cmake_minimum_required(VERSION 3.10)

file(GLOB cpp_files ./*.cpp)

set(LIB_NAME "kafka")

add_library(${LIB_NAME} SHARED ${cpp_files})
target_link_libraries(${LIB_NAME} jsoncpp_lib)
target_link_libraries(${LIB_NAME} ${PYTHON_LIBRARIES})
target_link_libraries(${LIB_NAME} rdkafka++)
target_link_libraries(${LIB_NAME} util)
target_link_libraries(${LIB_NAME} database)
target_link_libraries(${LIB_NAME} cryptopp)
file(GLOB allHeaderFiles "./*.h")
file(COPY ${allHeaderFiles} DESTINATION ${BUILD_DIR}/include)