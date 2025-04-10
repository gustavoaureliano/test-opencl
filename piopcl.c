#include <stdio.h>

#define CL_TARGET_OPENCL_VERSION 200
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x10000000)

int main(int argc, char* argv[]) {
	//printf("argc: %d\n", argc);
	if (argc <= 1) {
		printf("1 argument nedeed\n");
		return 0;
	}
	//double eps = atof(argv[1]);
	//printf("eps: %.16f\n", eps);
	float eps = atof(argv[1]);
	printf("eps: %.16f\n", eps);
	
	// Setup OpenCL

	cl_int status; // for error checking
	
	// Get number of platforms
	cl_uint numPlatforms = 0;
	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	printf("Number of Platforms: %d\n", numPlatforms);

	// Allocate space for platforms
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));

	// Fill int the platforms
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);

	char* selectedValue = NULL;

	printf("Available platforms:\n");
	// Print platform names
	for (int i = 0; i < numPlatforms; i++) {
		size_t param_value_size = sizeof(char)*60; // Arbitrary value for the size
		size_t param_value_size_ret; // Actual value will be here
		void* param_value = malloc(param_value_size); // Allocate memory to store the name
		clGetPlatformInfo(
				platforms[i],
				CL_PLATFORM_NAME,
				param_value_size,
				(char*)param_value,
				&param_value_size_ret
				);
		//printf("actual size of value: %zu\n", param_value_size_ret);
		if (param_value_size_ret <= 0) {
			printf("error: no name returned");
			break;
		}
		char* platformName = (char*)param_value;
		printf("[%d]: %s\n", i, platformName);
		free(param_value);
	}
	int selectedPlatform = 0;
	if (numPlatforms > 1) {
		do {
			printf("Select the desired platform\n: ");
			scanf("%d", &selectedPlatform);
			printf("Selected Platform: %d\n", selectedPlatform);
			if (selectedPlatform < numPlatforms) {
				break;
			}
			printf("Selected platform (%d) is invalid!\n", selectedPlatform);
		} while (selectedPlatform >= numPlatforms);
	}

	// Get number of devices for given platform
	cl_uint numDevices = 0;
	status = clGetDeviceIDs(
			platforms[selectedPlatform],
			CL_DEVICE_TYPE_ALL, 0, NULL,
			&numDevices
			);
	printf("numDevices: %d\n", numDevices);

	// Allocate space for devices
	cl_device_id *devices;
	devices = (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));

	// Fill in the devices
	status = clGetDeviceIDs(
			platforms[selectedPlatform],
			CL_DEVICE_TYPE_ALL,
			numDevices, devices,
			NULL
			);

	// Create a context that includes all devices
	cl_context context = clCreateContext(
			NULL, numDevices, devices,
			NULL, NULL, &status
			);

	int selectedDevice = 0;
	// Create a command-queue for a given device
	cl_command_queue cmdQueue = clCreateCommandQueueWithProperties(
			context, 
			devices[selectedDevice],
			0, &status
			);

	// Create Data
	float pi = 0;
	int div = 1;
	int num = 1;
	unsigned long total = 1000000;
	unsigned long addSum = total*2;
	int divs[total];
	int nums[total];

	for (int i = 0; i < total; i++) {
		divs[i] = div;
		nums[i] = num;
		div += 2;
		num *= -1;
	}
	/*
	// Print Data Input
	printf("Input: \nnums: ");
	for (int i = 0; i < total; i++) {
		printf("%d ", nums[i]);
	}
	printf("\ndivs: ");
	for (int i = 0; i < total; i++) {
		printf("%d ", divs[i]);
	}
	printf("\n");
	*/
	
	// Allocate buffers
	cl_mem bufAddSum = clCreateBuffer(
			context,
			CL_MEM_READ_ONLY,
			sizeof(addSum),
			NULL, &status
			);
	cl_mem bufNums = clCreateBuffer(
			context, 
			CL_MEM_READ_ONLY,
			sizeof(num)*total,
			NULL, &status
			);
	cl_mem bufDivs = clCreateBuffer(
			context, 
			CL_MEM_READ_ONLY,
			sizeof(div)*total,
			NULL, &status
			);
	cl_mem bufEps = clCreateBuffer(
			context, 
			CL_MEM_READ_ONLY,
			sizeof(eps),
			NULL, &status
			);
	cl_mem bufPi = clCreateBuffer(
			context, 
			CL_MEM_WRITE_ONLY,
			sizeof(pi),
			NULL, &status
			);

	// Copy data to device right now (CL_TRUE)
	status = clEnqueueWriteBuffer(
			cmdQueue, bufAddSum, CL_TRUE, 0, sizeof(addSum),
			&addSum, 0, NULL, NULL
			);
	status = clEnqueueWriteBuffer(
			cmdQueue, bufNums, CL_TRUE, 0, sizeof(num)*total,
			nums, 0, NULL, NULL
			);
	status = clEnqueueWriteBuffer(
			cmdQueue, bufDivs, CL_TRUE, 0, sizeof(div)*total,
			divs, 0, NULL, NULL
			);
	status = clEnqueueWriteBuffer(
			cmdQueue, bufEps, CL_TRUE, 0, sizeof(eps),
			&eps, 0, NULL, NULL
			);

	// Read kernel source code
	FILE* file = fopen("pikernel.cl", "r");
	if (!file) {
		printf("error: no such kernel\n");
		return 0;
	}
	char* kernelSouce = malloc(MAX_SOURCE_SIZE);
	size_t sourceSize = fread(kernelSouce, sizeof(char), MAX_SOURCE_SIZE, file);
	fclose(file);

	// printf("%s", kernelSouce); // print source code
	
	// Create a program with source code
	cl_program program = clCreateProgramWithSource(
			context, 1,
			(const char**)&kernelSouce,
			NULL, &status
			);

	// Build (compile) the program for the devices
	status = clBuildProgram(
			program, 1, &devices[0],
			NULL, NULL, NULL
			);

	// Create the kernel
    cl_kernel kernel = clCreateKernel(program, "myKernel", &status);

	// Set kernel arguments
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufAddSum);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bufNums);
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&bufDivs);
	status = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&bufEps);
	status = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*)&bufPi);

	// Define an index space of work-itens for execution
	// A work-group size is not required, but can be used
	//size_t indexSpaceSize[3] = {total, 1, 1};
	size_t indexSpaceSize[1] = {total};
	size_t workGroupSize[1] = {1};
	//size_t workGroupSize[3] = {64, 1, 1};
	
	cl_uint workDimension = 1;
	// Execute the kernel
	status = clEnqueueNDRangeKernel(
			cmdQueue, kernel, workDimension, NULL,
			indexSpaceSize, workGroupSize,
			0, NULL, NULL
			);

	// Copy data from device
	//int pi[dataSize]; // Allocate memory to copy data
	status = clEnqueueReadBuffer(
			cmdQueue, bufPi, CL_TRUE,
			0, sizeof(pi), &pi,
			0, NULL, NULL
			);

	// Print Data Output
	printf("Output: ");
	printf("%.16f\n", pi*4);

	/*
	for (int i = 0; i < total; i++) {
		printf("%d ", pi[i]);
	}
	printf("\n");
	*/
	
	// Block host execution until everything is done
	clFlush(cmdQueue);
	clFinish(cmdQueue);

	// Free memory
	
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cmdQueue);
	clReleaseMemObject(bufAddSum);
	clReleaseMemObject(bufNums);
	clReleaseMemObject(bufDivs);
	clReleaseMemObject(bufEps);
	clReleaseMemObject(bufPi);
	clReleaseContext(context);

	free(platforms);
	free(devices);
	free(kernelSouce);

	return 0;
}
