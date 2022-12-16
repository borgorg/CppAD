# ifndef CPPAD_CORE_AD_BINARY_HPP
# define CPPAD_CORE_AD_BINARY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
-------------------------------------------------------------------------------
{xrst_begin ad_binary}

AD Binary Arithmetic Operators
##############################

Syntax
******
*z* = *x* *Op* *y*

Purpose
*******
Performs arithmetic operations where either *x* or *y*
has type
``AD<`` *Base* > or
:ref:`VecAD@VecAD\<Base>::reference` .

Op
**
The operator *Op* is one of the following

.. csv-table::
   :widths: auto

   **Op**,**Meaning**
   ``+``,*z* is *x* plus *y*
   ``-``,*z* is *x* minus *y*
   ``*``,*z* is *x* times *y*
   ``/``,*z* is *x* divided by *y*

Base
****
The type *Base* is determined by the operand that
has type ``AD<`` *Base* > or ``VecAD<`` *Base* >:: ``reference`` .

x
*
The operand *x* has the following prototype

   ``const`` *Type* & *x*

where *Type* is
``VecAD<`` *Base* >:: ``reference`` ,
``AD<`` *Base* > ,
*Base* , or
``double`` .

y
*
The operand *y* has the following prototype

   ``const`` *Type* & *y*

where *Type* is
``VecAD<`` *Base* >:: ``reference`` ,
``AD<`` *Base* > ,
*Base* , or
``double`` .

z
*
The result *z* has the following prototype

   *Type* *z*

where *Type* is
``AD<`` *Base* > .

Operation Sequence
******************
This is an :ref:`atomic_base<glossary@Operation@Atomic>`
:ref:`glossary@AD of Base` operation
and hence it is part of the current
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .
{xrst_toc_hidden
   example/general/add.cpp
   example/general/sub.cpp
   example/general/mul.cpp
   example/general/div.cpp
}

Example
*******
The following files contain examples and tests of these functions.
Each test returns true if it succeeds and false otherwise.

.. csv-table::
   :widths: auto

   add.cpp,:ref:`add.cpp-title`
   sub.cpp,:ref:`sub.cpp-title`
   mul.cpp,:ref:`mul.cpp-title`
   div.cpp,:ref:`div.cpp-title`

Derivative
**********
If :math:`f` and :math:`g` are
:ref:`Base functions<glossary@Base Function>`

Addition
========

.. math::

   \D{[ f(x) + g(x) ]}{x} = \D{f(x)}{x} + \D{g(x)}{x}

Subtraction
===========

.. math::

   \D{[ f(x) - g(x) ]}{x} = \D{f(x)}{x} - \D{g(x)}{x}

Multiplication
==============

.. math::

   \D{[ f(x) * g(x) ]}{x} = g(x) * \D{f(x)}{x} + f(x) * \D{g(x)}{x}

Division
========

.. math::

   \D{[ f(x) / g(x) ]}{x} =
      [1/g(x)] * \D{f(x)}{x} - [f(x)/g(x)^2] * \D{g(x)}{x}

{xrst_end ad_binary}
-----------------------------------------------------------------------------
*/
# include <cppad/core/add.hpp>
# include <cppad/core/sub.hpp>
# include <cppad/core/mul.hpp>
# include <cppad/core/div.hpp>

# endif
