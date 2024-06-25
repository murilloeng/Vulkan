//defines
#define GLFW_INCLUDE_VULKAN

//std
#include <cstdlib>

//glfw
#include <GLFW/glfw3.h>

//data
const unsigned width = 800;
const unsigned height = 600;

class HelloTriangleApplication
{
public:
	void run(void);

private:
	void loop(void);
	void setup(void);
	void cleanup(void);

	//data
	GLFWwindow* m_window;
	VkInstance m_instance;
};

void HelloTriangleApplication::run(void)
{
	setup();
	loop();
	cleanup();
}

void HelloTriangleApplication::loop(void)
{
	while(!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
	}
}
void HelloTriangleApplication::setup(void)
{
	//glfw
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//window
	m_window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
	//instance
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
}
void HelloTriangleApplication::cleanup(void)
{
	//glfw
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

int main(void)
{
	//data
	HelloTriangleApplication app;
	//run
	app.run();
	//return
	return EXIT_SUCCESS;
}