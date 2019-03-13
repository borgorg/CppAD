#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
#!/bin/bash -e
name=`echo $0 | sed -e 's|^bug/||' -e 's|\.sh$||'`
if [ "$0" != "bug/$name.sh" ]
then
	echo 'usage: bug/alloc_global.sh'
	exit 1
fi
# -----------------------------------------------------------------------------
if [ -e build/bug ]
then
	rm -r build/bug
fi
mkdir -p build/bug
cd build/bug
cmake ../..
# -----------------------------------------------------------------------------
echo "create $name.cpp"
cat << EOF > $name.cpp
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <omp.h>
# define NUMBER_THREADS  2

namespace {
    using CppAD::thread_alloc;

    // used to inform CppAD when we are in parallel execution mode
    bool in_parallel(void)
    {   return static_cast<bool>( omp_in_parallel() ); }

    // used to inform CppAD of current thread number thread_number()
    size_t thread_number(void)
    {   return static_cast<bool>( omp_get_thread_num() ); }

    // structure with information for one thread
    typedef struct {
        // function object (worker input)
        CppAD::vector<double> x;
    } thread_one_t;

    // vector with information for all threads
    thread_one_t thread_all_[NUMBER_THREADS];

    // --------------------------------------------------------------------
    // function that does the work for one thread
    void worker(void)
    {
        size_t thread_num = thread_number();
        thread_all_[thread_num].x.resize(1);
        thread_all_[thread_num].x[0]=static_cast<double>(thread_num);

        std::stringstream stream;
        stream << "thread_num = " << thread_num << std::endl;
        std::cout << stream.str();
    }
}

// Test routine called by the master thread (thread_num = 0).
bool alloc_global(void)
{   bool ok = true;
    using std::cout;
    using std::endl;

    size_t num_threads = NUMBER_THREADS;
    if( omp_get_max_threads() < num_threads )
    {   cout << "can't set num_threads = " << num_threads << endl;
        ok = false;
        return ok;
    }

    // call setup for using thread_alloc in parallel mode.
    thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);

    // Execute the worker function in parallel
    int thread_num;
# pragma omp parallel for
    for(thread_num = 0; thread_num < num_threads; thread_num++)
        worker();
// end omp parallel for

    // now inform CppAD that there is only one thread
    thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);

    for(thread_num = 0; thread_num < num_threads; thread_num++)
    {   // check calculations by this thread in parallel model
        ok &= thread_all_[thread_num].x[0] == static_cast<double>(thread_num);

        // free memory that was allocated by thread thread_num
        thread_all_[thread_num].x.resize(0);
    }

    return ok;
}
int main(void)
{   bool ok = alloc_global();
    std::cout << "OK = " << ok << std::endl;
    return int(! ok);
}
EOF
echo "g++ -g $name.cpp -I../../include -fopenmp -std=c++11 -o $name"
g++ -g $name.cpp -I../../include -fopenmp -std=c++11 -o $name
#
echo "./$name"
./$name
#
# -----------------------------------------------------------------------------
echo "bug/$name.sh: OK"
exit 0
