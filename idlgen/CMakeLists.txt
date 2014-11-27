option(USE_SYSTEM_LIBCLANG "Set to ON to use the system libclang library" OFF)
set(PATH_TO_LLVM_ROOT "" CACHE PATH "Path to the root of an LLVM+Clang binary distribution")
set(EXTERNAL_LIBCLANG_PATH "" CACHE PATH "Path to teh libclang library to use")

# Try to be nice and auto-download clang if it's not present.
if (NOT USE_SYSTEM_LIBCLANG AND
        NOT PATH_TO_LLVM_ROOT AND
        NOT EXTERNAL_LIBCLANG_PATH)
    set(CLANG_URL "http://llvm.org/releases/3.5.0")

    if (APPLE)
        set(CLANG_DIRNAME "clang+llvm-3.5.0-macosx-apple-darwin")
        set(CLANG_MD5 "e580c8b8c64ca5442ac15a4343447500")
    else()
        set(CLANG_DIRNAME "clang+llvm-3.5.0-x86_64-linux-gnu-ubuntu-14.04")
        set(CLANG_MD5 "904e96e6f51e57b58de546a7324910c3")
    endif()
    set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")

    if (NOT EXISTS ${CLANG_FILENAME})
        set(need_download true)
    else()
        file(MD5 ${CLANG_FILENAME} actual_md5)
        if (NOT ${actual_md5} STREQUAL ${CLANG_MD5})
            set(need_download true)
        endif()
    endif()

    if (${need_download})
        message("Downloading Clang 3.5")
        file(
            DOWNLOAD "${CLANG_URL}/${CLANG_FILENAME}" "./${CLANG_FILENAME}"
            SHOW_PROGRESS EXPECTED_MD5 "${CLANG_MD5}"
        )

        message("Extracting Clang 3.5")
        execute_process(COMMAND tar -Jxf ${CLANG_FILENAME})
    endif()

    message("searching ${CMAKE_CURRENT_BINARY_DIR}")
    execute_process(COMMAND
        find ${CMAKE_CURRENT_BINARY_DIR}/.. -maxdepth 1 -type d -name clang*
        OUTPUT_VARIABLE PATH_TO_LLVM_ROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

if (NOT USE_SYSTEM_LIBCLANG AND
        NOT PATH_TO_LLVM_ROOT AND
        NOT EXTERNAL_LIBCLANG_PATH)
    message(FATAL_ERROR "We can't find an LLVM+Clang binary distribution to use.")
endif()

if (PATH_TO_LLVM_ROOT)
    set(CLANG_INCLUDES_DIR "${PATH_TO_LLVM_ROOT}/include")
else()
    set(CLANG_INCLUDES_DIR "${CMAKE_SOURCE_DIR}/llvm/include")
endif()

if (NOT IS_ABSOLUTE "${CLANG_INCLUDES_DIR}")
    get_filename_component(CLANG_INCLUDES_DIR "${CMAKE_BINARY_DIR}/${CLANG_INCLUDES_DIR}" ABSOLUTE)
endif()

if (NOT EXTERNAL_LIBCLANG_PATH AND PATH_TO_LLVM_ROOT)
    find_library(TEMP NAMES clang libclang
        PATHS "${PATH_TO_LLVM_ROOT}/lib"
        NO_DEFAULT_PATH )
    set(EXTERNAL_LIBCLANG_PATH ${TEMP})
endif()

set(LIBCLANG_TARGET ${EXTERNAL_LIBCLANG_PATH})
message("Using clang library: ${LIBCLANG_TARGET}")

# This is a workaround for a CMake bug with include_directories(SYSTEM ...)
# on Mac OS X. Bug report: http://public.kitware.com/Bug/view.php?id=10837
if (APPLE)
  set( CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem " )
endif()

include_directories(SYSTEM ${CLANG_INCLUDES_DIR})

# Now build our tools
add_executable(idlgen AstParser.cpp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
target_link_libraries(idlgen ${LIBCLANG_TARGET})
