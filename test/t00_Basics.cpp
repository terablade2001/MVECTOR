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

int main(int argc, char **argv) {
	// Create F0 with 10-floats initialized with number 1.0.
	MVECTOR::MVECTOR<float> F0(10, 1);
	printf("F0 size: " __ZU__ ", bytes: " __ZU__ "\n", F0.size(), F0.bytes());
	for (size_t i = 0; i < 10; i++) printf("F0[i] = %f\n", F0[i]);

	// Create F1 of type char. Update steps (memory de/allocation steps) and 
	// test read/write operations.
	MVECTOR::MVECTOR<char> F1;
	F1.set_steps(10, 20);
	F1.resize(11);
	printf("F1 size: " __ZU__ ", bytes: " __ZU__ "\n", F1.size(), F1.bytes());
	for (size_t i = 0; i < F1.size(); i++) F1[i] = i;
	for (size_t i = 0; i < F1.size(); i++) printf("%i\n",(int)F1[i]);

	// Create F2 of type short. Update steps (memory de/allocation steps) and 
	// test read/write operations using external pointer to the data. Test 
	// also what happens if the MVECTOR object destroyed!
	short *pF2outer;
	{
		MVECTOR::MVECTOR<short> F2(2048);
		printf("F2 size: " __ZU__ ", bytes: " __ZU__ "\n", F2.size(), F2.bytes());
		for (size_t i = 0; i < 2048; i++) F2[i] = i;
		short* pF2 = F2.data();
		pF2outer = pF2;
		for (int i = 2040; i < 2048; i++) printf("%i\n", (int)pF2[i]);
		printf("NO-CRASH: %i\n", (int)pF2outer[0]);
	}
	printf("Mem-Address: " __ZU__ "\n", (size_t)pF2outer);
	printf("CRASH: %i\n", (int)pF2outer[0]);

	MVECTOR::MVECTOR<float> F3;
	F3.set_steps(5,5);
	for (int i = 0; i < 21; i++) F3.push_back(i);
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

	return 0;
}