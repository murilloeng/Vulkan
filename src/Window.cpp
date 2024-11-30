//std
#include <vector>
#include <stdexcept>

//vulkan
#include "Vulkan/inc/Window.hpp"

#define width 700
#define height 700

//constructor
Window::Window(void) : m_window(nullptr), m_physical_device(VK_NULL_HANDLE)
{
	//setup
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
	//create
	create_instance();
	pick_physical_device();
}

//destructor
Window::~Window(void)
{
	//window
	glfwDestroyWindow(m_window);
	//library
	glfwTerminate();
}

//start
void Window::start(void)
{
	while(!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
	}
}

//instance
void Window::create_instance(void)
{
	//application
	VkApplicationInfo app_info{};
	app_info.pEngineName = "No Engine";
	app_info.pApplicationName = "Vulkan App";
	app_info.apiVersion = VK_API_VERSION_1_0;
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	//extensions
	const char** glfw_extension_names;
	uint32_t glfw_extension_count = 0;
	glfw_extension_names = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
	//instance
	VkInstanceCreateInfo create_info{};
	create_info.enabledLayerCount = 0;
	create_info.pApplicationInfo = &app_info;
	create_info.enabledExtensionCount = glfw_extension_count;
	create_info.ppEnabledExtensionNames = glfw_extension_names;
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	//create
	if(vkCreateInstance(&create_info, nullptr, &m_instance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
	}
}

//physical device
void Window::pick_physical_device(void)
{
	//count
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
	//check
	if(device_count == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}
	//devices
	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());
	//pick
	for(const VkPhysicalDevice& device : devices)
	{
		if(check_physical_device(device))
		{
			m_physical_device = device;
			break;
		}
	}
	if(m_physical_device == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}
bool Window::check_physical_device(VkPhysicalDevice device)
{
	queue_family_indices indices = find_queue_families(device);
	return indices.is_complete();
}

//queue
queue_family_indices Window::find_queue_families(VkPhysicalDevice device)
{
	//data
	int32_t i = 0;
	queue_family_indices indices;
	uint32_t queue_family_count = 0;
	std::vector<VkQueueFamilyProperties> queue_families;
	//query
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
	//properties
	queue_families.resize(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());
	//pick
	for(const VkQueueFamilyProperties& queue_family : queue_families)
	{
		if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.m_graphics_family = i;
		}
		if(indices.is_complete())
		{
			break;
		}
		i++;
	}
	return indices;
}