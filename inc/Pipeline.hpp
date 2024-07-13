#pragma once

//std
#include <string>
#include <vector>

//glfw
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Device;

class Pipeline
{
public:
	//constructors
	Pipeline(GLFWwindow*, std::string, std::string);

	//destructor
	~Pipeline(void);

	//read
	static void read_file(std::vector<char>&, std::string);

	//data
	Device* m_device;
	VkPipeline m_pipeline;
	std::string m_path_vertex;
	std::string m_path_fragment;
	VkShaderModule m_shader_vertex;
	VkShaderModule m_shader_fragment;
};