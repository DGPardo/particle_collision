# Main Executable
add_executable(MyApp boxed_spheres.cpp)

target_link_libraries(MyApp PRIVATE ${CONAN_LIBS} PhysxLib)

target_include_directories(
    MyApp PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)
