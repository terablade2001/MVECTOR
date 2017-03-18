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

#ifndef __MVECTOR_HPP__
#define __MVECTOR_HPP__

#include <stdlib.h>
#include <cstring>
#include <memory>
#include <cstdio>
#include <limits>

#ifdef _MSC_VER
	#define snprintf _snprintf_s
	#define __MVECTOR_FOPEN__(fp, fname, mode) fopen_s(&fp, fname, mode)
	#define __ZU__ "%Iu"
	#include <algorithm>
#endif
#ifndef _MSC_VER
	#define __MVECTOR_FOPEN__(fp, fname, mode) fp = fopen(fname, mode)
	#define __ZU__ "%zu"
#endif

#define MVECTOR_VERSION (0.001)
#define MVECTOR_STEP_ELEMENTS (1024)
#define MVECTOR_STEP_ELEMENTS_BACK (10*1024)

using namespace std;

namespace MVECTOR {

template<class T> class MVECTOR;

template<class T>
class MVECTOR {
public:
	MVECTOR();
	MVECTOR(
		size_t elements_,
		size_t step_elements_ = ((size_t)-1)
	);
	MVECTOR(
		size_t elements_,
		T init_value_,
		size_t step_elements_ = ((size_t)-1)
	);
	~MVECTOR();
	
	void resize(size_t new_elements_);
	void resize(size_t new_elements_, T value_);
	int clear();
	size_t size();
	size_t bytes();
	T* data();
	T &operator[](size_t index_);
	void push_back(T value_);
	void pop_back(T value_);

private:
	T* pdata;
	size_t elements;
	size_t mem_elements;
	size_t step_elements;
}; // class MVECTOR



//@#| ############### Template - Constructors ###############
template <class T>
MVECTOR<T>::MVECTOR() :
	pdata(NULL),
	elements(0),
	mem_elements(0),
	step_elements(MVECTOR_STEP_ELEMENTS)
{

}

template <class T>
MVECTOR<T>::~MVECTOR() {
    clear();
}


template <class T> MVECTOR<T>::MVECTOR(
	size_t elements_,
	size_t step_elements_
) :
	pdata(NULL),
	elements(0),
	mem_elements(0),
	step_elements(MVECTOR_STEP_ELEMENTS)
{
	if (step_elements_ != ((size_t)-1))
		step_elements = step_elements_;
	if (elements_ == 0) return;
	size_t align_elements = ((elements_ / step_elements) + 1) * step_elements;
	pdata = (T*)malloc(align_elements * sizeof(T));
	elements = elements_;
	mem_elements = align_elements;
}


template <class T> MVECTOR<T>::MVECTOR(
	size_t elements_,
	T init_value_,
	size_t step_elements_
) :
	pdata(NULL),
	elements(0),
	mem_elements(0),
	step_elements(MVECTOR_STEP_ELEMENTS)
{
	if (step_elements_ != ((size_t)-1))
		step_elements = step_elements_;
	if (elements_ == 0) return;
	size_t align_elements = ((elements_ / step_elements) + 1) * step_elements;
	pdata = (T*)malloc(align_elements * sizeof(T));
	elements = elements_;
	mem_elements = align_elements;
	for (size_t i = 0; i < elements; i++)
		pdata[i] = init_value_;	
}

//@#: ############### .HPP file ###############
template <class T> void MVECTOR<T>::resize (size_t new_elements_) {
	if (new_elements_ == 0) { clear(); return; }
	bool do_copy = ((pdata != NULL) && (elements > 0));
	T* pdata_old = pdata;
	size_t align_elements = ((new_elements_ / step_elements) + 1) * step_elements;
	pdata = (T*)malloc(align_elements * sizeof(T));
	if (do_copy) memcpy(pdata, pdata_old, elements * sizeof(T));
	free(pdata_old);
	elements = new_elements_;
	mem_elements = align_elements;
}

template <class T> void MVECTOR<T>::resize (size_t new_elements_, T value_) {
	resize(new_elements_);
	for (size_t i = 0; i < new_elements_; i++)
		pdata[i] = value_;
}

template <class T> size_t MVECTOR<T>::bytes () {
	return size() * sizeof(T);
}

template <class T> T* MVECTOR<T>::data() {
	return pdata;
}

template <class T> T& MVECTOR<T>::operator[](size_t index_) {
	// if (pdata == NULL) resize(1); // Crash if pdata == NULL.
	// if (index_ >= elements) return pdata[0]; // Crash if wrong index is called.
	return pdata[index_];
}

template <class T> void MVECTOR<T>::push_back(T value_) {
	elements++;
	if (elements >= mem_elements)
		resize(elements);
	pdata[elements-1] = value_;
}

template <class T> void MVECTOR<T>::pop_back(T value_) {
	if (elements == 0) return;
	elements--;
	if (mem_elements - elements >= MVECTOR_STEP_ELEMENTS_BACK)
		resize(elements);
}
	

template <class T>
int MVECTOR<T>::clear() {
	if (pdata == NULL) return 1;
	free(pdata);
	pdata = NULL;
	elements = 0;
	mem_elements = 0;
	return 0;
}

template <class T>
size_t MVECTOR<T>::size() {
	return elements;
}


}; // namespace MVECTOR


#endif









