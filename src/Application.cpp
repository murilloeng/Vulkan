//std
#include <set>
#include <limits>
#include <fstream>
#include <cstdint>
#include <stdexcept>
#include <algorithm>

//vulkan
#include "Vulkan/inc/Application.hpp"

//constructor
Application::Application(void) : m_width(800), m_height(800), m_physical_device(VK_NULL_HANDLE)
{
	return;
}

//run
void Application::run(void)
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
void Application::setup_glfw(void)
{
	//setup
	glfwInit();
	//window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(m_width, m_height, "Vulkan", nullptr, nullptr);
}
void Application::setup_vulkan(void)
{
	create_instance();
	create_physical_device();
}

//loop
void Application::main_loop(void)
{
	while(!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
	}
}

//cleanup
void Application::cleanup(void)
{
	//instance
	vkDestroyInstance(m_instance, nullptr);
	//window
	glfwDestroyWindow(m_window);
	//cleanup
	glfwTerminate();
}

//create
void Application::create_instance(void)
{
	//data
	VkApplicationInfo app_info;
	VkInstanceCreateInfo instance_info;
	//extensions
	uint32_t glfw_extension_count = 0;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
	std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
	//application
	app_info.pEngineName = "No Engine";
	app_info.apiVersion = VK_API_VERSION_1_0;
	app_info.pApplicationName = "Hello Triangle";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	//instance
	instance_info.pNext = nullptr;
	instance_info.enabledLayerCount = 0;
	instance_info.pApplicationInfo = &app_info;
	instance_info.ppEnabledExtensionNames = extensions.data();
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.enabledExtensionCount = (uint32_t) extensions.size();
	//create
	if(vkCreateInstance(&instance_info, nullptr, &m_instance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create instance!");
	}
}
void Application::create_physical_device(void)
{
	//count
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
	//check
	if(device_count == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}
	//search
	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());
	//choose
	for(const VkPhysicalDevice& device : devices)
	{
		if(check_physical_device(device))
		{
			m_physical_device = device;
			break;
		}
	}
	//check
	if(m_physical_device == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

//physical device
bool Application::check_physical_device(VkPhysicalDevice device)
{
	return true;
}