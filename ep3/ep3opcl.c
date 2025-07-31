#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../libopcl.h"

#define KERNELPATH "ep3kernel.cl"

int main(int argc, char* argv[]) {
	cl_int status;

	cl_device_id device = get_device_select();

	// Create a context for the selected device
	cl_uint num_devices = 1;
	cl_context context = clCreateContext(
			NULL, num_devices, &device,
			NULL, NULL, &status
			);
	handle_unknown_error(status);


	const int num_pixels = 10;
	uint8_t pixels[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	cl_mem bufPixels = clCreateBuffer(
			context,
			CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, // This option actually copies the data to the GPU VRAM
			num_pixels,
			pixels, &status
			);
	handle_unknown_error(status);
	cl_mem bufB = clCreateBuffer(
			context,
			CL_MEM_WRITE_ONLY,
			sizeof(int),
			NULL, &status
			);
	handle_unknown_error(status);

	cl_kernel kernel = compile_kernel(KERNELPATH, device, context);
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufPixels);
	handle_unknown_error(status);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bufB);
	handle_unknown_error(status);

	// Create a command-queue for a given device
	cl_command_queue cmdQueue = clCreateCommandQueueWithProperties(
			context,
			device,
			0, &status
			);

	// Define an index space of work-itens for execution
	// A work-group size is not required, but can be used
	size_t indexSpaceSize[3] = {num_pixels, 1, 1};
	size_t workGroupSize[3] = {10, 1, 1};
	cl_uint workDimension = 1;

	clock_t t;
	t = clock();

	// Execute the kernel
	status = clEnqueueNDRangeKernel(
			cmdQueue, kernel, workDimension, NULL,
			indexSpaceSize, workGroupSize,
			0, NULL, NULL
			);
	handle_unknown_error(status);

	// Block host execution until everything is done
	clFlush(cmdQueue);
	clFinish(cmdQueue);

	t = clock() - t;
	double timeTakenGPU = ((double)t)/(CLOCKS_PER_SEC);

	// Copy data from device
	int B = 0; // Allocate memory to copy data
	status = clEnqueueReadBuffer(
			cmdQueue, bufB, CL_TRUE,
			0, sizeof(int), &B,
			0, NULL, NULL
			);
	handle_unknown_error(status);

	// Print Data Output
	printf("gpu timetaken: %f miliseconds\n", timeTakenGPU*1000);
	printf("Output: ");
	printf("%d ", B);
	printf("\n");
}

