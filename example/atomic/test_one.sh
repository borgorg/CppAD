#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Run one of the tests
if [ "$1" = "" ]
then
	echo "usage: test_one.sh file [extra]"
	echo "file is the *.cpp file name with extension"
	echo "and extra is extra options for g++ command"
	exit 1
fi
if [ ! -e "$1" ]
then
	echo "Cannot find the file $1"
	exit 1
fi
# determine the function name
fun=`grep "^bool *[a-zA-Z0-9_]* *( *void *)" $1 | tail -1 | \
	sed -e "s/^bool *\([a-zA-Z0-9_]*\) *( *void *)/\1/"`
#
if [ -e test_one.exe ]
then
	rm test_one.exe
fi
if [ -e test_one.cpp ]
then
	rm test_one.cpp
fi
sed < atomic.cpp > test_one.cpp \
-e '/ok *\&= *Run( /d' \
-e "s/.*This line is used by test_one.sh.*/	ok \&= Run( $fun, \"$fun\");/"
#
cxxflags='-Wall -pedantic-errors -std=c++11 -Wshadow'
if echo "$fun" | grep 'eigen' > /dev/null
then
	cxxflags=`echo "$cxxflags" | sed -e 's|-Wshadow||'`
fi
cmd="g++ test_one.cpp $*"
cmd="$cmd
	-o test_one.exe
	$cxxflags
	-L /home/bradbell/cppad.git/build/cppad_lib
	-lcppad_lib
	-g
	-fopenmp
	-lboost_thread
	-lpthread
	-DCPPAD_ADOLC_EXAMPLES
	-DCPPAD_EIGEN_EXAMPLES
	-I../..
"
if [ -e /home/bradbell/prefix/adolc/include ]
then
	cmd="$cmd -I/home/bradbell/prefix/adolc/include"
fi
if [ -e /home/bradbell/prefix/colpack/include ]
then
	cmd="$cmd -I/home/bradbell/prefix/colpack/include"
fi
if [ -e /home/bradbell/prefix/eigen/include ]
then
	cmd="$cmd -I/home/bradbell/prefix/eigen/include"
fi
for lib in lib lib64
do
	if [ -e /home/bradbell/prefix/adolc/$lib ]
	then
		cmd="$cmd -L/home/bradbell/prefix/adolc/$lib -ladolc -lColPack"
		export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/bradbell/prefix/adolc/$lib"
	fi
	if [ -e /home/bradbell/prefix/colpack/$lib ]
	then
		cmd="$cmd -L/home/bradbell/prefix/colpack/$lib -lColPack"
		export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/bradbell/prefix/colpack/$lib"
	fi
done
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/bradbell/cppad.git/build/cppad_lib"
echo $cmd
$cmd
#
echo "./test_one.exe"
if ! ./test_one.exe
then
	exit 1
fi
exit 0
