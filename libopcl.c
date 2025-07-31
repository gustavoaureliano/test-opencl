#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "libopcl.h"

cl_kernel compile_kernel(char* source_path, cl_device_id device, cl_context context) {
	cl_int status;

	FILE* kernelfp = fopen(source_path, "r");
	if (!kernelfp) {
          fprintf(stderr, "Failed to read kernel '%s': %s\n", source_path,
                  strerror(errno));
          exit(1);
        }
	fseek(kernelfp, 0L, SEEK_END);
	long kernelsize = ftell(kernelfp);
	fseek(kernelfp, 0L, SEEK_SET);

	char* kernelSouce = (char*) malloc(kernelsize);
	size_t bytesread = fread(kernelSouce, sizeof(char), kernelsize, kernelfp);
	if (kernelsize != bytesread) {
		fprintf(stderr, "Error reading the kernel '%s': %s\n" , source_path, strerror(errno));
		exit(1);
	}
	fclose(kernelfp);

	// printf("%s", kernelSouce); // print source code
	// printf("size: (%zu) - end of source!\n", kernelsize);

	// Create a program with source code
	cl_uint number_of_sources = 1;
	cl_program program = clCreateProgramWithSource(
			context, number_of_sources,
			(const char**)&kernelSouce,
			NULL, &status
			);
	handle_unknown_error(status);
	free(kernelSouce);

	// Build (compile) the program for the devices
	cl_uint number_of_devices = 1;
	status = clBuildProgram(
			program, number_of_devices, &device,
			NULL, NULL, NULL
			);
	if (status == CL_INVALID_PROGRAM) {
		fprintf(stderr, "Invalid Program!\n");
		exit(1);
	} else if (status == CL_BUILD_PROGRAM_FAILURE) {
		fprintf(stderr, "Failed to build Program!\n");
		// print build log
		size_t log_size;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char *log = (char *) malloc(log_size);
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("Build log:\n%s\n", log);
		free(log);
		exit(1);
	}
	handle_unknown_error(status);

	// Create the kernel
	const char* kernelName = "myKernel";
    cl_kernel kernel = clCreateKernel(program, kernelName, &status);
	if (status == CL_INVALID_KERNEL_NAME) {
		fprintf(stderr, "The kernel name \"%s\" war not found in the program", kernelName);
		exit(1);
	}
	handle_unknown_error(status);
	return kernel;
}

void handle_unknown_error(cl_int status) {
	if (status != CL_SUCCESS) {
		fprintf(stderr, "Something went wrong!\n");
		fprintf(stderr, "OpenCL error code: %d (%s)\n", status, get_cl_error_string(status));
		exit(1);
	}
}

cl_device_id get_device_select() {
	return get_device(-1);
}

cl_device_id get_device(int default_platform_id) {
	cl_int status; // for error checking

	// Get number of platforms
	cl_uint numPlatforms = 0;
	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	printf("numPlatforms: %d\n", numPlatforms);
	handle_unknown_error(status);

	// Allocate space for platforms
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));

	// Fill in the platforms
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);
	handle_unknown_error(status);

	// Print platform names
	const unsigned int platform_name_max_size = 60;
	size_t param_value_size = sizeof(char)*platform_name_max_size; // Arbitrary value for the size
	size_t param_value_size_ret; // Actual value will be here
	char platformName[param_value_size]; // Allocate memory to store the name
	for (int i = 0; i < numPlatforms; i++) {
		param_value_size_ret = 0;
		// erase platformName
		memset(platformName, 0, param_value_size);
		handle_unknown_error(
				clGetPlatformInfo(
					platforms[i],
					CL_PLATFORM_NAME,
					param_value_size,
					platformName,
					&param_value_size_ret
					)
		);
		//printf("actual size of value: %zu\n", param_value_size_ret);
		if (param_value_size_ret <= 0) {
			fprintf(stderr, "error: no name returned for platform[%d]'\n", i);
			break;
		}
		printf("[%d]: %s\n", i, platformName);
	}

	// Select OpenCL platform
	int selectedPlatform = 0;
	if (default_platform_id >= 0) {
		selectedPlatform = default_platform_id;
		printf("Selected Platform: %d\n", selectedPlatform);
	} else {
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
	}

	// Get number of devices for given platform
	cl_uint numDevices = 0;
	status = clGetDeviceIDs(
			platforms[selectedPlatform],
			CL_DEVICE_TYPE_ALL, 0, NULL,
			&numDevices
			);
	printf("numDevices: %d\n", numDevices);
	if (numDevices <= 0) {
		fprintf(stderr, "No devices available on selected platform!\n");
		exit(1);
	}
	handle_unknown_error(status);

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
	if (status == CL_DEVICE_NOT_FOUND) {
		fprintf(stderr, "No devices available on selected platform!\n");
		exit(1);
	}
	handle_unknown_error(status);

	cl_device_id selected_device = devices[0];
	free(platforms);
	free(devices);
	return selected_device;
}

