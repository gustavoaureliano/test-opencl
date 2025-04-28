#include <stdio.h>
#include <time.h>

#define CL_TARGET_OPENCL_VERSION 200
#include <CL/cl.h>

#define MAX_SOURCE_SIZE 2048000000

int main(int argc, char* argv[]) {
	// Setup OpenCL

	cl_int status; // for error checking
	
	// Get number of platforms
	cl_uint numPlatforms = 0;
	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	printf("numPlatforms: %d\n", numPlatforms);

	// Allocate space for platforms
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));

	// Fill int the platforms
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);

	char* selectedValue = NULL;

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
	int total = 16;
	size_t dataSize = sizeof(int)*total;
	int* A = malloc(dataSize);
	for (int i = 0; i < total; i++) {
		A[i] = i;
	}

	// Print Data Input
	printf("Input: ");
	for (int i = 0; i < total; i++) {
		printf("%d ", A[i]);
	}
	printf("\n");


	char* filepath = "longtext.txt";
	FILE* fileData = fopen(filepath, "r");
	char* fileSource = malloc(MAX_SOURCE_SIZE);
	size_t fileSize = fread(fileSource, sizeof(char), MAX_SOURCE_SIZE, fileData);
	// printf("filesource:\n%s\n", fileSource);
	printf("file size: (%zu) - end of source!\n", fileSize);
	
	// Allocate buffers
	cl_mem bufMyChar = clCreateBuffer(
			context,
			CL_MEM_READ_ONLY,
			fileSize,
			NULL, &status
			);
	cl_mem bufB = clCreateBuffer(
			context, 
			CL_MEM_WRITE_ONLY,
			sizeof(int),
			NULL, &status
			);

	// Copy data to device right now (CL_TRUE)
	status = clEnqueueWriteBuffer(
			cmdQueue, bufMyChar, CL_TRUE, 0, fileSize,
			fileSource, 0, NULL, NULL
			);
	//
	// Read kernel source code
	FILE* file = fopen("charkernel.cl", "r");
	if (!file) {
		printf("error: no such kernel\n");
		return 0;
	}
	char* kernelSouce = malloc(MAX_SOURCE_SIZE);
	size_t sourceSize = fread(kernelSouce, sizeof(char), MAX_SOURCE_SIZE, file);
	fclose(file);

	// printf("%s", kernelSouce); // print source code
	// printf("size: (%zu) - end of source!\n", sourceSize);
	
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
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufMyChar);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bufB);

	// Define an index space of work-itens for execution
	// A work-group size is not required, but can be used
	size_t indexSpaceSize[3] = {fileSize, 1, 1};
	// size_t indexSpaceSize[1] = {total};
	// size_t workGroupSize[1] = {1};
	size_t workGroupSize[3] = {64, 1, 1};
	
	cl_uint workDimension = 1;

	clock_t t;
	t = clock();

	// Execute the kernel
	status = clEnqueueNDRangeKernel(
			cmdQueue, kernel, 1, NULL,
			indexSpaceSize, NULL,
			0, NULL, NULL
			);

	// Copy data from device
	int B = 0; // Allocate memory to copy data
	status = clEnqueueReadBuffer(
			cmdQueue, bufB, CL_TRUE,
			0, sizeof(int), &B,
			0, NULL, NULL
			);

	t = clock() - t;
	double timeTakenGPU = ((double)t)/(CLOCKS_PER_SEC);

	// Print Data Output
	printf("gpu timetaken: %f miliseconds\n", timeTakenGPU*1000);
	printf("Output: ");
	printf("%d ", B);
	printf("\n");
	
	// Block host execution until everything is done
	clFlush(cmdQueue);
	clFinish(cmdQueue);


    t = clock(); 
	uint64_t count = 0;
	for (int i = 0; i < fileSize; i++) {
		if (fileSource[i] == 'e') {
			count++;
		}
	}
	t = clock() - t;
	double timeTakenCPU = ((double)t)/CLOCKS_PER_SEC;
	printf("cpu timetaken: %f miliseconds\n", timeTakenCPU*1000);
	printf("output: %lu\n", count);

	printf("comparing: cpuTime/gpuTime= %f", timeTakenCPU/timeTakenGPU);

	// Free memory
	
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cmdQueue);
	clReleaseMemObject(bufMyChar);
	clReleaseMemObject(bufB);
	clReleaseContext(context);

	free(platforms);
	free(devices);
	free(A);
	free(kernelSouce);

	return 0;
}
