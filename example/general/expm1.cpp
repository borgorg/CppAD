// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin expm1.cpp}

The AD exp Function: Example and Test
#####################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end expm1.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cmath>

bool expm1(void)
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
   ay[0] = CppAD::expm1(ax[0]);

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);

   // expx0 value
   double expx0 = std::exp(x0);
   ok &= NearEqual(ay[0], expx0-1.0,  eps, eps);

   // forward computation of first partial w.r.t. x[0]
   CPPAD_TESTVECTOR(double) dx(n);
   CPPAD_TESTVECTOR(double) dy(m);
   dx[0] = 1.;
   dy    = f.Forward(1, dx);
   ok   &= NearEqual(dy[0], expx0, eps, eps);

   // reverse computation of derivative of y[0]
   CPPAD_TESTVECTOR(double)  w(m);
   CPPAD_TESTVECTOR(double) dw(n);
   w[0]  = 1.;
   dw    = f.Reverse(1, w);
   ok   &= NearEqual(dw[0], expx0, eps, eps);

   // use a VecAD<Base>::reference object with exp
   CppAD::VecAD<double> v(1);
   AD<double> zero(0);
   v[zero]           = x0;
   AD<double> result = CppAD::expm1(v[zero]);
   ok   &= NearEqual(result, expx0-1.0, eps, eps);

   return ok;
}

// END C++
