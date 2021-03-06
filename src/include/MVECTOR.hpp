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
#include <atomic>

#define MVECTOR_USE__NEW

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

#define MVECTOR_VERSION (0.010)

#define MVECTOR_MAX_GROUPS (64)
#define MVECTOR_STEP_ELEMENTS (1024)
#define MVECTOR_STEP_ELEMENTS_BACK (10*1024)

#ifndef __FNAME__
	#define __FNAMEBSL__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
	#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FNAMEBSL__ )
#endif

using namespace std;

namespace ns_MVECTOR {

template<class T> class MVECTOR;
template<class T > class s_mvector_deleter;

#ifdef MVECTOR_USE__NEW
template<class T >
class s_mvector_deleter {
public: 
  void operator()(T* p) { 
    delete[] p; 
  }
};
#endif

class MVECTOR_Base {
public:
	static std::atomic<size_t> Total_MVECTOR_Bytes;
#ifdef MVECTOR_MAX_GROUPS
	MVECTOR_Base();
	static std::atomic<size_t> Group_MVECTOR_Bytes[MVECTOR_MAX_GROUPS];
	int GroupID;
#endif
};


template<class T>
class MVECTOR : public MVECTOR_Base {
public:
	MVECTOR();
	MVECTOR(size_t elements_);
	MVECTOR(size_t elements_, T init_value_);
	MVECTOR(const MVECTOR& mvector_);
	~MVECTOR();
	
	void initialize(size_t elements_);
	void set_steps(
		size_t step_elements_,
		size_t step_elements_back_ = ((size_t)-1)
	);
	void resize(size_t new_elements_);
	void resize(size_t new_elements_, T value_);
	void cresize(size_t new_elements_);
	void cresize(size_t new_elements_, T value_);

	int clear();
	size_t size();
	size_t bytes();
	size_t total_bytes();
	T* data();
	T &operator[](size_t index_);
	MVECTOR<T>& operator=(MVECTOR<T>& mvector_);
	void push_back(T value_);
	void pop_back();
#ifdef MVECTOR_MAX_GROUPS
	void set_group(int group_id_);
	size_t total_bytes(int group_id_);
#endif
	void get_steps(size_t& step_elements_, size_t& step_elements_back_);
	T& front();
	T& back();

	int dbg;
private:
	T* pdata;
	size_t elements;
	size_t mem_elements;
	size_t step_elements;
	size_t step_elements_back;
#ifdef MVECTOR_USE__NEW
	std::shared_ptr<T> sptr;
#endif
}; // class MVECTOR



//@#| ############### Template - Constructors ###############
template <class T> MVECTOR<T>::MVECTOR() :
	dbg(0),
	pdata(NULL),
	elements(0),
	mem_elements(0),
	step_elements(MVECTOR_STEP_ELEMENTS),
	step_elements_back(MVECTOR_STEP_ELEMENTS_BACK)
{
}


template <class T> MVECTOR<T>::MVECTOR(const MVECTOR& mvector_):
	dbg(mvector_.dbg),
	pdata(mvector_.pdata),
	elements(mvector_.elements),
	mem_elements(0),
	step_elements(mvector_.step_elements),
	step_elements_back(mvector_.step_elements_back)

