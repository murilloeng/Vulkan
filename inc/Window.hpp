#pragma once

//std
#include <optional>

//ext
#include <vulkan/vulkan.h>
#include "external/cpp/inc/GLFW/glfw3.h"

struct queue_family_indices
{
	std::optional<uint32_t> m_graphics_family;
	bool is_complete(void) { return m_graphics_family.has_value(); }
};

class Window
{
public:
	//constructor
	Window(void);

	//destructor
	~Window(void);

	//start
	void start(void);

private:
	//instance
	void create_instance(void);

	//physical device
	void pick_physical_device(void);
	bool check_physical_device(VkPhysicalDevice);

	//queue
	queue_family_indices find_queue_families(VkPhysicalDevice);

	//data
	GLFWwindow* m_window;
	VkInstance m_instance;
	VkPhysicalDevice m_physical_device;
};