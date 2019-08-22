/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

// CppAD::vector tests
# include <cppad/utility/vector.hpp>
# include <vector>
# include <algorithm>

namespace { // BEGIN_EMPTY_NAMESPACE

bool test_find(void)
{   // find requires an input iterator
    bool ok = true;
    typedef CppAD::vector<char>::iterator iterator;
    //
    size_t n = 10;
    CppAD::vector<char> vec(n);
    for(size_t i = 0; i < n; ++i)
        vec[i] = static_cast<char>( '0' + i);
    iterator itr;
    //
    itr = std::find<iterator, char>(vec.begin(), vec.end(), '3');
    ok &= itr != vec.end();
    ok &= *itr == '3';
    //
    itr = std::find<iterator, char>(vec.begin(), vec.end(), 'a');
    ok &= itr == vec.end();
    //
    return ok;
}

bool test_copy(void)
{   // copy requires a forward iterator
    bool ok = true;
    typedef CppAD::vector<char>::iterator iterator;
    //
    size_t n = 10;
    CppAD::vector<char> src(n), des(n);
    for(size_t i = 0; i < n; ++i)
        src[i] = static_cast<char>( '0' + i);
    //
    std::copy<iterator, iterator>(src.begin(), src.end(), des.begin());
    //
    for(size_t i = 0; i < n; ++i)
        ok &= src[i] == des[i];
    //
    return ok;
}

bool test_copy_backward(void)
{   // copy requires a bidirectional iterator
    bool ok = true;
    typedef CppAD::vector<char>::iterator iterator;
    //
    size_t n = 10;
    CppAD::vector<char> src(n), des(n);
    for(size_t i = 0; i < n; ++i)
        src[i] = static_cast<char>( '0' + i);
    //
    std::copy_backward<iterator, iterator>(src.begin(), src.end(), des.end());
    //
    for(size_t i = 0; i < n; ++i)
        ok &= src[i] == des[i];
    //
    return ok;
}


} // END_EMPTY_NAMESPACE

bool cppad_vector(void)
{   bool ok = true;
    //
    ok &= test_find();
    ok &= test_copy();
    ok &= test_copy_backward();
    //
    return ok;
}