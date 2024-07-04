#pragma once

//std
#include <vector>
#include <string>
#include <optional>

//glfw
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Application
{
public:
	//constructor
	Application(void);

	//run
	void run(void);

private:
	//setup
	void setup_glfw(void);
	void setup_vulkan(void);

	//loop
	void main_loop(void);

	//cleanup
	void cleanup(void);

	//create
	void create_instance(void);
	void create_physical_device(void);

	//physical device
	bool check_physical_device(VkPhysicalDevice);

	//data
	unsigned m_width;
	unsigned m_height;
	GLFWwindow* m_window;
	VkInstance m_instance;
	VkPhysicalDevice m_physical_device;
};