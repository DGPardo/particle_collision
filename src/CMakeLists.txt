# Physics Library
file(GLOB SRC_MATH      "math/*.cpp")
file(GLOB SRC_GEOMETRY  "geometry/*.cpp")
file(GLOB SRC_PHYSICS   "physics/*.cpp")
file(GLOB SRC_RENDERING "rendering/*.cpp")

add_library(
    PhysxLib SHARED
    ${SRC_MATH}
    ${SRC_GEOMETRY}
    ${SRC_PHYSICS}
    ${SRC_RENDERING}
)

target_link_libraries(PhysxLib PRIVATE ${CONAN_LIBS})

target_include_directories(
    PhysxLib PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)

# Unit test
add_subdirectory(geometry/unit_test)
add_subdirectory(physics/unit_test)

