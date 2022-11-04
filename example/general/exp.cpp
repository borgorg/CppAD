// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
$begin exp.cpp$$
$spell
   exp
$$

$section The AD exp Function: Example and Test$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cmath>

bool exp(void)
{  bool ok = true;

   using CppAD::AD;
   using CppAD::NearEqual;
   double eps = 10. * std::numeric_limits<double>::epsilon();

   // domain space vector
   size_t n  = 1;
   double x0 = 0.5;
   CPPAD_TESTVECTOR(AD<double>) ax(n);
   ax[0]     = x0;

   // declare independent variables and start tape recording
   CppAD::Independent(ax);

   // range space vector
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) ay(m);
   ay[0] = CppAD::exp(ax[0]);

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);

   // check value
   double check = std::exp(x0);
   ok &= NearEqual(ay[0], check,  eps, eps);

   // forward computation of first partial w.r.t. x[0]
   CPPAD_TESTVECTOR(double) dx(n);
   CPPAD_TESTVECTOR(double) dy(m);
   dx[0] = 1.;
   dy    = f.Forward(1, dx);
   ok   &= NearEqual(dy[0], check, eps, eps);

   // reverse computation of derivative of y[0]
   CPPAD_TESTVECTOR(double)  w(m);
   CPPAD_TESTVECTOR(double) dw(n);
   w[0]  = 1.;
   dw    = f.Reverse(1, w);
   ok   &= NearEqual(dw[0], check, eps, eps);

   // use a VecAD<Base>::reference object with exp
   CppAD::VecAD<double> v(1);
   AD<double> zero(0);
   v[zero]           = x0;
   AD<double> result = CppAD::exp(v[zero]);
   ok   &= NearEqual(result, check, eps, eps);

   return ok;
}

// END C++
