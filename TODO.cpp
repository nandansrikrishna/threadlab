/* If you'd like, rename this file and update the Makefile */
#include <iostream>
#include <string>
#include "cpu.h"
#include "thread.h"
#include "mutex.h"
#include "cv.h"
#include <unistd.h>


struct args {
    mutex *m;
    cv *done;
	int *intp;
	int count;
    int *num_remaining;
};

void worker(void *vp) {
    struct args *ap = reinterpret_cast<struct args*>(vp);

    int *int_ptr = ap->intp;

    ap->m->lock();
    for (int i = 0; i < ap->count; ++i) {
        // ap->m->lock();
        (*int_ptr)++;
        // ap->m->unlock();
    }
    (*(ap->num_remaining))--;
    if ((*(ap->num_remaining)) == 0) {
        ap->done->signal();
    }
    ap->m->unlock();    

    std::cout << "Counter reached: " << *(ap->intp) << "\n";
}

void manager(void *vp) {
    int *int_ptr = new int(0);
    mutex *m = new(mutex);
    cv *done = new(cv);
    int *num_remaining = new int(10);

    struct args *ap = new(struct args);
    ap->m = m;
	ap->intp = int_ptr;
	ap->count = 100000;
    ap->done = done;
    ap->num_remaining = num_remaining;

    for (int i = 0; i < 10; ++i) {
        thread t(worker, ap);
    }

    m->lock();
    done->wait(*m);
    m->unlock();

    // usleep(10000);

    std::cout << *int_ptr << "\n";
}

int main(){
    cpu::boot(manager,nullptr,0);
    return 0;
}
