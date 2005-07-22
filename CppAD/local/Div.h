# ifndef CppADDivIncluded
# define CppADDivIncluded

// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
-------------------------------------------------------------------------------
$begin Div$$
$spell
	Var
	const
	Div
$$

$index binary, division$$
$index division, binary$$

$index binary, /$$
$index /, binary$$

$section The AD Binary Division Operator$$

$table
$bold Syntax$$ 
$cnext 
$syntax%%left% / %right%$$
$tend

$fend 20$$

$head Description$$
Suppose that either $italic left$$ or $italic right$$ is an
$syntax%AD<%Base%>%$$ object, 
and the other operand is an $code int$$, $italic Base$$
or $syntax%AD<%Base%>%$$ object.
In this case
$syntax%
	%left% / %right%
%$$
returns an
$syntax%AD<%Base%>%$$ object
where $code /$$ has the same interpretation as
for the $italic Base$$ type.

$head Operands$$
Note the value of the left and right operands are not changed
by this operation.
If the left operand has value zero and is a
$xref/glossary/Parameter/parameter/$$ or a $italic Base$$ operand,
the result is a parameter.
Otherwise,
if the left or right operand is a variable,
the result is a 
$xref/glossary/Variable/variable/$$.
$pre

$$
If the right operand is 
exactly equal to one and it is a
$xref/glossary/Parameter/parameter/$$ or a $italic Base$$ operand,
the result has the same derivative information as the 
left operand and no extra space on the tape is required for this operation.

$head Assumptions$$

If the $code /$$ operator is used with an 
$syntax%AD<%Base%>%$$ object,
it must be a
$xref/BinaryOp//Base type binary operator/$$
for the base type $italic Base$$.

In addition,
if $latex f$$ and $latex g$$ are 
$xref/glossary/Base Function/Base functions/$$
with the same range dimension,
$latex \[
	\D{[ f(x) / g(x) ]}{x} = 
		[1/g(x)] * \D{f(x)}{x} - [f(x)/g(x)^2] * \D{g(x)}{x} 
\] $$

$head Example$$
$children%
	Example/Div.cpp
%$$
The file
$xref/Div.cpp/$$
contains an example and a test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// Division operators: assume CppAD.h has already been included

template <class Base>
AD<Base> AD<Base>::operator /(const AD<Base> &right) const
{
	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	result.value  = value / right.value;

	if( Tape()->State() == Recording )
	{	if( Parameter(*this) )
		{	if( Variable(right) )
			{	if( ! IdenticalZero(value) )
				{	Tape()->RecordOp(DivpvOp, 
						result, value, right.index
					);
				}
			}
		}
		else if( Parameter(right) )
		{	if( IdenticalOne(right.value) )
			{	// z = left / 1
				result.MakeVariable(index);
			}
			else	Tape()->RecordOp(DivvpOp, 
					result, index, right.value
			);
		}
		else	Tape()->RecordOp(DivvvOp, 
			result, index, right.index
		);
	}
	return result;
}

CppADFoldBinaryOperator(AD<Base>, /)


} // END CppAD namespace

# endif 
