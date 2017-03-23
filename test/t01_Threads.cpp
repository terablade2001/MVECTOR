// MIT License

// Copyright (c) 2017 Vasileios Kon. Pothos (terablade2001)
// https://github.com/terablade2001/MVECTOR

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Include the MVECTOR library
#include "../src/include/MVECTOR.hpp"
#include <vector>
#include <conio.h>
#include <atomic>
#ifndef _MSC_VER
#include <unistd.h>
#include <pthread.h>
#endif

#define MAX_THREADS (100)

using namespace ns_MVECTOR;
#ifndef _MSC_VER
#ifdef MVECTOR_MAX_GROUPS
static int WaitFlag = 1;
static pthread_mutex_t mutex_lock;
static std::atomic<size_t> thread_count(0);

void *thread_function(void* tid) {
	size_t thread_id = (size_t)tid;
	

	MVECTOR<char> F;

	// Select specific group where bytes will count (Default: 0).
	if (thread_id >= 32)
		F.set_group((thread_id < MVECTOR_MAX_GROUPS) ? thread_id : 1);

	// Allocate memory.
	F.resize(1024);

	pthread_mutex_lock(&mutex_lock);
		printf("Thread: " __ZU__ ", Total Allocated Memory: " __ZU__ "\n",
			thread_id, F.total_bytes()
		);
	pthread_mutex_unlock(&mutex_lock);
	
	thread_count++;
	if (thread_count >= MAX_THREADS) {
		printf("\n-------- ALL Threads has allocated F(1024) --------\n");
		printf("Total Allocated Memory: " __ZU__ "\n", F.total_bytes());
		printf("Allocated Memory per group:\n");
		size_t sum_of_allocated_group_memory = 0;
		for(size_t j = 0; j < MVECTOR_MAX_GROUPS; j++) {
			sum_of_allocated_group_memory += F.total_bytes(j);
			printf("Group[" __ZU__ "] -> " __ZU__ " Bytes.\n",
				j, F.total_bytes(j)
			);			
		}
		printf("Sum of allocated memory for all groups: " __ZU__ "\n",
			sum_of_allocated_group_memory
		);
		//getch();
		WaitFlag = 0;
	}
		
	while (WaitFlag == 1) usleep(10000);

	pthread_exit(NULL);
    return NULL;
}
#endif
#endif

int t01_Threads() {
#ifndef MVECTOR_MAX_GROUPS
	printf("This test requires ns_MVECTOR::#MVECTOR_MAX_GROUPS to be defined!\n");
	return -1;
#else
#ifdef _MSC_VER
	printf("_MSC_VER detected: Can't use pThreads.\n");
#else
	int rc;
	pthread_mutex_init(&mutex_lock, NULL);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_t threads[MAX_THREADS];
	WaitFlag=1;
	for (size_t tid = 0; tid < MAX_THREADS; tid++) {
		rc = pthread_create(
				&threads[tid],
				&attr,
				thread_function,
				(void *)tid
		);
		if (rc) { printf("Threads-Error...\n"); break; }
	}
	void* status;
	for (int tid = 0; tid < MAX_THREADS; tid++) {
		rc = pthread_join(threads[tid], &status);
		if (rc) { printf("Threads-Error...\n"); break; }
	}

	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex_lock);
#endif
#endif
	return 0;
}