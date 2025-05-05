cmake_minimum_required(VERSION 3.10)
project(computer_club)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Общие исходники (без main.cpp)
file(GLOB CORE_SOURCES 
    "src/ClubManager.cpp"
    "src/Event.cpp"
    "src/CustomTime.cpp"
    "src/TableManager.cpp"
    "src/ClientManager.cpp"
    "src/ErrorHandler.cpp"
    "src/TableStats.cpp"
)

include_directories(include)

# Основной исполняемый файл (только с main.cpp)
add_executable(computer_club src/main.cpp ${CORE_SOURCES})

# Тестовый исполняемый файл (только с тестами)
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(computer_club_tests ${TEST_SOURCES} ${CORE_SOURCES})
target_link_libraries(computer_club_tests ${GTEST_LIBRARIES} pthread)