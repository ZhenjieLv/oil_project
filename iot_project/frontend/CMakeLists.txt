cmake_minimum_required(VERSION 3.10)

#打开全局moc
set(CMAKE_AUTOMOC ON)
#打开全局uic
set(CMAKE_AUTOUIC ON)
#打开全局rcc
set(CMAKE_AUTORCC ON)

set(qui_SOURCES 
	main.cpp
	appinit.cpp 
  login.cpp
  iconhelper.cpp
  register_info.cpp
  uidemo08.cpp
)

set(qui_HEADERS
  appinit.h 
  login.h
  iconhelper.h
  uidemo08.h
  register_info.h
)

set(qui_FORMS 
  appinit.ui 
  login.ui
  iconhelper.ui
  uidemo08.ui
  register_info.ui
)
set(qui_RESOURCES
	main.qrc
  qss.qrc
)


#查找需要的Qt包
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
#调用预编译器moc，需要使用 QT5_WRAP_CPP宏
QT5_WRAP_CPP(qui_HEADERS_MOC ${qui_HEADERS})

#使用uic处理.ui文件
QT5_WRAP_UI(qui_FORMS_HEADERS ${qui_FORMS})

#使用rcc处理.qrc文件
QT5_ADD_RESOURCES(qui_RESOURCES_RCC ${qui_RESOURCES})

#这些生成的中间文件都会在build目录下，这样的话，编译器则不能定位由uic程序产生的诸如_ui_mainwindow.h等文件。所以，我们需要把build目录添加到包含目录中
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


set(BUILD_ITEMS "${BUILD_ITEMS} frontend" CACHE INTERNAL "all items to build")
set(MAIN_EXECUTE_NAME "frontend")
add_executable(${MAIN_EXECUTE_NAME}
	${qui_SOURCES} package_process.cpp  ${BASIC_FUNC}
	${qui_HEADERS_MOC} 
    ${qui_FORMS_HEADERS} package_process.h
	${qui_RESOURCES_RCC}    
)

target_link_libraries(${MAIN_EXECUTE_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(${MAIN_EXECUTE_NAME} util)
target_link_libraries(${MAIN_EXECUTE_NAME} database)
target_link_libraries(${MAIN_EXECUTE_NAME} kafka)

#file(GLOB allHeaderFiles "status.h variables.h sync_cloud.h")
#file(COPY ${allHeaderFiles} DESTINATION ${BUILD_DIR}/include)
#file(GLOB allHeaderFiles "./*.h")
#file(COPY ${allHeaderFiles} DESTINATION ${BUILD_DIR}/include)