char* get_cl_error_string(cl_int status) {
	switch (status) {
		case CL_SUCCESS : return "CL_SUCESS";
		case CL_DEVICE_NOT_FOUND : return "CL_DEVICE_NOT_FOUND";
		case CL_DEVICE_NOT_AVAILABLE : return "CL_DEVICE_NOT_AVAILABLE";
		case CL_COMPILER_NOT_AVAILABLE : return "CL_COMPILER_NOT_AVAILABLE";
		case CL_MEM_OBJECT_ALLOCATION_FAILURE : return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
		case CL_OUT_OF_RESOURCES : return "CL_OUT_OF_RESOURCES";
		case CL_OUT_OF_HOST_MEMORY : return "CL_OUT_OF_HOST_MEMORY";
		case CL_PROFILING_INFO_NOT_AVAILABLE : return "CL_PROFILING_INFO_NOT_AVAILABLE";
		case CL_MEM_COPY_OVERLAP : return "CL_MEM_COPY_OVERLAP";
		case CL_IMAGE_FORMAT_MISMATCH : return "CL_IMAGE_FORMAT_MISMATCH";
		case CL_IMAGE_FORMAT_NOT_SUPPORTED : return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
		case CL_BUILD_PROGRAM_FAILURE : return "CL_BUILD_PROGRAM_FAILURE";
		case CL_MAP_FAILURE : return "CL_MAP_FAILURE";
		// CL_VERSION_1_1
		case CL_MISALIGNED_SUB_BUFFER_OFFSET : return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
		case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST : return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
		// CL_VERSION_1_2
		case CL_COMPILE_PROGRAM_FAILURE : return "CL_COMPILE_PROGRAM_FAILURE";
		case CL_LINKER_NOT_AVAILABLE : return "CL_LINKER_NOT_AVAILABLE";
		case CL_LINK_PROGRAM_FAILURE : return "CL_LINK_PROGRAM_FAILURE";
		case CL_DEVICE_PARTITION_FAILED : return "CL_DEVICE_PARTITION_FAILED";
		case CL_KERNEL_ARG_INFO_NOT_AVAILABLE : return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
		case CL_INVALID_VALUE : return "CL_INVALID_VALUE";
		case CL_INVALID_DEVICE_TYPE : return "CL_INVALID_DEVICE_TYPE";
		case CL_INVALID_PLATFORM : return "CL_INVALID_PLATFORM";
		case CL_INVALID_DEVICE : return "CL_INVALID_DEVICE";
		case CL_INVALID_CONTEXT : return "CL_INVALID_CONTEXT";
		case CL_INVALID_QUEUE_PROPERTIES : return "CL_INVALID_QUEUE_PROPERTIES";
		case CL_INVALID_COMMAND_QUEUE : return "CL_INVALID_COMMAND_QUEUE";
		case CL_INVALID_HOST_PTR : return "CL_INVALID_HOST_PTR";
		case CL_INVALID_MEM_OBJECT : return "CL_INVALID_MEM_OBJECT";
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR : return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
		case CL_INVALID_IMAGE_SIZE : return "CL_INVALID_IMAGE_SIZE";
		case CL_INVALID_SAMPLER : return "CL_INVALID_SAMPLER";
		case CL_INVALID_BINARY : return "CL_INVALID_BINARY";
		case CL_INVALID_BUILD_OPTIONS : return "CL_INVALID_BUILD_OPTIONS";
		case CL_INVALID_PROGRAM : return "CL_INVALID_PROGRAM";
		case CL_INVALID_PROGRAM_EXECUTABLE : return "CL_INVALID_PROGRAM_EXECUTABLE";
		case CL_INVALID_KERNEL_NAME : return "CL_INVALID_KERNEL_NAME";
		case CL_INVALID_KERNEL_DEFINITION : return "CL_INVALID_KERNEL_DEFINITION";
		case CL_INVALID_KERNEL : return "CL_INVALID_KERNEL";
		case CL_INVALID_ARG_INDEX : return "CL_INVALID_ARG_INDEX";
		case CL_INVALID_ARG_VALUE : return "CL_INVALID_ARG_VALUE";
		case CL_INVALID_ARG_SIZE : return "CL_INVALID_ARG_SIZE";
		case CL_INVALID_KERNEL_ARGS : return "CL_INVALID_KERNEL_ARGS";
		case CL_INVALID_WORK_DIMENSION : return "CL_INVALID_WORK_DIMENSION";
		case CL_INVALID_WORK_GROUP_SIZE : return "CL_INVALID_WORK_GROUP_SIZE";
		case CL_INVALID_WORK_ITEM_SIZE : return "CL_INVALID_WORK_ITEM_SIZE";
		case CL_INVALID_GLOBAL_OFFSET : return "CL_INVALID_GLOBAL_OFFSET";
		case CL_INVALID_EVENT_WAIT_LIST : return "CL_INVALID_EVENT_WAIT_LIST";
		case CL_INVALID_EVENT : return "CL_INVALID_EVENT";
		case CL_INVALID_OPERATION : return "CL_INVALID_OPERATION";
		case CL_INVALID_GL_OBJECT : return "CL_INVALID_GL_OBJECT";
		case CL_INVALID_BUFFER_SIZE : return "CL_INVALID_BUFFER_SIZE";
		case CL_INVALID_MIP_LEVEL : return "CL_INVALID_MIP_LEVEL";
		case CL_INVALID_GLOBAL_WORK_SIZE : return "CL_INVALID_GLOBAL_WORK_SIZE";
		// CL_VERSION_1_1
		case CL_INVALID_PROPERTY : return "CL_INVALID_PROPERTY";
		//CL_VERSION_1_2
		case CL_INVALID_IMAGE_DESCRIPTOR : return "CL_INVALID_IMAGE_DESCRIPTOR";
		case CL_INVALID_COMPILER_OPTIONS : return "CL_INVALID_COMPILER_OPTIONS";
		case CL_INVALID_LINKER_OPTIONS : return "CL_INVALID_LINKER_OPTIONS";
		case CL_INVALID_DEVICE_PARTITION_COUNT : return "CL_INVALID_DEVICE_PARTITION_COUNT";
		// CL_VERSION_2_0
		case CL_INVALID_PIPE_SIZE : return "CL_INVALID_PIPE_SIZE";
		case CL_INVALID_DEVICE_QUEUE : return "CL_INVALID_DEVICE_QUEUE";
		default: return "unknown_error";
	}
}
