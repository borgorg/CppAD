// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/utility/vector.hpp>

/*
$begin sacado_sparse_hessian.cpp$$
$spell
   boolsparsity
   const
   Sacado
   bool
   CppAD
$$

$section Sacado Speed: Sparse Hessian$$

$srccode%cpp% */
// A sacado version of this test is not yet implemented
extern bool link_sparse_hessian(
   size_t                            size      ,
   size_t                            repeat    ,
   const CppAD::vector<size_t>&      row       ,
   const CppAD::vector<size_t>&      col       ,
   CppAD::vector<double>&            x         ,
   CppAD::vector<double>&            hessian   ,
   size_t&                           n_color
)
{
   return false;
}
/* %$$
$end
*/
