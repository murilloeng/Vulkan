#pragma once

//std
#include <vector>
#include <string>
#include <optional>

//glfw
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Pipeline;

class AppGalea
{
public:
	//constructor
	AppGalea(void);

	//run
	void run(void);

	//setup
	void setup_glfw(void);
	void setup_vulkan(void);

	//loop
	void main_loop(void);

	//cleanup
	void cleanup(void);

	//data
	unsigned m_width;
	unsigned m_height;

	Pipeline* m_pipeline;
	GLFWwindow* m_window;
};