set(MINISPRING_SQLITE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SQLite")

if(NOT EXISTS "${MINISPRING_SQLITE_DIR}/sqlite3.c")
    message(FATAL_ERROR
        "SQLite sources not found in ${MINISPRING_SQLITE_DIR}.\n"
        "Run:  git submodule update --init --recursive")
endif()

add_library(minispring_sqlite3 STATIC "${MINISPRING_SQLITE_DIR}/sqlite3.c")

target_include_directories(minispring_sqlite3 PUBLIC
    $<BUILD_INTERFACE:${MINISPRING_SQLITE_DIR}>)

target_compile_definitions(minispring_sqlite3 PRIVATE
    SQLITE_THREADSAFE=1               # serialized mode: one connection used from several threads
    SQLITE_DEFAULT_FOREIGN_KEYS=1     # every connection enforces FOREIGN KEY / ON DELETE CASCADE
    SQLITE_OMIT_LOAD_EXTENSION=1)     # no dlopen dependency

target_compile_options(minispring_sqlite3 PRIVATE
    $<$<C_COMPILER_ID:MSVC>:/w>)      # third-party code: no warnings

find_package(Threads REQUIRED)
target_link_libraries(minispring_sqlite3 PUBLIC Threads::Threads)

file(GLOB_RECURSE MINISPRING_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE MINISPRING_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# vendored code is not ours
list(FILTER MINISPRING_SOURCES EXCLUDE REGEX "/vendor/")
list(FILTER MINISPRING_HEADERS EXCLUDE REGEX "/vendor/")

add_library(MiniSpring STATIC ${MINISPRING_SOURCES} ${MINISPRING_HEADERS})
add_library(MiniSpring::MiniSpring ALIAS MiniSpring)

target_include_directories(MiniSpring PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)   # apps write #include <MiniSpring.h>

target_compile_features(MiniSpring PUBLIC cxx_std_17)

# Tells Core.h that MINISPRING_API must expand to nothing (see below).
# PUBLIC: the app and the library must agree.
target_compile_definitions(MiniSpring PUBLIC MINISPRING_STATIC)

# Public headers may include <sqlite3.h>, so SQLite is PUBLIC.
target_link_libraries(MiniSpring PUBLIC minispring_sqlite3)

if(WIN32)
    # Needed only if the HTTP/WebSocket modules use winsock; harmless otherwise.
    target_link_libraries(MiniSpring PUBLIC ws2_32)
    target_compile_definitions(MiniSpring PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

if(MSVC)
    target_compile_options(MiniSpring PRIVATE /W3 /utf-8)
else()
    target_compile_options(MiniSpring PRIVATE -Wall -Wextra)
endif()

# Keeps the folder structure in the Visual Studio solution explorer.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${MINISPRING_SOURCES} ${MINISPRING_HEADERS})