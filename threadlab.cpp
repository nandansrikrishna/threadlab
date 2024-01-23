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
		   
  
//void incrementer(struct args *ap)
void incrementer(void *vp)
{
	struct args *ap = reinterpret_cast<struct args *>(vp); // New

	int *ip = ap->intp;
	
	for (int i = 0; i < ap->count; i++) {
		(*ip)++;
	}

	printf("%s value is %d\n", ap->msg.c_str(), *(ap->intp));
}


void parent(void *vp) // vp is unused
{
	int *ip = new(int);

	struct args *ap = new(struct args);
	ap->intp = ip;
	ap->count = 1000000;
	ap->msg = "incrementer";

	//incrementer(ap);
	thread t(incrementer, ap);

	printf("value is: %d\n", *(ap->intp));
}


int main()
{
	cpu::boot(parent,nullptr,0);
}
