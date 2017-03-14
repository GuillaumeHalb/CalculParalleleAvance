#include "util.hpp"
#include <err_code.h>
#include "device_picker.hpp"
#include <CL/cl.h>

int main(int argc, char *argv[])
{
	//OpenCl variables
	cl_platform_id platform_id = NULL;		
	cl_device_id device_id = NULL;		
	cl_context context = NULL;		
	cl_command_queue command_queue = NULL;


	double start_time;      // Starting time
        double run_time;        // Timing
    	util::Timer timer;      // Timing

	float* Option;
	float* Model;
	float* Data;	

	int Depth = 7; //default value
	int Root_i, Root_j;
	
	Option = (float*)malloc(5*sizeof(float));
	Model = (float*)malloc(7*sizeof(float));
	Data = (float*)malloc(depth*sizeof(float));

	
