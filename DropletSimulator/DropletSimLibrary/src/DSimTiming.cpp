#include "DSimTiming.h"

#ifndef _WIN32
#include <sys/time.h>
#endif

//=================== Windows functions =========================
#ifdef _WIN32
void DSimTiming::initTimer(double sss) {

	init_time = (double)(timeGetTime())/1000.0;
	current_time = init_time;
	sim_step_size = sss;
	elapsed_ST = 0.0;
}

void DSimTiming::updateTimer(double sss) {
	
	sim_step_size = sss;
	last_step = current_time;
	current_time = (double)(timeGetTime())/1000.0;
	elapsed_ST += sim_step_size;
}

void DSimTiming::updateTimer() {

	last_step = current_time;
	current_time = (double)(timeGetTime())/1000.0;
	elapsed_ST += sim_step_size;
}

void DSimTiming::resetTimer() {
	init_time = (double)(timeGetTime())/1000.0;
	current_time = init_time;
	elapsed_ST = 0.0;
}

//=================== Unix functions =========================
#else

double tvToDouble(struct timeval time) {

	return time.tv_sec + (double)(time.tv_usec)/1000000.0;
}
void DSimTiming::initTimer(double sss) {

	gettimeofday(&time, NULL);
	init_time = tvToDouble(time);
	current_time = init_time;
	sim_step_size = sss;
	elapsed_ST = 0.0;
}

void DSimTiming::updateTimer(double sss) {
	
	sim_step_size = sss;
	last_step = current_time;
	gettimeofday(&time, NULL);
	current_time = tvToDouble(time);
	elapsed_ST += sim_step_size;
}

void DSimTiming::updateTimer() {

	last_step = current_time;
	gettimeofday(&time, NULL);
	current_time = tvToDouble(time);
	elapsed_ST += sim_step_size;
}

void DSimTiming::resetTimer() {
	gettimeofday(&time, NULL);
	init_time = tvToDouble(time);
	current_time = init_time;
	elapsed_ST = 0.0;
}
#endif
// =============== get functions ====================
double DSimTiming::getTotalRT() {

	return current_time - init_time;
}

double DSimTiming::getTotalST() {

	return elapsed_ST;
}

double DSimTiming::getStepRT() {

	return current_time - last_step;
}

double DSimTiming::getTotalDiff() {

	return getTotalRT() - getTotalST();
}

double DSimTiming::getTimeRatio() {

	return sim_step_size/getStepRT();
}

// ================= Print Functions ==========================
void DSimTiming::printTotalRT() {

	printf("Total Real Time Elapsed: %.3f\n", getTotalRT() );
}

void DSimTiming::printTotalST() {
	
	printf("Total Simulator Time Elapsed: %.3f\n", getTotalST() );
}

void DSimTiming::printStepRT() {
	
	printf("Time Elapsed Since the Last Step: %.3f\n", getStepRT() );
}

void DSimTiming::printTotalDiff() {
	
	printf("Difference Between Elapsed RT and ST: %.3f\n", getTotalDiff() );
	printf("A negative value indicates RT < ST\n");
}

void DSimTiming::printTimeRatio() {
	
	printf("Ratio Between ST and RT: %.3f\n", getTimeRatio() );
}

void DSimTiming::printAll() {
	
	printf("Total RT: %.3f, Total ST: %.3f, Step time: %.3f, Total Diff: %.3f, Ratio: %.3f\n", 
		getTotalRT(), getTotalST(), getStepRT(), getTotalDiff(), getTimeRatio() );
}

void DSimTiming::printVars() {

	printf("init_time: %f, current_time: %f, last_step: %f\n", 
		init_time, current_time, last_step);
	printf("sim_step_size: %f, elapsed_ST: %f\n", sim_step_size, elapsed_ST);
}