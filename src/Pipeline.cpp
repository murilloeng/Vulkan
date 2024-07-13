//std
#include <fstream>
#include <stdexcept>

//vulkan
#include "vulkan/inc/Device.hpp"
#include "vulkan/inc/Pipeline.hpp"

//constructors
Pipeline::Pipeline(GLFWwindow* window, std::string path_vertex, std::string path_fragment) : 
	m_path_vertex(path_vertex), m_path_fragment(path_fragment)
{
	m_device = new Device(window);
	std::vector<char> buffer_vertex;
	std::vector<char> buffer_fragment;
	read_file(buffer_vertex, path_vertex);
	read_file(buffer_fragment, path_fragment);
	printf("code size (vertex): %llu\n", buffer_vertex.size());
	printf("code size (fragment): %llu\n", buffer_fragment.size());
}

//destructor
Pipeline::~Pipeline(void)
{
	return;
}

//read
void Pipeline::read_file(std::vector<char>& buffer, std::string path)
{
	//file
	std::ifstream file{path, std::ios::ate | std::ios::binary};
	//check
	if(!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + path);
	}
	//size
	size_t file_size = (size_t) file.tellg();
	//buffer
	file.seekg(0);
	buffer.resize(file_size);
	file.read(buffer.data(), file_size);
}