 {
	size_t els = elements;
	T* hdata = pdata;
	elements = 0;
	clear();
	initialize(els);
	for (size_t i = 0; i < els; i++)
		pdata[i] = hdata[i];
}

template <class T> MVECTOR<T>::~MVECTOR() {
	clear();
}

template <class T> MVECTOR<T>::MVECTOR(
	size_t elements_
) :
	dbg(0),
	pdata(NULL),
	elements(0),
	mem_elements(0),
	step_elements(MVECTOR_STEP_ELEMENTS),
	step_elements_back(MVECTOR_STEP_ELEMENTS_BACK)
{
	initialize(elements_);
}


template <class T> MVECTOR<T>::MVECTOR(
	size_t elements_,
	T init_value_
) :
	dbg(0),
	pdata(NULL),
	elements(0),
	mem_elements(0),
	step_elements(MVECTOR_STEP_ELEMENTS),
	step_elements_back(MVECTOR_STEP_ELEMENTS_BACK)
{
	initialize(elements_);
	for (size_t i = 0; i < elements; i++)
		pdata[i] = init_value_;	
}


template <class T>
void MVECTOR<T>::initialize(
	size_t elements_
) {
	if (elements_ == 0) return;
	size_t align_elements = ((elements_ / step_elements) + 1) * step_elements;
#ifndef MVECTOR_USE__NEW
	pdata = (T*)malloc(align_elements * sizeof(T));
#endif
#ifdef MVECTOR_USE__NEW
	sptr = std::shared_ptr<T>(new T[align_elements], s_mvector_deleter<T>());
	pdata = sptr.get();
#endif
	elements = elements_;
	mem_elements = align_elements;
	size_t current_bytes = bytes();
	Total_MVECTOR_Bytes += current_bytes;
#ifdef MVECTOR_MAX_GROUPS
	Group_MVECTOR_Bytes[GroupID] += current_bytes;
#endif
}

template <class T>
void MVECTOR<T>::set_steps(
	size_t step_elements_,
	size_t step_elements_back_
) {
	step_elements = step_elements_;
	if (step_elements_back_ != ((size_t)-1))
		step_elements_back = step_elements_back_;
}
	


//@#: ############### .HPP file ###############
template <class T> void MVECTOR<T>::resize (size_t new_elements_) {
	if (new_elements_ == 0) { clear(); return; }
	bool do_copy = ((pdata != NULL) && (elements > 0));
	size_t to_copy_elements =
		(elements <= new_elements_)? elements : new_elements_;
	T* pdata_old = NULL;
	size_t align_elements = ((new_elements_ / step_elements) + 1) * step_elements;
	size_t previous_bytes = bytes();
#ifndef MVECTOR_USE__NEW
	if (do_copy) pdata_old = pdata;
	pdata = (T*)malloc(align_elements * sizeof(T));
	if (do_copy) {
		memcpy(pdata, pdata_old, to_copy_elements * sizeof(T));
		free(pdata_old);
	}
#endif
#ifdef MVECTOR_USE__NEW
	std::shared_ptr<T> sptr_old;
	if (do_copy) {
		 sptr_old = sptr;
		 pdata_old = sptr_old.get();
	}
	sptr = std::shared_ptr<T>(new T[align_elements], s_mvector_deleter<T>());
	pdata = sptr.get();
	if (do_copy)
		for (size_t i = 0; i < to_copy_elements; i++) pdata[i] = pdata_old[i];
#endif
	elements = new_elements_;
	mem_elements = align_elements;
	size_t current_bytes = bytes();
	Total_MVECTOR_Bytes -= previous_bytes;
	Total_MVECTOR_Bytes += current_bytes;
#ifdef MVECTOR_MAX_GROUPS
	Group_MVECTOR_Bytes[GroupID] -= previous_bytes;
	Group_MVECTOR_Bytes[GroupID] += current_bytes;
#endif
}

template <class T> void MVECTOR<T>::resize (size_t new_elements_, T value_) {
	resize(new_elements_);
	for (size_t i = 0; i < new_elements_; i++)
		pdata[i] = value_;
}

template <class T> void MVECTOR<T>::cresize (size_t new_elements_) {
	clear();
	size_t align_elements = ((new_elements_ / step_elements) + 1) * step_elements;
#ifndef MVECTOR_USE__NEW
	pdata = (T*)malloc(align_elements * sizeof(T));
#endif
#ifdef MVECTOR_USE__NEW
	sptr = std::shared_ptr<T>(new T[align_elements], s_mvector_deleter<T>());
	pdata = sptr.get();
#endif
	elements = new_elements_;
	mem_elements = align_elements;
	size_t current_bytes = bytes();
	Total_MVECTOR_Bytes += current_bytes;
#ifdef MVECTOR_MAX_GROUPS
	Group_MVECTOR_Bytes[GroupID] += current_bytes;
#endif
}

template <class T> void MVECTOR<T>::cresize (size_t new_elements_, T value_) {
	cresize(new_elements_);
	for (size_t i = 0; i < new_elements_; i++)
		pdata[i] = value_;
}



template <class T> size_t MVECTOR<T>::bytes() {
	return mem_elements * sizeof(T);
}

template <class T> size_t MVECTOR<T>::total_bytes() {
	return Total_MVECTOR_Bytes;
}

template <class T> T* MVECTOR<T>::data() {
	return pdata;
}

template <class T> T& MVECTOR<T>::operator[](size_t index_) {
	// if (pdata == NULL) resize(1); // Crash if pdata == NULL.
	// if (index_ >= elements) return pdata[0]; // Crash if wrong index is called.
	return pdata[index_];
}

template <class T> MVECTOR<T>& MVECTOR<T>::operator=(MVECTOR<T>& mvector_) {
	if (this != &mvector_) {
		clear();
		mvector_.get_steps(step_elements, step_elements_back);
		initialize(mvector_.size());
		for (int i = 0; i < mvector_.size(); i++)
			pdata[i] = mvector_[i];
	}
	return *this;
}

template <class T> T& MVECTOR<T>::front() {
	if (elements == 0) resize(1);
	return pdata[0];
}
template <class T> T& MVECTOR<T>::back() {
	if (elements == 0) resize(1);
	return pdata[elements-1];
}

template <class T> void MVECTOR<T>::push_back(T value_) {
	elements++;
	if (elements >= mem_elements)
		resize(elements);
	pdata[elements-1] = value_;
}

template <class T> void MVECTOR<T>::pop_back() {
	if (elements == 0) return;
	elements--;
	if (mem_elements - elements >= step_elements_back) {
		size_t align_elements = ((elements / step_elements) + 1) * step_elements;
		if (align_elements != mem_elements)
			resize(elements);
	}
}

template <class T> void MVECTOR<T>::get_steps(
	size_t& step_elements_,
	size_t& step_elements_back_
) {
		step_elements_ = step_elements;
		step_elements_back_ = step_elements_back;
}

	

template <class T>
int MVECTOR<T>::clear() {
	if (pdata == NULL) return 1;
	size_t current_bytes = bytes();
	Total_MVECTOR_Bytes -= current_bytes;
#ifdef MVECTOR_MAX_GROUPS
	Group_MVECTOR_Bytes[GroupID] -= current_bytes;
#endif
#ifndef MVECTOR_USE__NEW
	free(pdata);
#endif
#ifdef MVECTOR_USE__NEW
	sptr.reset();
#endif
	pdata = NULL;
	elements = 0;
	mem_elements = 0;
	return 0;
}

template <class T>
size_t MVECTOR<T>::size() {
	return elements;
}



#ifdef MVECTOR_MAX_GROUPS
template <class T> size_t MVECTOR<T>::total_bytes(int group_id_) {
	if (group_id_ >= MVECTOR_MAX_GROUPS) return (size_t)group_id_;
	return Group_MVECTOR_Bytes[group_id_];
}

template <class T> void MVECTOR<T>::set_group(int group_id_) {
	if (group_id_ >= MVECTOR_MAX_GROUPS) return;
	size_t current_bytes = bytes();
	if (current_bytes > 0)
		Group_MVECTOR_Bytes[GroupID] -= current_bytes;
	GroupID = group_id_;
	if (current_bytes > 0)
		Group_MVECTOR_Bytes[GroupID] += current_bytes;
}
#endif


}; // namespace MVECTOR


#endif









