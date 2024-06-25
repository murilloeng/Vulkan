//defines
#define GLFW_INCLUDE_VULKAN

//glfw
#include <GLFW/glfw3.h>

int main(void)
{
	//setup
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
	//main loop
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	//window
	glfwDestroyWindow(window);
	//finish
	glfwTerminate();
	//return
	return 0;
}