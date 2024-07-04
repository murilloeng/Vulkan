#pragma once

//std
#include <vector>
#include <string>
#include <optional>

//glfw
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct queue_family_indices
{
	std::optional<uint32_t> m_family_graphics;
	std::optional<uint32_t> m_family_presentation;
};


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
	void create_surface(void);
	void create_instance(void);
	void create_logical_device(void);
	void create_physical_device(void);

	//physical device
	bool check_physical_device(VkPhysicalDevice);
	bool check_extension_support(VkPhysicalDevice);
	queue_family_indices find_queue_families(VkPhysicalDevice);

	//data
	unsigned m_width;
	unsigned m_height;

	//instances
	GLFWwindow* m_window;
	VkInstance m_instance;
	VkSurfaceKHR m_surface;

	//queues
	VkQueue m_queue_graphics;
	VkQueue m_queue_presentation;

	//devices
	VkDevice m_device;
	VkPhysicalDevice m_physical_device;

	//extensions
	static const std::vector<const char*> m_extensions;
};