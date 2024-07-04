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
	create_surface();
	create_physical_device();
	create_logical_device();
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
	//device
	vkDestroyDevice(m_device, nullptr);
	//surface
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	//instance
	vkDestroyInstance(m_instance, nullptr);
	//window
	glfwDestroyWindow(m_window);
	//cleanup
	glfwTerminate();
}

//create
void Application::create_surface(void)
{
	if(glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface!");
	}
}
void Application::create_instance(void)
{
	//data
	VkApplicationInfo app_info;
	VkInstanceCreateInfo instance_create_info{};
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
	instance_create_info.pNext = nullptr;
	instance_create_info.enabledLayerCount = 0;
	instance_create_info.pApplicationInfo = &app_info;
	instance_create_info.ppEnabledExtensionNames = extensions.data();
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.enabledExtensionCount = (uint32_t) extensions.size();
	//create
	if(vkCreateInstance(&instance_create_info, nullptr, &m_instance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create instance!");
	}
}
void Application::create_logical_device(void)
{
	//data
	float queue_priority = 1.0f;
	VkDeviceCreateInfo device_create_info{};
	VkPhysicalDeviceFeatures physiscal_device_features{};
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos(2);
	queue_family_indices indices = find_queue_families(m_physical_device);
	//queue info
	queue_create_infos[0].queueCount = 1;
	queue_create_infos[1].queueCount = 1;
	queue_create_infos[0].pQueuePriorities = &queue_priority;
	queue_create_infos[1].pQueuePriorities = &queue_priority;
	queue_create_infos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_infos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_infos[0].queueFamilyIndex = indices.m_family_graphics.value();
	queue_create_infos[1].queueFamilyIndex = indices.m_family_presentation.value();
	//device info
	device_create_info.enabledLayerCount = 0;
	device_create_info.queueCreateInfoCount = 2;
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pQueueCreateInfos = queue_create_infos.data();
	device_create_info.pEnabledFeatures = &physiscal_device_features;
	device_create_info.ppEnabledExtensionNames = m_extensions.data();
	device_create_info.enabledExtensionCount = (uint32_t) m_extensions.size();
	//create
	if(vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_device) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}
	//queue
	vkGetDeviceQueue(m_device, indices.m_family_graphics.value(), 0, &m_queue_graphics);
	vkGetDeviceQueue(m_device, indices.m_family_presentation.value(), 0, &m_queue_presentation);
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
	//data
	queue_family_indices indices = find_queue_families(device);
	//return
	return 
		indices.m_family_graphics.has_value() && 
		indices.m_family_presentation.has_value() &&
		check_extension_support(device);
}
bool Application::check_extension_support(VkPhysicalDevice device)
{
	//count
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
	//extensions
	std::vector<VkExtensionProperties> available_extensions(extension_count);
	std::set<std::string> required_extensions(m_extensions.begin(), m_extensions.end());
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());
	//check
	for(const VkExtensionProperties& extension : available_extensions)
	{
		required_extensions.erase(extension.extensionName);
	}
	//return
	return required_extensions.empty();
}
queue_family_indices Application::find_queue_families(VkPhysicalDevice device)
{
	//count
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
	//families
	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());
	//search
	uint32_t index = 0;
	queue_family_indices indices;
	VkBool32 presentation_support = false;
	for(const VkQueueFamilyProperties& queue_family : queue_families)
	{
		if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.m_family_graphics = index;
		}
		vkGetPhysicalDeviceSurfaceSupportKHR(device, index, m_surface, &presentation_support);
		if(presentation_support)
		{
			indices.m_family_presentation = index;
		}
		index++;
	}
	//return
	return indices;
}

//static data
const std::vector<const char*> Application::m_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};