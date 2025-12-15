cmake_minimum_required(VERSION 3.16)

project(LMS_Qt_App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


qt_standard_project_setup()

set(SOURCES
    ConsoleApplication1.cpp
    LMSDatabase.cpp
    LMSDatabase.h
    User.cpp
    User.h
    Student.cpp
    Student.h
    Teacher.cpp
    Teacher.h
    Admin.cpp
    Admin.h
    Course.cpp
    Course.h
    OnlineCourse.cpp
    OnlineCourse.h
    OnsiteCourse.cpp
    OnsiteCourse.h
    Assignment.h 
    Submission.cpp 
    Submission.h
    MainWindow.h
    MainWindow.cpp
    LoginWidget.h
    LoginWidget.cpp
    AdminDashboard.h
    AdminDashboard.cpp
    TeacherDashboard.h
    TeacherDashboard.cpp
    StudentDashboard.h
    StudentDashboard.cpp
    StyleHelper.h
)


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Assignment.cpp")
    list(APPEND SOURCES Assignment.cpp)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(LMS_Qt_App ${SOURCES})

target_link_libraries(LMS_Qt_App PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

if(WIN32)
    set_target_properties(LMS_Qt_App PROPERTIES WIN32_EXECUTABLE OFF) # Set ON to hide console
endif()

target_compile_definitions(LMS_Qt_App PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

file(COPY 
    ${CMAKE_CURRENT_SOURCE_DIR}/admins.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/teachers.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/students.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/courses.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/assignments.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/submissions.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
