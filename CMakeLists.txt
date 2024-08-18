cmake_minimum_required(VERSION 3.28)
project(Qt_Lunch_Lottery)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ui 폴더에 있는 ui 파일을 읽을 수 있도록 설정
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

# 프로젝트 루트 디렉토리의 include 폴더를 포함 경로에 추가
include_directories(${CMAKE_SOURCE_DIR}/include)

# 헤더 파일 리스트
set(HEADER_FILES
        include/mainwidget.h
        include/imagewidget.h
)

set(SRC_FILES
        src/mainwidget.cpp
        src/imagewidget.cpp
)

set(UI_FILES
        ui/mainwidget.ui
)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Qt_Lunch_Lottery main.cpp
        ${HEADER_FILES}
        ${SRC_FILES}
        ${UI_FILES}
        include/community.h
        src/community.cpp
        images.qrc
)
target_link_libraries(Qt_Lunch_Lottery
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

