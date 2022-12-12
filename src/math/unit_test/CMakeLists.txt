add_executable(TestIntegrate Test_integrate.cpp)

target_link_libraries(TestIntegrate ${CONAN_LIBS} PhysxLib)
target_include_directories(
    TestIntegrate PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)

add_test(NAME Test_integrate
         COMMAND TestIntegrate)
