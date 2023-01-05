# Main Executable
add_executable(EmptyBox empty_box.cpp)

target_link_libraries(EmptyBox PRIVATE ${CONAN_LIBS} PhysxLib)

target_include_directories(
    EmptyBox PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)
