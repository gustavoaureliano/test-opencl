#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
#define CL_TARGET_OPENCL_VERSION 200

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

 
#define MAX_SOURCE_SIZE (0x10000000)
 
int main(void) {
    // Create the two input vectors
    int i;
    const int LIST_SIZE = 102400000;
    int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
    int *B = (int*)malloc(sizeof(int)*LIST_SIZE);
    for(i = 0; i < LIST_SIZE; i++) {
        A[i] = i;
        B[i] = LIST_SIZE - 1;
    }
 
    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;
 
    fp = fopen("vector_add_kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
 
	// Setup OpenCL

	cl_int ret; // for error checking
	
	// Get number of platforms
	cl_uint numPlatforms = 0;
	ret = clGetPlatformIDs(0, NULL, &numPlatforms);
	printf("Number of Platforms: %d\n", numPlatforms);

	// Allocate space for platforms
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));

	// Fill int the platforms
	ret = clGetPlatformIDs(numPlatforms, platforms, NULL);

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
	ret = clGetDeviceIDs(
			platforms[selectedPlatform],
			CL_DEVICE_TYPE_ALL, 0, NULL,
			&numDevices
			);
	printf("numDevices: %d\n", numDevices);

	// Allocate space for devices
	cl_device_id *devices;
	devices = (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));

	// Fill in the devices
	ret = clGetDeviceIDs(
			platforms[selectedPlatform],
			CL_DEVICE_TYPE_ALL,
			numDevices, devices,
			NULL
			);

	// Create a context that includes all devices
	cl_context context = clCreateContext(
			NULL, numDevices, devices,
			NULL, NULL, &ret
			);
 
	int selectedDevice = 0;
    // Create a command queue
		cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, devices[selectedDevice], 0, &ret);
 
    // Create memory buffers on the device for each vector 
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            LIST_SIZE * sizeof(int), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
            LIST_SIZE * sizeof(int), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            1 * sizeof(int), NULL, &ret);
 
    // Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
            LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
            LIST_SIZE * sizeof(int), B, 0, NULL, NULL);
 
    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, 
            (const char **)&source_str, (const size_t *)&source_size, &ret);
 
    // Build the program
    ret = clBuildProgram(program, 1, &devices[selectedDevice], NULL, NULL, NULL);
 
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
 
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

	clock_t t; 
    t = clock(); 
 
    // Execute the OpenCL kernel on the list
    size_t global_item_size = LIST_SIZE; // Process the entire lists
    size_t local_item_size = 256; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
 
    // Read the memory buffer C on the device to the local variable C
    int *C = (int*)malloc(sizeof(int)*1);
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, 
            1 * sizeof(int), C, 0, NULL, NULL);
 
    // Clean up

	t = clock() - t;
	double timeTaken = ((double)t)/CLOCKS_PER_SEC;

    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

	printf("gpu timetaken: %f miliseconds\n", timeTaken*1000);
	printf("%d\n", C[0]);

    t = clock(); 
	int c = 0;
	for (int i = 0; i < LIST_SIZE; i++) {
		c += A[i] + B[i];
	}
	t = clock() - t;
	timeTaken = ((double)t)/CLOCKS_PER_SEC;
	printf("cpu timetaken: %f miliseconds\n", timeTaken*1000);
	printf("%d\n", c);

    // for(i = 0; i < LIST_SIZE; i++)
    //     printf("%d + %d = %d\n", A[i], B[i], A[i] + B[i]);

    free(A);
    free(B);
    free(C);
    return 0;
}

