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
	create_swap_chain();
	create_images();
	create_image_views();
	create_render_pass();
	create_graphics_pipeline();
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
	//image views
	for(VkImageView image_view : m_swap_chain_image_views)
	{
		vkDestroyImageView(m_device, image_view, nullptr);
	}
	//swap chain
	vkDestroySwapchainKHR(m_device, m_swap_chain, nullptr);
	//pipeline
	vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
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
void Application::create_images(void)
{
	//count
	uint32_t image_count;
	vkGetSwapchainImagesKHR(m_device, m_swap_chain, &image_count, nullptr);
	//images
	m_swap_chain_images.resize(image_count);
	vkGetSwapchainImagesKHR(m_device, m_swap_chain, &image_count, m_swap_chain_images.data());
}
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
void Application::create_swap_chain(void)
{
	//data
	VkSwapchainCreateInfoKHR swap_chain_info{};
	queue_family_indices indices = query_queue_families(m_physical_device);
	swap_chain_support support = query_swap_chain_support(m_physical_device);
	uint32_t indices_ptr[] = {indices.m_family_graphics.value(), indices.m_family_present.value()};
	//options
	m_swap_chain_extent = choose_swap_chain_extent(support.m_capabilities);
	VkPresentModeKHR present_mode = choose_swap_chain_present_mode(support.m_present_modes);
	VkSurfaceFormatKHR surface_format = choose_swap_chain_surface_format(support.m_formats);
	//count
	uint32_t image_count = support.m_capabilities.minImageCount + 1;
	if(support.m_capabilities.maxImageCount > 0 && image_count > support.m_capabilities.maxImageCount)
	{
		image_count = support.m_capabilities.maxImageCount;
	}
	//swap chain info
	swap_chain_info.clipped = VK_TRUE;
	swap_chain_info.surface = m_surface;
	swap_chain_info.imageArrayLayers = 1;
	swap_chain_info.presentMode = present_mode;
	swap_chain_info.minImageCount = image_count;
	swap_chain_info.oldSwapchain = VK_NULL_HANDLE;
	swap_chain_info.imageExtent = m_swap_chain_extent;
	swap_chain_info.imageFormat = surface_format.format;
	swap_chain_info.imageColorSpace = surface_format.colorSpace;
	swap_chain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swap_chain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swap_chain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swap_chain_info.preTransform = support.m_capabilities.currentTransform;
	//sharing
	if(indices.m_family_graphics != indices.m_family_present)
	{
		swap_chain_info.queueFamilyIndexCount = 2;
		swap_chain_info.pQueueFamilyIndices = indices_ptr;
		swap_chain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	}
	else
	{
		swap_chain_info.queueFamilyIndexCount = 0;
		swap_chain_info.pQueueFamilyIndices = nullptr;
		swap_chain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	//create
	m_swap_chain_image_format = surface_format.format;
	if(vkCreateSwapchainKHR(m_device, &swap_chain_info, nullptr, &m_swap_chain) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create swap chain!");
	}
}
void Application::create_image_views(void)
{
	//size
	m_swap_chain_image_views.resize(m_swap_chain_images.size());
	//loop
	for(size_t i = 0; i < m_swap_chain_images.size(); i++)
	{
		VkImageViewCreateInfo image_view_create_info{};
		image_view_create_info.image = m_swap_chain_images[i];
		image_view_create_info.subresourceRange.levelCount = 1;
		image_view_create_info.subresourceRange.layerCount = 1;
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.subresourceRange.baseMipLevel = 0;
		image_view_create_info.format = m_swap_chain_image_format;
		image_view_create_info.subresourceRange.baseArrayLayer = 0;
		image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		if(vkCreateImageView(m_device, &image_view_create_info, nullptr, &m_swap_chain_image_views[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create image views!");
		}
	}
}
void Application::create_render_pass(void)
{
	VkAttachmentDescription color_attachment{};
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.format = m_swap_chain_image_format;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
}
void Application::create_logical_device(void)
{
	//data
	float queue_priority = 1.0f;
	VkDeviceCreateInfo device_create_info{};
	VkPhysicalDeviceFeatures physiscal_device_features{};
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos(2);
	queue_family_indices indices = query_queue_families(m_physical_device);
	//queue info
	queue_create_infos[0].queueCount = 1;
	queue_create_infos[1].queueCount = 1;
	queue_create_infos[0].pQueuePriorities = &queue_priority;
	queue_create_infos[1].pQueuePriorities = &queue_priority;
	queue_create_infos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_infos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_infos[0].queueFamilyIndex = indices.m_family_graphics.value();
	queue_create_infos[1].queueFamilyIndex = indices.m_family_present.value();
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
	vkGetDeviceQueue(m_device, indices.m_family_present.value(), 0, &m_queue_present);
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
void Application::create_graphics_pipeline(void)
{
	//data
	VkPipelineLayoutCreateInfo pipeline_layout_info{};
	VkPipelineDynamicStateCreateInfo dynamic_state_info{};
	VkPipelineViewportStateCreateInfo viewport_state_info{};
	VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
	VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
	VkPipelineColorBlendStateCreateInfo color_blend_state_info{};
	VkPipelineMultisampleStateCreateInfo multisample_state_info{};
	VkPipelineVertexInputStateCreateInfo vertex_input_state_info{};
	VkPipelineRasterizationStateCreateInfo rasterizer_state_info{};
	VkPipelineInputAssemblyStateCreateInfo input_assembly_state_info{};
	VkPipelineColorBlendAttachmentState color_blend_attachment_state{};
	std::vector<VkDynamicState> dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	//code
	std::vector<char> code_vert = read_file("shd/base_vert.spv");
	std::vector<char> code_frag = read_file("shd/base_frag.spv");
	//shader modules
	VkShaderModule shader_module_vert = create_shader_module(code_vert);
	VkShaderModule shader_module_frag = create_shader_module(code_frag);
	//pipeline stage create info
	vert_shader_stage_info.pName = "main";
	vert_shader_stage_info.module = shader_module_vert;
	vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//pipeline stage create info
	frag_shader_stage_info.pName = "main";
	frag_shader_stage_info.module = shader_module_frag;
	frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//pipeline create info
	VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};
	//vertex input info
	vertex_input_state_info.vertexBindingDescriptionCount = 0;
	vertex_input_state_info.vertexAttributeDescriptionCount = 0;
	vertex_input_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	//input assembly info
	input_assembly_state_info.primitiveRestartEnable = VK_FALSE;
	input_assembly_state_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	//viewport state create info
	viewport_state_info.scissorCount = 1;
	viewport_state_info.viewportCount = 1;
	viewport_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	//rasterizer state create info
	rasterizer_state_info.lineWidth = 1.0f;
	rasterizer_state_info.depthBiasEnable = VK_FALSE;
	rasterizer_state_info.depthClampEnable = VK_FALSE;
	rasterizer_state_info.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer_state_info.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer_state_info.rasterizerDiscardEnable = VK_FALSE;
	rasterizer_state_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	//multisample state create info
	multisample_state_info.sampleShadingEnable = VK_FALSE;
	multisample_state_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisample_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	//color blend attachement info
	color_blend_attachment_state.blendEnable = VK_FALSE;
	color_blend_attachment_state.colorWriteMask = 
		VK_COLOR_COMPONENT_R_BIT | 
		VK_COLOR_COMPONENT_G_BIT | 
		VK_COLOR_COMPONENT_B_BIT | 
		VK_COLOR_COMPONENT_A_BIT;
	//color blend state info
	color_blend_state_info.attachmentCount = 1;
	color_blend_state_info.logicOpEnable = VK_FALSE;
	color_blend_state_info.blendConstants[0] = 0.0f;
	color_blend_state_info.blendConstants[1] = 0.0f;
	color_blend_state_info.blendConstants[2] = 0.0f;
	color_blend_state_info.blendConstants[3] = 0.0f;
	color_blend_state_info.logicOp = VK_LOGIC_OP_COPY;
	color_blend_state_info.pAttachments = &color_blend_attachment_state;
	color_blend_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	//dynamic states info
	dynamic_state_info.pDynamicStates = dynamic_states.data();
	dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
	//pipeline layout info
	pipeline_layout_info.setLayoutCount = 0;
	pipeline_layout_info.pushConstantRangeCount = 0;
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	//create pipeline
	if(vkCreatePipelineLayout(m_device, &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout!");
	}
	//cleanup
	vkDestroyShaderModule(m_device, shader_module_vert, nullptr);
	vkDestroyShaderModule(m_device, shader_module_frag, nullptr);
}

//physical device
bool Application::check_physical_device(VkPhysicalDevice device) 
{
	//data
	queue_family_indices indices = query_queue_families(device);
	swap_chain_support support = query_swap_chain_support(device);
	//return
	return 
		indices.m_family_graphics.has_value() && indices.m_family_present.has_value() &&
		check_extension_support(device) && !support.m_formats.empty() && !support.m_present_modes.empty();
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
queue_family_indices Application::query_queue_families(VkPhysicalDevice device)
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
	VkBool32 present_support = false;
	for(const VkQueueFamilyProperties& queue_family : queue_families)
	{
		if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.m_family_graphics = index;
		}
		vkGetPhysicalDeviceSurfaceSupportKHR(device, index, m_surface, &present_support);
		if(present_support)
		{
			indices.m_family_present = index;
		}
		index++;
	}
	//return
	return indices;
}

//shaders
std::vector<char> Application::read_file(const std::string& path)
{
	//open
	std::ifstream file(path, std::ios::ate | std::ios::binary);
	//check
	if(!file.is_open())
	{
		throw std::runtime_error("Failed to open file!");
	}
	//count
	size_t file_size = (size_t) file.tellg();
	//read
	file.seekg(0);
	std::vector<char> buffer(file_size);
	file.read(buffer.data(), file_size);
	//close
	file.close();
	//return
	return buffer;
}
VkShaderModule Application::create_shader_module(const std::vector<char>& code)
{
	//data
	VkShaderModuleCreateInfo shader_module_create_info{};
	//shader module info
	shader_module_create_info.codeSize = code.size();
	shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_module_create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());
	//create
	VkShaderModule shader_module;
	if(vkCreateShaderModule(m_device, &shader_module_create_info, nullptr, &shader_module) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create shader module!");
	}
	//return
	return shader_module;
}

//swap chain
swap_chain_support Application::query_swap_chain_support(VkPhysicalDevice device)
{
	//data
	swap_chain_support support;
	//count
	uint32_t format_count, present_mode_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &format_count, nullptr);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_mode_count, nullptr);
	//capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &support.m_capabilities);
	//formats
	if(format_count != 0)
	{
		support.m_formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &format_count, support.m_formats.data());
	}
	//presentation
	if(present_mode_count != 0)
	{
		support.m_present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_mode_count, support.m_present_modes.data());
	}
	//return
	return support;
}
VkExtent2D Application::choose_swap_chain_extent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(m_window, &width, &height);
		VkExtent2D actual_extent = {uint32_t(width), uint32_t(height)};
		actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actual_extent;
	}
}
VkPresentModeKHR Application::choose_swap_chain_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes)
{
	for(const auto& available_present_mode : available_present_modes)
	{
		if(available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return available_present_mode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}
VkSurfaceFormatKHR Application::choose_swap_chain_surface_format(const std::vector<VkSurfaceFormatKHR>& avaliable_formats)
{
	for(const VkSurfaceFormatKHR& available_format : avaliable_formats)
	{
		if(available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return available_format;
		}
	}
	return avaliable_formats[0];
}

//static data
const std::vector<const char*> Application::m_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};