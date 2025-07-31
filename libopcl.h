#ifndef LIB_OPCL_200_H
#define LIB_OPCL_200_H

#define CL_TARGET_OPENCL_VERSION 200
#include <CL/cl.h>

cl_device_id get_device(int default_platform_id);
cl_device_id get_device_select();
cl_kernel compile_kernel(char* source_path, cl_device_id device, cl_context context);
char* get_cl_error_string(cl_int status);
void handle_unknown_error(cl_int status);


#endif // LIB_OPCL_200_H
