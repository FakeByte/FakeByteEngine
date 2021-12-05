
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

void printDeviceStats(VkPhysicalDevice &device) {
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);

	std::cout << "Name: " << properties.deviceName << std::endl;
	uint32_t apiVersion = properties.apiVersion;
	std::cout << "API Version: " << VK_VERSION_MAJOR(apiVersion) << "." << VK_VERSION_MINOR(apiVersion) << "." << VK_VERSION_PATCH(apiVersion) << std::endl;
	std::cout << "Driver Version: " << properties.driverVersion << std::endl;
	std::cout << "Vendor ID: " << properties.vendorID << std::endl;
	std::cout << "Device ID: " << properties.deviceID << std::endl;
	std::cout << "Device Type: " << properties.deviceType << std::endl;

	std::cout << std::endl;
}

int main() {

	// Used to check results of different Vulkan methods
	VkResult result;

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


	result = vkCreateInstance(&instanceInfo, NULL, &instance);

	ASSERT_VULKAN(result);

	// Get amount of graphic cards
	uint32_t physicalDeviceCount = 0;
	result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL); // if pPhysicalDevices is NULL it returns the amount of physical devices
	ASSERT_VULKAN(result);

	// Get graphic cards
	VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
	result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);
	ASSERT_VULKAN(result);

	for (int i = 0; i < physicalDeviceCount; i++) {
		printDeviceStats(physicalDevices[i]);
	}

	return 0;
}

