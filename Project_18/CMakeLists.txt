cmake_minimum_required(VERSION 3.15)
project(Project_18)

# Явно указываем путь к Python
#set(Python_ROOT_DIR "C:/Users/User/AppData/Local/Programs/Python/Python313")
#set(pybind11_ROOT "C:/Users/User/Desktop/C++/pybind11")

set(Python_ROOT_DIR "C:/Users/User/AppData/Local/Programs/Python/Python312")
set(pybind11_ROOT "C:/Users/User/AppData/Local/Programs/Python/Python312/Lib/site-packages/pybind11")

# Найти Python с Interpreter
find_package(Python 3 REQUIRED COMPONENTS Development Interpreter)
find_package(pybind11 CONFIG)

#add_subdirectory(pybind11)
# Создать модуль
pybind11_add_module(particle
    src/particle_binding.cpp
    include/particle.h
)

# Настройка путей
target_include_directories(particle PRIVATE
    ${Python_INCLUDE_DIRS}
    include/
)

# Линковка
target_link_libraries(particle PRIVATE 
    ${Python_LIBRARIES}
)

# Выходной путь
set_target_properties(particle PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/python"
    PREFIX ""
    SUFFIX ".pyd"
)