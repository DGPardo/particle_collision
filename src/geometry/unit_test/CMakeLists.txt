add_executable(TestTriangleAlgo Test_triangle_algo.cpp)

target_link_libraries(TestTriangleAlgo ${CONAN_LIBS} PhysxLib)
target_include_directories(
    TestTriangleAlgo PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)

add_test(NAME Test_triangle_algo
         COMMAND TestTriangleAlgo)


################################################################################


add_executable(TestPolygon Test_polygon.cpp)

target_link_libraries(TestPolygon ${CONAN_LIBS} PhysxLib)
target_include_directories(
    TestPolygon PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)

add_test(NAME TestPolygon
         COMMAND TestPolygon)


################################################################################


add_executable(TestCoordTransformation Test_coordinate_transformation.cpp)

target_link_libraries(TestCoordTransformation ${CONAN_LIBS} PhysxLib)
target_include_directories(
    TestCoordTransformation PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)

add_test(NAME TestCoordTransformation
        COMMAND TestCoordTransformation)
