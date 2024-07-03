//vulkan
#include "Vulkan/inc/Application.hpp"

int main(void)
{
	//data
	Application app;
	//application
	try { app.run(); }
	catch (const std::exception& exception) 
	{
		fprintf(stderr, "%s\n", exception.what());
		return EXIT_FAILURE;
	}
	//retun
	return EXIT_SUCCESS;
}