# Main Executable
add_executable(BoxedSpheres boxed_spheres.cpp)

target_link_libraries(BoxedSpheres PRIVATE ${CONAN_LIBS} PhysxLib)

target_include_directories(
    BoxedSpheres PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)
