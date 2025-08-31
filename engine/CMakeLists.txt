project(engine)


add_executable(engine ../glad/src/glad.c
		main.cpp
		shaders/ShadersPath.cpp
		application.cpp
		core/shader.cpp)


target_include_directories(engine PUBLIC ../glfw/include/)
target_include_directories(engine PUBLIC ../glad/include/)
target_include_directories(engine PUBLIC ../engine/ )

target_link_libraries(engine glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(engine OpenGL::GL)

