// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
$begin cppad_jit_poly.cpp$$
$spell
   cppad_jit
$$

$section cppad_jit Speed: Second Derivative of a Polynomial$$


$head Specifications$$
$cref link_poly$$

$head Implementation$$
// a cppad_jit version of this test is not yet implemented
$srccode%cpp% */
# include <map>
# include <cppad/utility/vector.hpp>

// list of possible options
extern std::map<std::string, bool> global_option;

bool link_poly(
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &a        ,  // coefficients of polynomial
   CppAD::vector<double>     &z        ,  // polynomial argument value
   CppAD::vector<double>     &ddp      )  // second derivative w.r.t z
{  return false; }
/* %$$
$end
*/
