project(engine)


add_executable(engine ../glad/src/glad.c
		main.cpp
		shaders/src/ShadersPath.cpp
		application.cpp
		shaders/shader.cpp
		shaders/program.cpp
		managers/ProgramManager.cpp
		managers/TextureManager.cpp
		assets/AssetsPath.cpp
		core/camera.cpp
		ECS/components/lights/point_light.cpp
		ECS/components/transform.cpp
		ECS/ecs.cpp
		ECS/entity.cpp
)


target_include_directories(engine PUBLIC ../glfw/include/)
target_include_directories(engine PUBLIC ../glad/include/)
target_include_directories(engine PUBLIC ../engine/ )
target_include_directories(engine PUBLIC ../ )

target_link_libraries(engine glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(engine OpenGL::GL)

