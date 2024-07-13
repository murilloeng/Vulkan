//std

//vulkan
#include "Vulkan/inc/AppGalea.hpp"
#include "Vulkan/inc/Pipeline.hpp"

//constructor
AppGalea::AppGalea(void) : m_width(800), m_height(800)
{
	return;
}

//run
void AppGalea::run(void)
{
	//setup
	setup_glfw();
	setup_vulkan();
	//loop
	main_loop();
	//cleanup
	cleanup();
}

//setup
void AppGalea::setup_glfw(void)
{
	//setup
	glfwInit();
	//window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(m_width, m_height, "Vulkan", nullptr, nullptr);
}
void AppGalea::setup_vulkan(void)
{
	m_pipeline = new Pipeline(m_window, "shd/base_vert.spv", "shd/base_frag.spv");
}

//loop
void AppGalea::main_loop(void)
{
	while(!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
	}
}

//cleanup
void AppGalea::cleanup(void)
{
	//window
	glfwDestroyWindow(m_window);
	//cleanup
	glfwTerminate();
}