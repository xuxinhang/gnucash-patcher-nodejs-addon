set(PAT_TEST_INCLUDE_DIRS
  ${GNC_SRC_DIR}/libgnucash/backend/xml
  ${GNC_SRC_DIR}/libgnucash/engine
  ${GNC_SRC_DIR}/libgnucash/engine/test-core
  ${GNC_SRC_DIR}/common # for config.h
  ${GNC_SRC_DIR}/common/test-core  # for unittest-support.h
  ${GLIB2_INCLUDE_DIRS}
  ${pat_ALL_INCLUDES} # temporary
)

set(PAT_TEST_LIBS
  ${pat_ALL_LIBRARIES} # temporary
  pat
)


### utils for testing exec

add_library(pat-test-utils test-utils.cpp)
target_link_libraries(pat-test-utils ${PAT_TEST_LIBS})
target_include_directories(pat-test-utils PRIVATE ${PAT_TEST_INCLUDE_DIRS})


# add test exec

macro(pat_add_test _TARGET _SOURCE_FILES)
  add_executable(${_TARGET} ${_SOURCE_FILES})
  target_link_libraries(${_TARGET} ${PAT_TEST_LIBS} pat-test-utils)
  target_include_directories(${_TARGET} PRIVATE ${PAT_TEST_INCLUDE_DIRS})
endmacro(pat_add_test)

pat_add_test(pat-test-book-patcher test-book-patcher.cpp)
