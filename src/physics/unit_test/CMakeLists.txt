add_executable(TestCollision Test_collision.cpp)

target_link_libraries(TestCollision ${CONAN_LIBS} PhysxLib)
target_include_directories(
    TestCollision PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)

add_test(NAME Test_collision
         COMMAND TestCollision)
