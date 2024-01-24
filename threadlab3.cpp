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
	mutex      *mu;
	int        *intp;
	int         count;
	string      msg;
};
		   
  
//void incrementer(struct args *ap)
void incrementer(void *vp)
{
	struct args *ap = reinterpret_cast<struct args *>(vp); // New

	int *ip = ap->intp;
	
	ap->mu->lock();
	for (int i = 0; i < ap->count; i++) {
		(*ip)++;
	}
	ap->mu->unlock();

	printf("%s value is %d\n", ap->msg.c_str(), *(ap->intp));
}


void parent(void *vp) // vp is unused
{
	const int numInc = 10;
	const int counter = 1000000;

	int *ip = new(int);
	mutex *mu = new(mutex);

	for (int i = 0; i < 10; i++) {
		struct args *ap = new(struct args);
		ap->mu = mu;
		ap->intp = ip;
		ap->count = counter/numInc;
		ap->msg = "incrementer ";
		ap->msg += std::to_string(i);
		thread t(incrementer, ap);
	}

	mu->lock();
	printf("value is: %d\n", *ip);
	mu->unlock();
}


int main()
{
	cpu::boot(parent,nullptr,0);
}
