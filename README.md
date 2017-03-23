# MVECTOR

## Information
I created the MVECTOR as a utility to allocate/deallocate memory, in order 
to replace the std::vector library. std::vector library seems not to be 
reliable as far as it concerns the actual memory allocation/deallocation in 
all devices. I.e, calling the .clear() method, it may or may not(!) 
de-allocate the allocated memory of the container of std::vector (*"A 
reallocation is not guaranteed to happen, and the vector capacity is not 
guaranteed to change due to calling this function."). This behaviour seems 
to be unacceptable for some needs (i.e. embedded systems).

MVECTOR is using malloc/free or new/delete (with shared_ptr) depending the 
needs of the client (change behaviour by disable/enable the #MVECTOR_USE__NEW 
directive). For simple types of data it can also track the amount of total 
allocated memory, or the amount of allocated memory in user-defined groups.

### Example 1: Allocate Easy Memory and track the allocated size of all MVECTORs.
```cpp
using namespace ns_MVECTOR;

MVECTOR<float> vF; // vector<float> vF;
vF.resize(20);

// Access elements like an array:
for(size_t i=0; i < vF.size(); i++) vF[i] = i;
// Or Access the elements using a pointer to the data of the MVECTOR.
float* pointer_vF = vF.data();

// Create a second vector:
MVECTOR<char> vC; // vector<char> vC;
vC.resize(100, 8); // Initialize 100 elements of type 'char' with value 8.

// Check Total Memory allocated with MVECTOR arrays.
printf("vF allocated memory: " __ZU__ " Bytes\n", vF.bytes());
printf("vC allocated memory: " __ZU__ " Bytes\n", vC.bytes());
printf("Total MVECTOR allocated memory: " __ZU__ " Bytes\n",
	vF.total_bytes() // or vC.total_bytes()
);
```
### Example 2: Allocate in groups
For this test - using also pthreads - please also review t01_Threads.cpp 
test-file.
```cpp
MVECTOR<float> vF; vF.set_group(1);
MVECTOR<char> vC; vC.set_group(1);
MVECTOR<double> vD; vD.set_group(2);
vF.resize(10);
vC.resize(2048);
vD.resize(4096);

// Total Allocated memory for vF+vC+vD
printf("Total Allocated memory: " __ZU__ " Bytes\n", vF.total_bytes());

// Allocated memory for MVECTOR group = 1
printf("Allocated memory for Group = 1: " __ZU__ " Bytes\n",
	vF.total_bytes(1)
);

// Allocated memory for MVECTOR group = 2
printf("Allocated memory for Group = 2: " __ZU__ " Bytes\n",
	vF.total_bytes(2)
);
```

## Compile And Test using CMAKE

### Windows10 using "make-3.81" and "cmake-3.8" and "TDM-GCC-64 5.1.0".
Run from the main repository folder the following scripts in command prompt.
```makefile
mkdir build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make

# Test with:
MVECTOR t00_Basics
# or
MVECTOR t01_Threads
```

### Windows10 using ie. MSVC 12.
Run from the main repository folder the following scripts in command prompt.
```makefile
mkdir build && cd build
cmake -G "Visual Studio 12 2013" ..

# Open the .sln file via the MSVC-IDE and compile.
# Test with:
MVECTOR t00_Basics
```


## Notes

This library is under development. It may not work correct for more 
complicates issues rather than allocating / deallocating memory and objects.
