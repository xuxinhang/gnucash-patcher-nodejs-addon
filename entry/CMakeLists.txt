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


# Required files for Node.js Addons

add_definitions(-DNAPI_VERSION=3)
set(CMAKE_JS_INC ~/.cmake-js/node-x64/v10.15.2/include/node CACHE PATH "cmake-js-inc")
set(CMAKE_JS_SRC "" CACHE PATH "cmake-js-src")
set(CMAKE_JS_LIB "" CACHE PATH "cmake-js-lib")
set(NAPI_WRAPPER_DIR ${CMAKE_SOURCE_DIR}/node_modules/node-addon-api CACHE PATH "NAPI dir")
# execute_process(COMMAND node -p "require('node-addon-api').include"
#   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#   OUTPUT_VARIABLE NAPI_WRAPPER_DIR
# )
# string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NAPI_WRAPPER_DIR})
# string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NAPI_WRAPPER_DIR})
message("CMAKE_JS_INC=${CMAKE_JS_INC}")
message("CMAKE_JS_SRC=${CMAKE_JS_SRC}")
message("CMAKE_JS_LIB=${CMAKE_JS_LIB}")
message("NAPI_DIR=${NAPI_WRAPPER_DIR}")


# Node.js C++ Addons (using cmake.js)

set(NODE_ADDON_SOURCES node-addon.cpp)
# set(NODE_ADDON_HEADERS node-addon.hpp)

add_library(node-addon SHARED ${NODE_ADDON_SOURCES} ${CMAKE_JS_SRC})
set_target_properties(node-addon PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(node-addon
  PUBLIC
  # ${NODE_ADDON_HEADERS}
  ${CMAKE_JS_INC}
  # ${pat_ALL_INCLUDES} # temporary
)
target_link_libraries(node-addon ${CMAKE_JS_LIB} pat)
target_include_directories(node-addon
  PRIVATE ${NAPI_WRAPPER_DIR}
  # PUBLIC ${PAT_SOURCE_DIR}
)
