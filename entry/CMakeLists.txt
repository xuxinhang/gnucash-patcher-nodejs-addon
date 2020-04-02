
add_executable (hello main.cpp)
target_link_libraries (hello
  pat
  ${pat_ALL_LIBRARIES} # temporary
)

target_include_directories (hello
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${PAT_SOURCE_DIR}
  ${pat_ALL_INCLUDES} # temporary
)
