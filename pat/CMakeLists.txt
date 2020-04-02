set (CONFIG_H ${CMAKE_BINARY_DIR}/gnucash/common/config.h )
message("CONFIG_H=${CONFIG_H}")

set(pat_SOURCES
  sess.cpp
  book-patcher.cpp
)

set(pat_HEADERS
  sess.hpp
  book-patcher.hpp
)

set(pat_noinst_HEADERS
  ${CONFIG_H}
)

# target_include_directories(hello
#   PUBLIC
#   ${GNC_LIBGNC_SOURCE_DIR}/backend/xml
#   ${GNC_LIBGNC_SOURCE_DIR}/app-utils
#   ${GNC_LIBGNC_SOURCE_DIR}/core-utils
#   ${GNC_LIBGNC_SOURCE_DIR}/gnc-module
#   ${GNC_LIBGNC_SOURCE_DIR}/engine
# )

# set_source_files_properties (${pat_SOURCES} PROPERTIES OBJECT_DEPENDS ${CONFIG_H})

# target_link_libraries(hello
#   gncmod-engine
#   gncmod-app-utils
#   # gncmod-backend-xml
#   gncmod-backend-xml-utils
#   gnc-backend-xml-utils
#   gnc-core-utils
#   gnc-module
# )

# target_link_libraries(hello gncmod-gnome-search gncmod-ledger-core gncmod-report-gnome gncmod-report-system
#    gncmod-register-gnome gncmod-register-core gncmod-gnome-utils gncmod-engine)

set(pat_ALL_SOURCES ${pat_SOURCES} ${pat_HEADERS} ${pat_noinst_HEADERS})

set(pat_ALL_LIBRARIES
  gncmod-engine gnc-module gncmod-backend-xml-utils
  ${GIO_LDFLAGS} ${LIBXML2_LDFLAGS} ${LIBXSLT_LDFLAGS}
)

set(pat_ALL_INCLUDES ${GIO_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS}
    ${LIBXSLT_INCLUDE_DIRS} ${GNC_SRC_DIR}/libgnucash/backend ${GNC_SRC_DIR}/libgnucash/engine)

set_source_files_properties (${pat_SOURCES} PROPERTIES OBJECT_DEPENDS ${CONFIG_H})

add_library (pat ${pat_ALL_SOURCES})
target_link_libraries(pat ${pat_ALL_LIBRARIES})
target_include_directories (pat
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${pat_ALL_INCLUDES}
)


# TEST

add_subdirectory(test)
