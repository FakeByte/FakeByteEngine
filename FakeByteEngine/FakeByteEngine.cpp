
#include "pch.h"
#include <iostream>
#include "vulkan/vulkan.h"

#ifdef NDEBUG

#define ASSERT_VULKAN(value)

#else

#define ASSERT_VULKAN(value)\
		if(value != VK_SUCCESS){\
			__debugbreak();\
		}

#endif // NDEBUG

VkInstance instance;

int main() {

	VkApplicationInfo applicationInfo;
	applicationInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext				= NULL;
	applicationInfo.pApplicationName	= "Test Application";
	applicationInfo.applicationVersion	= VK_MAKE_API_VERSION(0, 0, 1, 0);
	applicationInfo.pEngineName			= "FakeByteEngine";
	applicationInfo.engineVersion		= VK_MAKE_API_VERSION(0, 0, 1, 0);
	applicationInfo.apiVersion			= VK_API_VERSION_1_2;

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext						= NULL;
	instanceInfo.flags						= 0;
	instanceInfo.pApplicationInfo			= &applicationInfo;
	instanceInfo.enabledLayerCount			= 0;
	instanceInfo.ppEnabledLayerNames		= NULL;
	instanceInfo.enabledExtensionCount		= 0;
	instanceInfo.ppEnabledExtensionNames	= NULL;


	VkResult result = vkCreateInstance(&instanceInfo, NULL, &instance);

	ASSERT_VULKAN(result);

	return 0;
}

