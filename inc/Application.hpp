#pragma once

//std
#include <vector>
#include <string>
#include <optional>

//glfw
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> device_extensions = 
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct queue_family_indices
{
	//data
	std::optional<uint32_t> m_family_present;
	std::optional<uint32_t> m_family_graphics;
	//check
	bool check_family_indexes(void)
	{
		return m_family_graphics.has_value() && m_family_present.has_value();
	}
};

struct swap_chain_support_details
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
	void create_surface(void);
	void create_instance(void);
	void create_swap_chain(void);
	void create_image_views(void);
	void create_render_pass(void);
	void create_command_pool(void);
	void create_sync_objects(void);
	void create_framebuffers(void);
	void create_logical_device(void);
	void create_command_buffer(void);
	void create_physical_device(void);
	void create_graphics_pipeline(void);
	VkShaderModule create_shader_module(const std::vector<char>&);

	//physical device
	bool check_device_features(VkPhysicalDevice);
	bool check_device_extension_support(VkPhysicalDevice);
	swap_chain_support_details query_swap_chain_support(VkPhysicalDevice);

	static std::vector<char> read_file(const std::string&);
	queue_family_indices find_queue_families(VkPhysicalDevice);
	static std::vector<const char*> get_required_extensions(void);
	VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR&);
	static VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>&);
	static VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>&);

	void draw_frame(void);
	void record_command_buffer(VkCommandBuffer, uint32_t);

	//data
	GLFWwindow* m_window;
	VkInstance m_instance;
	VkSurfaceKHR m_surface;

	VkQueue present_queue;
	VkQueue graphics_queue;

	VkCommandPool m_command_pool;
	VkCommandBuffer m_command_buffer;

	VkDevice m_device;
	VkPhysicalDevice m_physical_device;

	VkPipeline m_pipeline;
	VkRenderPass m_render_pass;
	VkPipelineLayout m_pipeline_layout;

	VkFence m_fence;
	VkSemaphore m_image_available_semaphore;
	VkSemaphore m_render_finished_semaphore;

	VkSwapchainKHR m_swap_chain;
	VkExtent2D m_swap_chain_extent;
	VkFormat m_swap_chain_image_format;
	std::vector<VkImage> m_swap_chain_images;
	std::vector<VkImageView> m_swap_chain_image_views;
	std::vector<VkFramebuffer> m_swap_chain_framebuffers;
};