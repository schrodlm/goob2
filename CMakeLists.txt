cmake_minimum_required(VERSION 3.10)
project(Goob VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
#Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MVSC) 
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wunused -pedantic)
endif()


add_subdirectory(src)
add_subdirectory(sandbox)
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()



