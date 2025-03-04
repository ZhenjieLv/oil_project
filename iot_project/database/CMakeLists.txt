cmake_minimum_required(VERSION 3.10)

file(GLOB cpp_files ./*.cpp)

set(LIB_NAME "database")

add_library(${LIB_NAME} SHARED ${cpp_files})
target_compile_options(${LIB_NAME} PUBLIC ${MYSQL_CFLAGS})
target_link_libraries(${LIB_NAME} ${MYSQL_LIBS})

file(GLOB allHeaderFiles "./*.h")
file(COPY ${allHeaderFiles} DESTINATION ${BUILD_DIR}/include)