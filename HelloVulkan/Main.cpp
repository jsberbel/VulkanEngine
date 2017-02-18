#include "vulkan/vulkan.h"
#include "glfw/glfw3.h"

#ifndef _WIN64
	#pragma comment(lib, "glfw/x32/glfw3.lib")
	#pragma comment(lib, "vulkan/x32/vulkan-1.lib")
#else
	#pragma comment(lib, "glfw/x64/glfw3.lib")
	#pragma comment(lib, "vulkan/x64/vulkan-1.lib")
#endif

#include <Windows.h>
#include <cassert>
#include <iostream>
#include <vector>

const int WIDTH			{ 800 };
const int HEIGHT		{ 600 };
GLFWwindow* s_window	{ nullptr }; // Reference to the window object

const std::vector<const char*> validationLayers
{
	"VK_LAYER_LUNARG_standard_validation" // Enables a whole range of useful diagnostics layers
};

inline void checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		assert(layerFound);
	}
}

std::vector<const char*> getRequiredExtensions(){
	std::vector<const char*> extensions;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	#ifdef _DEBUG
	extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	#endif

	return extensions;
}

int __stdcall WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd)
{
	// INIT WINDOW
	{
		glfwInit(); // Initializes the GLFW library

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	// Tell GLFW not to create an OpenGL context
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		// Disable window resizability

		assert((s_window = glfwCreateWindow(WIDTH, HEIGHT,	// Window dimensions
											"Hello Vulkan", // Window title
											nullptr,		// Monitor to open a window
											nullptr)));		// OpenGL only
	}

	// INIT VULKAN
	{
		checkValidationLayerSupport();

		VkApplicationInfo appInfo{}; // Structure specifying application info
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;			// Type of the structure
		appInfo.pApplicationName = "Hello Triangle";				// The name of the application
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);		// The developer-supplied version number of the application
		appInfo.pEngineName = "No Engine";							// The name of the engine used to create the application
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);			// The developer-supplied version number of the engine
		appInfo.apiVersion = VK_API_VERSION_1_0;					// The version of the Vulkan API against which the application expects to run

		VkInstanceCreateInfo createInfo{}; // Structure specifying parameters of a newly created instance
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;	// Type of the structure
		createInfo.pApplicationInfo = &appInfo;						// The pointer to an instance of VkApplicationInfo			
		
		std::vector<const char*> extensions{ getRequiredExtensions() };
		createInfo.enabledExtensionCount = extensions.size(); // The names of extensions to enable
		createInfo.ppEnabledLayerNames = extensions.data(); // The names of extensions to enable

		#ifdef _DEBUG
		createInfo.enabledLayerCount = validationLayers.size(); // The number of global layers to enable
		createInfo.ppEnabledLayerNames = validationLayers.data(); // The names of extensions to enable
		#else
		createInfo.enabledLayerCount = 0;
		#endif

		VkInstance instance; // Opaque handle to a instance object
		assert(vkCreateInstance(&createInfo,						// Pointer to struct with creation info
								nullptr,							// Pointer to custom allocator callbacks
								&instance) == VK_SUCCESS);			// Pointer to the handle of the instance
	}

	while (!glfwWindowShouldClose(s_window)) // While window is not closed by the user
		glfwPollEvents(); // Check for events

	glfwDestroyWindow(s_window);

	return EXIT_SUCCESS;
}
