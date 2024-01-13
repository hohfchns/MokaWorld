cmake_minimum_required(VERSION 3.10)
project(MokaWorld)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

include("${PROJECT_SOURCE_DIR}/cmake/helpers.cmake")

set(MOKA_WORLD_DEPS MOKA_ECS MOKA_UTILS)
DEPENDENCIES(MOKA_WORLD)

include_directories("inc/")
include_directories(${MOKA_WORLD_DEP_INCS})

file(
  GLOB SOURCE_FILES 
  "src/moka/world/components/transform.cpp"
)

file (
  GLOB TESTS_SOURCE_FILES
)

add_library(MokaWorld STATIC ${SOURCE_FILES})
target_link_libraries(MokaWorld ${MOKA_WORLD_DEP_LIBS})
# add_link_options(-no-pie)

set_property(TARGET MokaWorld PROPERTY CXX_STANDARD 20)
set_property(TARGET MokaWorld PROPERTY CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")

if (DEFINED UNIT_TESTS)
  if (${TESTS_SOURCE_FILES})
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    if (WIN32)
      # For Windows: Prevent overriding the parent project's compiler/linker settings
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
      FetchContent_MakeAvailable(googletest)
    endif()

    enable_testing()

    include(GoogleTest)
    add_executable(MokaWorld_UnitTests ${TESTS_SOURCE_FILES})
    target_link_libraries(MokaWorld_UnitTests gtest ${MOKA_WORLD_DEP_LIBS} MokaWorld)
    set_property(TARGET MokaWorld_UnitTests PROPERTY CXX_STANDARD 20)
    set_property(TARGET MokaWorld_UnitTests PROPERTY CXX_STANDARD_REQUIRED 20)

    gtest_discover_tests(MokaWorld_UnitTests)
  endif()
endif()

set_property(GLOBAL PROPERTY MOKA_WORLD 1)
set_property(GLOBAL PROPERTY MOKA_WORLD_INCS "${CMAKE_CURRENT_SOURCE_DIR}/inc")
set_property(GLOBAL PROPERTY MOKA_WORLD_LIBS MokaWorld)
