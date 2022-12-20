# Main Executable
set(APP_NAME BarBarBall)
add_executable(${APP_NAME} bar_bar_ball.cpp)

target_link_libraries(${APP_NAME} PRIVATE ${CONAN_LIBS} PhysxLib)

target_include_directories(
    ${APP_NAME} PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/
)
