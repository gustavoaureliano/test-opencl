#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define CL_TARGET_OPENCL_VERSION 200
#include <CL/cl.h>

#define MAX_SOURCE_SIZE 2048000000
#define MAX_FILE_SIZE 4294967295

#define KERNELPATH "../charkernel.cl"

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


	char* filepath = "../longtext.txt";
	FILE* datafp = fopen(filepath, "r");
	if (!datafp) {
		fprintf(stderr, "Failed to read file '%s': %s\n" , filepath, strerror(errno));
		return 1;
	}
	fseek(datafp, 0L, SEEK_END);
	long datafilesize = ftell(datafp);
	fseek(datafp, 0L, SEEK_SET);
	printf("datafilesize: %ld\n", datafilesize);
	if (datafilesize > MAX_FILE_SIZE) {
		printf("WARNING! file size is way too big! \nThe GPU might not be able to handle values bigger than 32 bits can hold.\n");
	}

	char* fileSource = (char*) malloc(datafilesize);
	size_t filebytesread = fread(fileSource, sizeof(char), datafilesize, datafp);
	printf("filesbytesread: %ld\n", filebytesread);
	if (datafilesize != filebytesread) {
		fprintf(stderr, "Error reading file '%s': %s\n" , filepath, strerror(errno));
		return 1;
	}
	fclose(datafp);
	// printf("filesource:\n%s\n", fileSource);
	printf("file size: (%zu) - end of source!\n", filebytesread);
	
	// Allocate buffers
	cl_mem bufMyChar = clCreateBuffer(
			context,
			// Use CL_MEM_USE_HOST_PTR to work around GPU VRAM limitations
			CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
			// CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, // This option actually copies the data to the GPU VRAM (Should be way faster)
			filebytesread,
			fileSource, &status
			);
	cl_mem bufB = clCreateBuffer(
			context, 
			CL_MEM_WRITE_ONLY,
			sizeof(int),
			NULL, &status
			);

	// Copy data to device right now (CL_TRUE)
	// Not needed with CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR
	// status = clEnqueueWriteBuffer(
	// 		cmdQueue, bufMyChar, CL_TRUE, 0, filebytesread,
	// 		fileSource, 0, NULL, NULL
	// 		);

	// Read kernel source code
	FILE* kernelfp = fopen(KERNELPATH, "r");
	if (!kernelfp) {
          fprintf(stderr, "Failed to read kernel '%s': %s\n", KERNELPATH,
                  strerror(errno));
          return 1;
        }
	fseek(kernelfp, 0L, SEEK_END);
	long kernelsize = ftell(kernelfp);
	fseek(kernelfp, 0L, SEEK_SET);

	char* kernelSouce = (char*) malloc(kernelsize);
	size_t bytesread = fread(kernelSouce, sizeof(char), kernelsize, kernelfp);
	if (kernelsize != bytesread) {
		fprintf(stderr, "Error reading the kernel '%s': %s\n" , KERNELPATH, strerror(errno));
		return 1;
	}
	fclose(kernelfp);

	// printf("%s", kernelSouce); // print source code
	// printf("size: (%zu) - end of source!\n", sourceSize);
	
	// Create a program with source code
	cl_program program = clCreateProgramWithSource(
			context, 1,
			(const char**)&kernelSouce,
			NULL, &status
			);

	cl_uint addr_bits = 0;
	clGetDeviceInfo(devices[0],
			CL_DEVICE_ADDRESS_BITS,
			sizeof(addr_bits),
			&addr_bits,
			NULL);

	// Get build log
	// size_t log_size;
	// clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
	// char *log = (char *) malloc(log_size);
	// clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
	// printf("Build log:\n%s\n", log);
	// free(log);

	// Create the kernel
    cl_kernel kernel = clCreateKernel(program, "myKernel", &status);

	// Set kernel arguments
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufMyChar);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bufB);

	// Define an index space of work-itens for execution
	// A work-group size is not required, but can be used
	size_t indexSpaceSize[3] = {filebytesread, 1, 1};
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
	int count = 0;
	for (size_t i = 0; i < filebytesread; i++) {
		if (fileSource[i] == 'e') {
			count++;
		}
	}
	t = clock() - t;
	double timeTakenCPU = ((double)t)/CLOCKS_PER_SEC;
	printf("cpu timetaken: %f miliseconds\n", timeTakenCPU*1000);
	printf("output: %d\n", count);

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
