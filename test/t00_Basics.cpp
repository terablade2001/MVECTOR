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

using namespace ns_MVECTOR;

int main(int argc, char **argv) {
	// Create F0 with 10-floats initialized with number 1.0.
	printf("---------------------------- F0 ---------------------------------\n");
	MVECTOR<float> F0(10, 1);
	printf("F0 size: " __ZU__ ", bytes: " __ZU__ "\n", F0.size(), F0.bytes());
	for (size_t i = 0; i < 10; i++) printf("F0[i] = %f\n", F0[i]);

	// Create F1 of type char. Update steps (memory de/allocation steps) and 
	// test read/write operations.
	printf("---------------------------- F1 ---------------------------------\n");
	MVECTOR<char> F1;
	F1.set_steps(10, 20);
	F1.resize(11);
	printf("F1 size: " __ZU__ ", bytes: " __ZU__ "\n", F1.size(), F1.bytes());
	for (size_t i = 0; i < F1.size(); i++) F1[i] = i;
	for (size_t i = 0; i < F1.size(); i++) printf("%i\n",(int)F1[i]);

	// Create F2 of type short. Update steps (memory de/allocation steps) and 
	// test read/write operations using external pointer to the data. Test 
	// also what happens if the MVECTOR object destroyed!
	printf("---------------------------- F2 ---------------------------------\n");
	short *pF2outer;
	{
		MVECTOR<short> F2(2048);
		printf("F2 size: " __ZU__ ", bytes: " __ZU__ "\n", F2.size(), F2.bytes());
		for (size_t i = 0; i < 2048; i++) F2[i] = i;
		short* pF2 = F2.data();
		pF2outer = pF2;
		for (int i = 2040; i < 2048; i++) printf("%i\n", (int)pF2[i]);
		printf("NO-CRASH: %i\n", (int)pF2outer[0]);
	}
	printf("Mem-Address: " __ZU__ "\n", (size_t)pF2outer);
	printf("CRASH: %i\n", (int)pF2outer[0]);

	// Create F3 of type float, and test push_back and pop_back functions
	printf("---------------------------- F3 ---------------------------------\n");
	MVECTOR<float> F3;
	F3.set_steps(5,5);
	for (int i = 0; i < 21; i++)
		F3.push_back(i);
	printf("F3 size(): " __ZU__ ", Bytes: " __ZU__ "\n",
		F3.size(), F3.bytes()
	);
	for (int i = 0; i < 21; i++)
		printf("F3[%i] = %f\n",i,F3[(size_t)i]);

	for (int i = 0; i < 12; i++) F3.pop_back();
	printf("F3 size(): " __ZU__ ", Bytes: " __ZU__ "\n",
		F3.size(), F3.bytes()
	);
	for (int i = 0; i < (int)F3.size(); i++)
		printf("F3[%i] = %f\n",i,F3[(size_t)i]);	

	// Create F4 of type vector<float> and handle two vectors (#MVECTOR_USE__NEW)
	printf("---------------------------- F4 ---------------------------------\n");
#ifndef MVECTOR_USE__NEW
	printf("F4 requires that the #MVECTOR_USE__NEW to be defined! - Aborting!\n");
#else
	{
		MVECTOR<vector<float>> F4;
		for (size_t i = 0; i < 5; i++) {
			vector<float> v;
			F4.push_back(v);
		}
		printf("F4 size(): " __ZU__ ", Bytes: " __ZU__ "\n",
			F4.size(), F4.bytes()
		);
		for (size_t i = 0; i < 5; i++) {
			vector<float> *v = F4.data();
			for (int j = 0; j < 10; j++)
				v[i].push_back((float)(j + 10*i));
		}
		vector<float> *v = F4.data();
		for (size_t i = 0; i < 5; i++) {
			printf("v[" __ZU__ "] = F4[" __ZU__ "] = [",i,i);
			for (size_t j = 0; j < v[i].size(); j++) printf("%f ",v[i][j]);
			printf("]\n");
		}
	}
#endif

	// Create F5 of type vector<float> and fill up some memory! Check deallocate!
	printf("---------------------------- F5 ---------------------------------\n");
#ifndef MVECTOR_USE__NEW
	printf("F5 requires that the #MVECTOR_USE__NEW to be defined! - Aborting!\n");
#else
	{
		MVECTOR<vector<char>> F5;
		vector<char> v; v.resize(1,1);
		printf("Check your system's memory. Press any key to fill!..\n");
		getch();
		F5.resize(50000000, v);
		printf("Check your system's memory. Did you see allocation?..\n");
		printf("Press any key to destroy F5 object, while checking the memory.\n");
		getch();
	}
	printf("Check your system's memory;F5 destroyed. Did memory de-allocated?\n");
	getch();
#endif

	printf("\n\n ----------------- TEST END --------------------\n");
	return 0;
}