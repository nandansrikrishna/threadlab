/*
 * The first thing they should do is launch the incrementer in a
 * thread instead of calling it synchronously. Have them notice that
 * the "final" print is no longer final (and is also not correct.)
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


struct args {
	int        *intp;
	int         count;
	string      msg;
};
		   
  
// Incrementer now has to take a void * to type check
void incrementer(void *vp)
{
	// And we have to cast it back to the "correct" type
	struct args *ap = reinterpret_cast<struct args *>(vp); 

	int *ip = ap->intp;
	
	for (int i = 0; i < ap->count; i++) {
		(*ip)++;
	}

	printf("%s value is %d\n", ap->msg.c_str(), *(ap->intp));
}


void parent(void *vp) // vp is unused
{
	int *ip = new int(0);

	struct args *ap = new(struct args);
	ap->intp = ip;
	ap->count = 1000000;
	ap->msg = "incrementer";

	// Creates a thread rather than calling
	thread t(incrementer, ap);

	printf("value is: %d\n", *(ap->intp));
}


int main()
{
	cpu::boot(parent,nullptr,0);
}
