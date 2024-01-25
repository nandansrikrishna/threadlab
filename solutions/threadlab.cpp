/*
 * The final version includes a CV, plus a state variable that counts
 * the remaining live incrementers. Each incrementer reduces that
 * count by one and signals when it is done. The parent thread waits
 * unitl all of them have finished to print the total value, which is
 * now guaranteed to be correct.
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
	mutex      *mu;
	cv         *finished;
	int        *intp;
	int        *remainp;
	int         count;
	string      msg;
};
		   
  
//void incrementer(struct args *ap)
void incrementer(void *vp)
{
	struct args *ap = reinterpret_cast<struct args *>(vp); // New

	int *ip = ap->intp;
	
	for (int i = 0; i < ap->count; i++) {
		ap->mu->lock();
		(*ip)++;
		ap->mu->unlock();
	}

	printf("%s value is %d\n", ap->msg.c_str(), *(ap->intp));

	ap->mu->lock();
	*(ap->remainp) -= 1;
	ap->finished->signal();
	ap->mu->unlock();
}


void parent(void *vp) // vp is unused
{
	const int numInc = 10;
	const int counter = 1000000;

	mutex *mu = new(mutex);
	cv    *finished = new(cv);
	int   *ip = new(int);
	int   *remainp = new int(numInc);
	

	for (int i = 0; i < 10; i++) {
		struct args *ap = new(struct args);
		ap->mu = mu;
		ap->finished = finished;
		ap->intp = ip;
		ap->remainp = remainp;
		ap->count = counter/numInc;
		ap->msg = "incrementer ";
		ap->msg += std::to_string(i);
		thread t(incrementer, ap);
	}

	mu->lock();
	while (*remainp) {
		finished->wait(*mu);
	}
	printf("value is: %d\n", *ip);
	mu->unlock();
}


int main()
{
	cpu::boot(parent,nullptr,0);
}
