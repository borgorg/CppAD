# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_ATOMIC_MAT_MUL_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_ATOMIC_MAT_MUL_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_mat_mul.hpp$$
$spell
$$

$section Atomic Matrix Multiply Class: Example Implementation$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
template <class Base>
class atomic_mat_mul : public CppAD::atomic_four<double> {
//
public:
    // ctor
    atomic_mat_mul(const std::string& name) :
    CppAD::atomic_four<double>(name)
    {   for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
            work_[thread] = reinterpret_cast<dim_vector*>( nullptr );
    }
    // set
    size_t set(
        size_t n_left, size_t n_right, size_t n_middle
    );
    // get
    void get(
        size_t call_id, size_t& n_left, size_t& n_right, size_t& n_middle
    );
private:
    //
    // matrix dimensions corresponding to a call_id
    struct call_struct {
        size_t n_left; size_t n_middle; size_t n_right; size_t thread;
    };
    // map from call_id to matrix dimensions
    typedef CppAD::vector<call_struct> call_vector;
    // Use pointers, to avoid false sharing between threads.
    dim_vector* work_[CPPAD_MAX_NUM_THREADS];
    //
    // -----------------------------------------------------------------------
    // overrides
    // -----------------------------------------------------------------------
    //
    // for_type
    bool for_type(
        size_t                                        call_id,
        const CppAD::vector<CppAD::ad_type_enum>&     type_x,
        CppAD::vector<CppAD::ad_type_enum>&           type_y
    ) override;
    //
    // rev_depend
    bool rev_depend(
        size_t                                         call_id,
        CppAD::vector<bool>&                           depend_x,
        const CppAD::vector<bool>&                     depend_y
    ) override;
    //
    // jac_sparsity
    bool jac_sparsity(
        size_t                                         call_id,
        bool                                           dependency,
        const CppAD::vector<bool>&                     select_x,
        const CppAD::vector<bool>&                     select_y,
        CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out
    ) override;
    //
    // hes_sparsity
    bool hes_sparsity(
        size_t                                         call_id,
        const CppAD::vector<bool>&                     select_x,
        const CppAD::vector<bool>&                     select_y,
        CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out
    ) override;
    //
    // Base forward
    bool forward(
        size_t                                           call_id,
        const CppAD::vector<bool>&                       select_y,
        size_t                                           order_low,
        size_t                                           order_up,
        const CppAD::vector<Base>&                       tx,
        CppAD::vector<Base>&                             ty
    ) override;
    //
    // AD<Base> forward
    bool forward(
        size_t                                           call_id,
        const CppAD::vector<bool>&                       select_y,
        size_t                                           order_low,
        size_t                                           order_up,
        const CppAD::vector< CppAD::AD<Base> >&          atx,
        CppAD::vector< CppAD::AD<Base> >&                aty
    ) override;
    //
    // Base reverse
    bool reverse(
        size_t                                           call_id,
        const CppAD::vector<bool>&                       select_x,
        size_t                                           order_up,
        const CppAD::vector<Base>&                       tx,
        const CppAD::vector<Base>&                       ty,
        CppAD::vector<Base>&                             px,
        const CppAD::vector<Base>&                       py
    ) override;
    //
    // AD<Base> reverse
    bool reverse(
        size_t                                           call_id,
        const CppAD::vector<bool>&                       select_x,
        size_t                                           order_up,
        const CppAD::vector< CppAD::AD<Base> >&          atx,
        const CppAD::vector< CppAD::AD<Base> >&          aty,
        CppAD::vector< CppAD::AD<Base> >&                apx,
        const CppAD::vector< CppAD::AD<Base> >&          apy
    ) override;
};
} // END_CPPAD_NAMESPACE

# undef CPPAD_ATOMIC_FOUR_FORWARD_AND_REVERSE
# include <cppad/example/atomic_four/atomic_mat_mul/set.hpp>
# include <cppad/example/atomic_four/atomic_mat_mul/get.hpp>
# include <cppad/example/atomic_four/atomic_mat_mul/for_type.hpp>
// END C++

# endif