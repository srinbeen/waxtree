cmake_minimum_required(VERSION 3.31)
project(Waxtree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/lib/SQLiteCpp)
add_subdirectory(src/lib/tabulate)


add_library(WaxtreeDB)
target_sources(WaxtreeDB
  PRIVATE
    src/core/wtdb.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      src/core/include
    FILES
      src/core/include/wtdb.hpp
)
target_include_directories(WaxtreeDB
	PUBLIC
	src/core/include
)
target_link_libraries(WaxtreeDB 
	PUBLIC 
	SQLiteCpp
)


add_executable(WaxtreeApp)
target_sources(WaxtreeApp
	PRIVATE
	src/app/main.cpp
	src/app/wtapp.cpp
)
target_include_directories(WaxtreeApp
	PRIVATE
	src/app/include
)
target_link_libraries(WaxtreeApp
	PRIVATE
	WaxtreeDB
	tabulate
)
