# Include FetchContent module
include(FetchContent)

# Download and unpack GoogleTest at configure time
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
# Make available Googletest
FetchContent_MakeAvailable(googletest)

# Add test source files
set(TEST_SOURCES
    test.cpp
)

# Link GoogleTest to the tests
add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests gtest gtest_main)

# Register tests
add_test(NAME runTests COMMAND runTests)