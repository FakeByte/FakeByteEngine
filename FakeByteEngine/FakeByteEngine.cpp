
#include "pch.h"
#include <iostream>
#include <vector>
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
VkDevice device;

void printDeviceStats(VkPhysicalDevice &device) {
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);

	uint32_t apiVersion = properties.apiVersion;

	std::cout << "Name:                    " << properties.deviceName << std::endl;
	std::cout << "API Version:             " << VK_VERSION_MAJOR(apiVersion) << "." << VK_VERSION_MINOR(apiVersion) << "." << VK_VERSION_PATCH(apiVersion) << std::endl;
	std::cout << "Driver Version:          " << properties.driverVersion << std::endl;
	std::cout << "Vendor ID:               " << properties.vendorID << std::endl;
	std::cout << "Device ID:               " << properties.deviceID << std::endl;
	std::cout << "Device Type:             " << properties.deviceType << std::endl;
	std::cout << "discreteQueuePriorities: " << properties.limits.discreteQueuePriorities << std::endl;

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);
	std::cout << "Geometry Shader: " << features.geometryShader << std::endl;

	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

	VkQueueFamilyProperties *familyProperties = new VkQueueFamilyProperties[queueFamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, familyProperties);

	std::cout << "Queue Family Count: " << queueFamilyCount << std::endl;

	for (int i = 0; i < queueFamilyCount; i++) {
		std::cout << std::endl;
		std::cout << "Queue Family #" << i << std::endl;
		std::cout << "VK_QUEUE_GRAPHICS_BIT:       " << ((familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << std::endl;
		std::cout << "VK_QUEUE_COMPUTE_BIT:        " << ((familyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) << std::endl;
		std::cout << "VK_QUEUE_TRANSFER_BIT:       " << ((familyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << std::endl;
		std::cout << "VK_QUEUE_SPARSE_BINDING_BIT: " << ((familyProperties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) << std::endl;
		std::cout << "Queue Count:                 " << familyProperties[i].queueCount << std::endl;
		std::cout << "Timestamp Valid Bits:        " << familyProperties[i].timestampValidBits << std::endl;


		uint32_t width  = familyProperties[i].minImageTransferGranularity.width;
		uint32_t height = familyProperties[i].minImageTransferGranularity.height;
		uint32_t depth  = familyProperties[i].minImageTransferGranularity.depth;
		std::cout << "Min Image Timestamp Granularity: " << width << ", " << height << ", " << depth << std::endl;
	}

	std::cout << std::endl;

	delete[] familyProperties;
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

	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, NULL);
	VkLayerProperties *layers = new VkLayerProperties[layerCount];
	vkEnumerateInstanceLayerProperties(&layerCount, layers);

	std::cout << "Layer Count: " << layerCount << std::endl;
	for (int i = 0; i < layerCount; i++) {
		std::cout << std::endl;
		std::cout << "Name:                   " << layers[i].layerName << std::endl;
		std::cout << "Spec Version:           " << layers[i].specVersion << std::endl;
		std::cout << "Implementation Version: " << layers[i].implementationVersion << std::endl;
		std::cout << "Descritption:           " << layers[i].description << std::endl;
	}
	std::cout << std::endl;

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	VkExtensionProperties *extensions = new VkExtensionProperties[extensionCount];
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);

	std::cout << "Extension Count: " << layerCount << std::endl;
	for (int i = 0; i < extensionCount; i++) {
		std::cout << std::endl;
		std::cout << "Name:         " << extensions[i].extensionName << std::endl;
		std::cout << "Spec Version: " << extensions[i].specVersion << std::endl;
	}
	std::cout << std::endl;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	VkInstanceCreateInfo instanceCreateInfo;
	instanceCreateInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext					= NULL;
	instanceCreateInfo.flags					= 0;
	instanceCreateInfo.pApplicationInfo			= &applicationInfo;
	instanceCreateInfo.enabledLayerCount		= validationLayers.size();
	instanceCreateInfo.ppEnabledLayerNames		= validationLayers.data();
	instanceCreateInfo.enabledExtensionCount	= 0;
	instanceCreateInfo.ppEnabledExtensionNames	= NULL;


	result = vkCreateInstance(&instanceCreateInfo, NULL, &instance);

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

	float queuePriorities[] = {1.0, 1.0, 1.0, 1.0};

	VkDeviceQueueCreateInfo deviceQueueCreateInfo;
	deviceQueueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.pNext            = NULL;
	deviceQueueCreateInfo.flags            = 0;
	deviceQueueCreateInfo.queueFamilyIndex = 0;
	deviceQueueCreateInfo.queueCount       = 4;
	deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

	VkPhysicalDeviceFeatures usedFeatures = {};

	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext                   = NULL;
	deviceCreateInfo.flags                   = 0;
	deviceCreateInfo.queueCreateInfoCount    = 1;
	deviceCreateInfo.pQueueCreateInfos       = &deviceQueueCreateInfo;
	deviceCreateInfo.enabledLayerCount       = 0;
	deviceCreateInfo.ppEnabledLayerNames     = NULL;
	deviceCreateInfo.enabledExtensionCount   = 0;
	deviceCreateInfo.ppEnabledExtensionNames = NULL;
	deviceCreateInfo.pEnabledFeatures        = &usedFeatures;

	result = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, NULL, &device);
	ASSERT_VULKAN(result);

	return 0;
}

