/*
 * This is the version to start with. It uses the thread library but
 * has only one thread.
 */

#include <iostream>
#include <string>
#include "cpu.h"
#include "thread.h"
#include "mutex.h"
#include "cv.h"

using std::cout;
using std::endl;
using std::string;


// The argument block to the incrementer
// I am not using globals, but you could if you think it is easier

struct args {
	int        *intp; // Must be a pointer
	int         count;
	string      msg;
};
		   
  
// Increments the pointed-to-integer count times.
void incrementer(struct args *ap)
{
	int *ip = ap->intp;
	
	for (int i = 0; i < ap->count; i++) {
		(*ip)++;
	}

	printf("%s value is %d\n", ap->msg.c_str(), *(ap->intp));
}


// The "main" thread body; argument is unused.
void parent(void *vp) 
{
	int *ip = new int(0);

	struct args *ap = new(struct args);
	ap->intp = ip;
	ap->count = 1000000;
	ap->msg = "incrementer";

	incrementer(ap);

	printf("value is: %d\n", *(ap->intp));
}


int main()
{
	cpu::boot(parent,nullptr,0);
}
