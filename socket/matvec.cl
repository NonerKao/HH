__kernel void matvec_mult(__global float4* matrix, __global float4* vector, __global float* result){
	int i = get_global_id(0);
	if(i == 0) printf("start\n");
	result[i] = dot(matrix[i], vector[0]);
	if(i == 0)printf("end\n");
}
