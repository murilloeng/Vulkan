//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//vulkan
#include "Vulkan/inc/Window.hpp"

int main(void)
{
	//data
	Window window;
	//start
	try{ window.start(); }
	catch(const std::exception& e)
	{
		printf("%s\n", e.what());
		return EXIT_FAILURE;
	}
	//retun
	return EXIT_SUCCESS;
}