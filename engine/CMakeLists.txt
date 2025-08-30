project(engine)


add_executable(engine main.cpp application.cpp)

target_include_directories(engine PUBLIC ../glfw/include/)

target_link_libraries(engine glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(engine OpenGL::GL)

