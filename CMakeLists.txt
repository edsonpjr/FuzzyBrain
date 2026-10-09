cmake_minimum_required(VERSION 3.15)
project(fuzzybrain VERSION 1.0 LANGUAGES CXX)

# Monitor .clang-tidy-config for changes - forces reconfiguration if file changes
set_directory_properties(PROPERTIES CMAKE_CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/.clang-tidy-config")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/headers)

# Recursively find all C++ source files in the src directory
file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.cpp"
)

add_executable(fuzzybrain ${PROJECT_SOURCES})

    # Ensure exception handling is enabled for all compilers used by this project.
    # For MSVC we use /EHsc. For Clang/GCC use -fexceptions and -fcxx-exceptions where applicable.
    target_compile_options(fuzzybrain PRIVATE
        $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/EHsc>
        $<$<COMPILE_LANG_AND_ID:CXX,Clang>:-fexceptions;-fcxx-exceptions>
        $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-fexceptions>
    )

# Post-build step to copy resource files into the output binary folder
add_custom_command(TARGET fuzzybrain POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/src/resources"
    "$<TARGET_FILE_DIR:fuzzybrain>/resources"
)

# ============================================================================ 
# DOXYGEN CONFIGURATION (WITH DIAGRAMS FOR WINDOWS & LINUX)
# ============================================================================
find_package(Doxygen)

# Windows fallback: If standard find_package fails, look into default installation paths
if(NOT DOXYGEN_FOUND AND WIN32)
    find_program(DOXYGEN_EXECUTABLE doxygen PATHS "C:/Program Files/doxygen/bin")
    if(DOXYGEN_EXECUTABLE)
        set(DOXYGEN_FOUND TRUE)
    endif()
endif()

if(DOXYGEN_FOUND)
    # Basic project information and output format settings
    set(DOXYGEN_PROJECT_NAME "FuzzyBrain")
    set(DOXYGEN_GENERATE_HTML "YES")
    set(DOXYGEN_GENERATE_LATEX "NO")
    
    # Stores the generated documentation into the "docs" directory at the project root
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs")
    
    # Code extraction settings (Ensures undocumented and private members are processed)
    set(DOXYGEN_RECURSIVE "YES")
    set(DOXYGEN_EXTRACT_ALL "YES")
    set(DOXYGEN_EXTRACT_PRIVATE "YES")
    set(DOXYGEN_EXTRACT_STATIC "YES")
    
    # ------------------------------------------------------------------------
    # GRAPHVIZ / DOT DIAGRAMS CONFIGURATION
    # ------------------------------------------------------------------------
    set(DOXYGEN_HAVE_DOT "YES")               # Enables Graphviz dot tool engine
    set(DOXYGEN_UML_LOOK "YES")               # Applies standard UML styling to diagrams
    set(DOXYGEN_CLASS_GRAPH "YES")            # Generates class inheritance diagrams
    set(DOXYGEN_COLLABORATION_GRAPH "YES")    # Generates member aggregation relationship graphs
    set(DOXYGEN_INCLUDE_GRAPH "YES")          # Generates #include dependency graphs
    set(DOXYGEN_CALL_GRAPH "YES")             # Generates function call graphs (who this function calls)
    set(DOXYGEN_CALLER_GRAPH "YES")           # Generates caller graphs (who calls this function)
    
    # Performance optimization: Uses multi-threading for faster diagram rendering
    set(DOXYGEN_DOT_NUM_THREADS 4)          
    
    # Windows fallback: If dot engine is not in system PATH, explicitly locate it
    if(WIN32)
        find_program(DOT_EXECUTABLE dot PATHS "C:/Program Files/Graphviz/bin")
        if(DOT_EXECUTABLE)
            get_filename_component(DOT_DIR_PATH "${DOT_EXECUTABLE}" DIRECTORY)
            set(DOXYGEN_DOT_PATH "${DOT_DIR_PATH}")
        endif()
    endif()

    # Creates the custom target named 'docs'
    # The 'ALL' keyword forces Visual Studio / Ninja to include it in the main build pipeline
    doxygen_add_docs(
        docs
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        ALL
        COMMENT "Generating Doxygen documentation with Graphviz diagrams..."
    )
    
    # Enforces target execution order: Generates documentation automatically on every project build
    add_dependencies(fuzzybrain docs)
else()
    message(STATUS "Doxygen not found. The 'docs' target will not be created.")
endif()

# =============================================================================
# CODE QUALITY: CLANG-FORMAT & CLANG-TIDY (C++ Core Guidelines Validation)
# =============================================================================

# --- CLANG-FORMAT Configuration ---
find_program(CLANG_FORMAT_EXE NAMES "clang-format")

if(CLANG_FORMAT_EXE)
    message(STATUS "Clang-Format found: ${CLANG_FORMAT_EXE}")

    # Target to check code formatting without modifying files
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Checking C++ code formatting against .clang-format rules"
        VERBATIM
    )

    # Target to automatically format all source files
    add_custom_target(format-fix
        COMMAND ${CLANG_FORMAT_EXE} -i ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Auto-formatting C++ sources according to .clang-format rules"
        VERBATIM
    )
else()
    message(WARNING "Clang-Format executable not found! Install clang-tools to enable format validation.")
endif()

# --- CLANG-TIDY Configuration ---
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
    message(STATUS "Clang-Tidy found: ${CLANG_TIDY_EXE}")

    # Read clang-tidy config from file (always re-read, never cached)
    file(READ "${CMAKE_SOURCE_DIR}/.clang-tidy-config" ENABLE_CLANG_TIDY)
    string(STRIP "${ENABLE_CLANG_TIDY}" ENABLE_CLANG_TIDY)

    message(STATUS "Clang-Tidy config (from .clang-tidy-config): ${ENABLE_CLANG_TIDY}")

    if(ENABLE_CLANG_TIDY STREQUAL "ON")
        message(STATUS "? Clang-Tidy ENABLED - will run after build")
        # Add clang-tidy as a post-build step (works with MSVC/Ninja)
        add_custom_command(
            TARGET fuzzybrain 
            POST_BUILD
            COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} --header-filter=.* ${PROJECT_SOURCES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running Clang-Tidy analysis..."
            VERBATIM
        )
    else()
        message(STATUS "? Clang-Tidy DISABLED - edit .clang-tidy-config to enable")
    endif()

    # Standalone target to run clang-tidy on project sources
    # Usage: cmake --build out/build --target clang-tidy-check
    add_custom_target(clang-tidy-check
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} --header-filter=.* --warnings-as-errors=* ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy for C++ Core Guidelines validation"
        VERBATIM
    )
else()
    message(WARNING "Clang-Tidy executable not found! Install clang-tools to enable linting.")
endif()

# --- Consolidated Lint Target ---
if(CLANG_FORMAT_EXE AND CLANG_TIDY_EXE)
    add_custom_target(lint
        COMMAND ${CMAKE_COMMAND} -E echo "========== Checking Code Formatting =========="
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror ${PROJECT_SOURCES} || true
        COMMAND ${CMAKE_COMMAND} -E echo "========== Running Clang-Tidy Analysis =========="
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} --header-filter=.* ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running full code quality checks (format + tidy)"
        VERBATIM
    )
elseif(CLANG_FORMAT_EXE)
    add_custom_target(lint
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running code format validation"
        VERBATIM
    )
elseif(CLANG_TIDY_EXE)
    add_custom_target(lint
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} --header-filter=.* ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy analysis"
        VERBATIM
    )
endif()
