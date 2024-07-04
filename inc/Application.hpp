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
	std::optional<uint32_t> m_family_present;
	std::optional<uint32_t> m_family_graphics;
};
struct swap_chain_support
{
	VkSurfaceCapabilitiesKHR m_capabilities;
	std::vector<VkSurfaceFormatKHR> m_formats;
	std::vector<VkPresentModeKHR> m_present_modes;
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
	void create_images(void);
	void create_surface(void);
	void create_instance(void);
	void create_swap_chain(void);
	void create_image_views(void);
	void create_render_pass(void);
	void create_logical_device(void);
	void create_physical_device(void);
	void create_graphics_pipeline(void);

	//physical device
	bool check_physical_device(VkPhysicalDevice);
	bool check_extension_support(VkPhysicalDevice);
	queue_family_indices query_queue_families(VkPhysicalDevice);

	//shaders
	std::vector<char> read_file(const std::string&);
	VkShaderModule create_shader_module(const std::vector<char>&);

	//swap chain
	swap_chain_support query_swap_chain_support(VkPhysicalDevice);
	VkExtent2D choose_swap_chain_extent(const VkSurfaceCapabilitiesKHR&);
	VkPresentModeKHR choose_swap_chain_present_mode(const std::vector<VkPresentModeKHR>&);
	VkSurfaceFormatKHR choose_swap_chain_surface_format(const std::vector<VkSurfaceFormatKHR>&);

	//data
	unsigned m_width;
	unsigned m_height;

	//instances
	GLFWwindow* m_window;
	VkInstance m_instance;
	VkSurfaceKHR m_surface;

	//queues
	VkQueue m_queue_present;
	VkQueue m_queue_graphics;

	//pipeline
	VkPipelineLayout m_pipeline_layout;

	//swap chain
	VkSwapchainKHR m_swap_chain;
	VkExtent2D m_swap_chain_extent;
	VkFormat m_swap_chain_image_format;
	std::vector<VkImage> m_swap_chain_images;
	std::vector<VkImageView> m_swap_chain_image_views;

	//devices
	VkDevice m_device;
	VkPhysicalDevice m_physical_device;

	//extensions
	static const std::vector<const char*> m_extensions;
};