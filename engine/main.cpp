#include "application.h"
#include <chrono>
#include <memory>
#include <thread>
#include <iostream>


int main() {
  

	std::unique_ptr<application> _application;
	_application = std::make_unique<application>();

	if ( !_application->Initialize() )
	{
		std::cout << "Failed to initialize application!" << std::endl;
		return 0;
	}

	const double FPS = 120.0;
	double frame_duration = 1. / FPS;

	double counter_start = glfwGetTime();
	int frame_count = 0;

	while ( ! _application->IsFinished() )
	{	
		double frame_start = glfwGetTime();
		_application->Run();
		double frame_end = glfwGetTime();
		double frame_difference = frame_end - frame_start;

		if ( frame_difference < frame_duration 	)
			std::this_thread::sleep_for(std::chrono::milliseconds(int(frame_duration * 1000 - frame_difference * 1000)));
		frame_count ++;

                if (frame_end - counter_start > 1.0) {
                  std::cout << "FPS: " << frame_count << std::endl;
                  frame_count = 0;
                  counter_start = frame_end;
                }
        }

	_application->Finalize();

  	return 0;
}
