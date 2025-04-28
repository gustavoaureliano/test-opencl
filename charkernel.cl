__kernel 
void myKernel(__global char *myChar, __global int *B) {
	int i = get_global_id(0);
	if (myChar[i] == 'e') {
		atomic_add(B, 1);
	}
}
