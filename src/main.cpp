//vulkan
#include "Vulkan/inc/AppGalea.hpp"
#include "Vulkan/inc/Application.hpp"

int main(void)
{
	//application
	Application app;
	try { app.run(); }
	catch (const std::exception& exception) 
	{
		fprintf(stderr, "%s\n", exception.what());
		return EXIT_FAILURE;
	}
	//retun
	return EXIT_SUCCESS;
}