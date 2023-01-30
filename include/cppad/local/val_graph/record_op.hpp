# ifndef  CPPAD_LOCAL_VAL_GRAPH_RECORD_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_RECORD_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/call_op.hpp>
# include <cppad/core/numeric_limits.hpp>
//
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_graph_record_op dev}
{xrst_spell
   operands
   dep
}

Recording Value Operators on a Tape
###################################

Purpose
*******
These operations store a function in the :ref:`val_graph_tape-name`.

set_ind
*******
{xrst_literal
   // BEGIN_SET_IND
   // END_SET_IND
}
This clears all of the memory in the tape.
It then sets the number of independent values and
places the constant nan directly after the last independent value.
The return value is the index where the nan is placed in the
value vector; i.e., *n_ind* .
The constant nan uses one operator,
one argument, one constant, and one result.
This is the first step in a creating a recording.

record_op
*********
{xrst_literal
   // BEGIN_RECORD_OP
   // END_RECORD_OP
}
This places a :ref:`val_unary_op-name` or :ref`val_binary_op-name`
operator in the tape.
The argument *op_enum* identifies the operator.
The argument *op_arg* is a vector that contains the index of the operands
in the value vector.
It has length one (two) if this is a unary (binary) operator.
The return value is the index were the result of the operation
is placed in the value vector.

record_con_op
*************
{xrst_literal
   // BEGIN_RECORD_CON_OP
   // END_RECORD_CON_OP
}
This places a :ref:`val_con_op-name` operator in the tape.
The return value is the index were *constant* is placed
in the value vector.

record_call_op
**************
{xrst_literal
   // BEGIN_RECORD_MAP_OP
   // END_RECORD_MAP_OP
}
This places a :ref:`val_call_op-name` operator in the tape.

atomic_index
============
This is the *atomic_index* for the atomic function; see
:ref:`atomic_index@index_out` in the case where *index_in* is zero.

call_id
=======
This is the *call_id* for this use of the atomic function; see
:ref:`atomic_four_call@call_id` .

n_res
=====
This is the number of values returned by the atomic function call
and placed in the value vector.

fun_arg
=======
This vector has size equal to the number of arguments to the atomic function
for this *call_id* .
(The combination of *atomic_index* and *call_id* must specify a function.)
The *j*-th element of *fun_arg* is the index on the value vector of
the *j*-th argument to the function.

return
======
The ``record_con_op`` function returns the index in the value vector where
the first result is placed in the value vector.
A total of *n_res* results are placed in the value vector.

set_dep
*******
{xrst_literal
   // BEGIN_SET_DEP
   // END_SET_DEP
}
This sets the dependent vector to the corresponding indices
in the value vector.
This is last step in creating a recording.

{xrst_end val_graph_record_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_SET_IND
template <class Value>
addr_t tape_t<Value>::set_ind(size_t n_ind)
// END_SET_IND
{  Value nan = CppAD::numeric_limits<Value>::quiet_NaN();
   n_ind_ = n_ind;
   n_val_ = n_ind;
   dep_vec_.clear();
   op_vec_.clear();
   con_vec_.clear();
   addr_t nan_addr = record_con_op(nan);
   assert ( size_t(nan_addr) == n_ind_ );
   assert( n_val_ == n_ind + 1 );
   //
   return nan_addr;
}
// ----------------------------------------------------------------------------
// BEGIN_RECORD_OP
template <class Value>
addr_t tape_t<Value>::record_op(op_enum_t op_enum, const Vector<addr_t>& op_arg)
// END_RECORD_OP
{  //
   // res_index
   addr_t res_index = addr_t( n_val_) ;
   //
   // arg_index
   addr_t arg_index = addr_t( arg_vec_.size() );
   //
   // op_ptr
   op_base_t<Value>* op_ptr = nullptr;
   switch(op_enum)
   {
      case add_op_enum:
      op_ptr = add_op_t<Value>::get_instance();
      break;

      case sub_op_enum:
      op_ptr = sub_op_t<Value>::get_instance();
      break;

      default:
      assert( false );
   }
   //
   // op_vec_
   op_info_t op_info = { arg_index, res_index, op_ptr};
   op_vec_.push_back(op_info);
   //
   // arg_vec_
   size_t n_op_arg = op_ptr->n_arg(arg_index, arg_vec_);
   for(size_t i = 0; i < n_op_arg; ++i)
      arg_vec_.push_back( op_arg[i] );
   //
   // n_val_
   n_val_ = n_val_ + op_ptr->n_res(arg_index, arg_vec_);
   //
   return res_index;
}
// ----------------------------------------------------------------------------
// BEGIN_RECORD_CON_OP
template <class Value>
addr_t tape_t<Value>::record_con_op(const Value& constant)
// END_RECORD_CON_OP
{  //
   // con_index
   addr_t con_index = addr_t( con_vec_.size() );
   con_vec_.push_back( constant );
   //
   // res_index
   addr_t res_index = addr_t( n_val_ );
   //
   // arg_index
   addr_t arg_index = addr_t( arg_vec_.size() );
   //
   // op_ptr
   op_base_t<Value>* op_ptr = con_op_t<Value>::get_instance();
   //
   // op_vec_
   op_info_t op_info = { arg_index, res_index, op_ptr};
   op_vec_.push_back(op_info);
   //
   // arg_vec_
   arg_vec_.push_back( con_index );
   //
   // n_val_
   n_val_ = n_val_ + op_ptr->n_res(arg_index, arg_vec_);
   //
   return res_index;
}
// ----------------------------------------------------------------------------
// BEGIN_RECORD_MAP_OP
template <class Value>
addr_t tape_t<Value>::record_call_op(
   size_t  atomic_index          ,
   size_t  call_id               ,
   size_t  n_res                 ,
   const Vector<addr_t>& fun_arg )
// END_RECORD_MAP_OP
{  //
   // res_index
   addr_t res_index = addr_t( n_val_ );
   //
   // arg_index
   addr_t arg_index = addr_t( arg_vec_.size() );
   //
   // op_ptr
   op_base_t<Value>* op_ptr = call_op_t<Value>::get_instance();
   //
   // op_vec_
   op_info_t op_info = { arg_index, res_index, op_ptr};
   op_vec_.push_back(op_info);
   //
   // arg_vec_
   size_t n_arg = 4 + fun_arg.size();
   arg_vec_.push_back( addr_t( n_arg ) );
   arg_vec_.push_back( addr_t( n_res ) );
   arg_vec_.push_back( addr_t( atomic_index ) );
   arg_vec_.push_back( addr_t( call_id ) );
   for(size_t i = 0; i < fun_arg.size(); ++i)
      arg_vec_.push_back( fun_arg[i] );
   //
   // n_val_
   n_val_ = n_val_ + n_res;
   //
   return res_index;
}
// ----------------------------------------------------------------------------
// BEGIN_SET_DEP
template <class Value>
void tape_t<Value>::set_dep(const Vector<addr_t>& dep_vec)
// END_SET_DEP
{  dep_vec_ = dep_vec; }
// ----------------------------------------------------------------------------

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
