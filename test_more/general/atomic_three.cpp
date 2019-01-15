/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

/*
g_0 (x) = x_0 * x_0
g_1 (x) = x_0 * x_1
g_2 (x) = x_1 * x_2
g_3 (x) = x_2 * x_2

*/
# include <cppad/cppad.hpp>  // CppAD include file
namespace {                  // start empty namespace
using CppAD::vector;         // abbreviate CppAD::vector using vector

// ---------------------------------------------------------------------------
// start definition of atomic derived class using atomic_three interface
class atomic_optimize : public CppAD::atomic_three<double> {
public:
    // can use const char* name when calling this constructor
    atomic_optimize(const std::string& name) : // can have more arguments
    CppAD::atomic_three<double>(name)          // inform base class of name
    { }

private:
    // calculate type_y
    virtual bool for_type(
        const vector<double>&               parameter_x ,
        const vector<CppAD::ad_type_enum>&  type_x      ,
        vector<CppAD::ad_type_enum>&        type_y      )
    {   assert( parameter_x.size() == type_x.size() );
        bool ok = type_x.size() == 3; // n
        ok     &= type_y.size() == 4; // m
        if( ! ok )
            return false;
        type_y[0] = type_x[0];
        type_y[1] = std::max( type_x[0], type_x[1] );
        type_y[2] = std::max( type_x[1], type_x[2] );
        type_y[3] = type_x[2];
        return true;
    }
    // calculate depend_x
    virtual bool rev_depend(
        const vector<double>&               parameter_x ,
        vector<bool>&                       depend_x      ,
        const vector<bool>&                 depend_y      )
    {   assert( parameter_x.size() == depend_x.size() );
        bool ok = depend_x.size() == 3; // n
        ok     &= depend_y.size() == 4; // m
        if( ! ok )
            return false;
        depend_x[0] = depend_y[0] | depend_y[1];
        depend_x[1] = depend_y[1] | depend_y[2];
        depend_x[2] = depend_y[2] | depend_y[3];
        return true;
    }
    virtual bool forward(
        size_t                              need_y    ,
        size_t                              order_low ,
        size_t                              order_up  ,
        const vector<CppAD::ad_type_enum>&  type_x    ,
        const vector<double>&               taylor_x  ,
        vector<double>&                     taylor_y
    )
    {
# ifndef NDEBUG
        size_t n = taylor_x.size() / (order_up + 1);
        size_t m = taylor_y.size() / (order_up + 1);
# endif
        assert( n == 3 );
        assert( m == 4 );
        assert( order_low <= order_up );

        // return flag
        bool ok = order_up == 0;
        if( ! ok )
            return ok;

        // g_0 = x_0 * x_0
        taylor_y[0] = taylor_x[0] * taylor_x[0];
        // g_1 = x_0 * x_1
        taylor_y[1] = taylor_x[0] * taylor_x[1];
        // g_2 = x_1 * x_2
        taylor_y[2] = taylor_x[1] * taylor_x[2];
        // g_3 = x_2 * x_2
        taylor_y[3] = taylor_x[2] * taylor_x[2];

        return ok;
    }
}; // End of atomic_optimize class

// ---------------------------------------------------------------------------
bool check_dynamic_one(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::NearEqual;
    double eps = 10. * CppAD::numeric_limits<double>::epsilon();
    atomic_optimize afun("atomic_optimize");
    // Create the function f(u) = g(c, p, u) for this example.
    //
    // constant parameter
    double c_0 = 2.0;
    //
    // indepndent dynamic parameter vector
    size_t np = 1;
    CPPAD_TESTVECTOR(double) p(np);
    CPPAD_TESTVECTOR( AD<double> ) ap(np);
    ap[0] = p[0] = 3.0;
    //
    // independent variable vector
    size_t  nu  = 1;
    double  u_0 = 0.5;
    CPPAD_TESTVECTOR( AD<double> ) au(nu);
    au[0] = u_0;

    // declare independent variables and start tape recording
    size_t abort_op_index = 0;
    bool   record_compare = true;
    CppAD::Independent(au, abort_op_index, record_compare, ap);

    // create a dynamic parameter that is not used
    AD<double> p2 = 2.0 * ap[0];

    // call atomic function and store result in ay
    CPPAD_TESTVECTOR( AD<double> ) ax(3), av(4);
    ax[0] = c_0;   // x_0
    ax[1] = ap[0]; // x_1
    ax[2] = au[0]; // x_2
    afun(ax, av);

    // check type of result
    ok &= Constant( av[0] ); // v_0 = c * c
    ok &= Dynamic(  av[1] ); // v_1 = c * p
    ok &= Variable( av[2] ); // v_2 = p * u
    ok &= Variable( av[3] ); // v_3 = u * u

    // range space vector
    size_t ny = 3;
    CPPAD_TESTVECTOR( AD<double> ) ay(ny);
    for(size_t i = 0; i < ny; ++i)
        ay[i] = av[i];

    // create f: u -> y and stop tape recording
    CppAD::ADFun<double> f;
    f.Dependent (au, ay);  // f(u) = (c * c, c * p, p * u)

    // sequence properties
    ok &= f.size_dyn_ind() == 1;
    ok &= f.size_dyn_par() == 3;

    // optimize
    f.optimize();

    // sequence properties
    ok &= f.size_dyn_ind() == 1;
    ok &= f.size_dyn_par() == 2;

    // check
    double check;

    // check zero order forward mode
    size_t q;
    CPPAD_TESTVECTOR( double ) u_q(nu), y_q(ny);
    q      = 0;
    u_q[0] = u_0;
    y_q    = f.Forward(q, u_q);
    check  = c_0 * c_0;
    ok    &= NearEqual(y_q[0] , check,  eps, eps);
    check = c_0 * p[0];
    ok    &= NearEqual(y_q[1] , check,  eps, eps);
    check = p[0] * u_0;
    ok    &= NearEqual(y_q[2] , check,  eps, eps);

    // set new value for dynamic parameters
    p[0]  = 2.0 * p[0];
    f.new_dynamic(p);
    y_q   = f.Forward(q, u_q);
    check = c_0 * c_0;
    ok    &= NearEqual(y_q[0] , check,  eps, eps);
    check = c_0 * p[0];
    ok    &= NearEqual(y_q[1] , check,  eps, eps);
    check = p[0] * u_0;
    ok    &= NearEqual(y_q[2] , check,  eps, eps);

    return ok;
}
// ---------------------------------------------------------------------------
bool check_dynamic_two(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::NearEqual;
    double eps = 10. * CppAD::numeric_limits<double>::epsilon();
    atomic_optimize afun("atomic_optimize");
    // Create the function f(u) = g(c, p, u) for this example.
    //
    // indepndent dynamic parameter vector
    size_t np = 1;
    CPPAD_TESTVECTOR(double) p(np);
    CPPAD_TESTVECTOR( AD<double> ) ap(np);
    ap[0] = p[0] = 3.0;
    //
    // independent variable vector
    size_t  nu  = 1;
    double  u_0 = 0.5;
    CPPAD_TESTVECTOR( AD<double> ) au(nu);
    au[0] = u_0;

    // declare independent variables and start tape recording
    size_t abort_op_index = 0;
    bool   record_compare = true;
    CppAD::Independent(au, abort_op_index, record_compare, ap);

    // create a dynamic parameter that is used by atomic function
    // but not needed to compute f(u)
    AD<double> r = 2.0 * ap[0];

    // call atomic function and store result in ay
    CPPAD_TESTVECTOR( AD<double> ) ax(3), av(4);
    ax[0] = au[0]; // x_0
    ax[1] = ap[0]; // x_1
    ax[2] = r ;    // x_2
    afun(ax, av);

    // check type of result
    ok &= Variable( av[0] );  // v_0 = u * u , used
    ok &= Variable( av[1] );  // v_1 = u * p , used
    ok &= Dynamic( av[2] );   // v_2 = r * p , not used
    ok &= Dynamic( av[3] );   // v_3 = r * r , not used

    // range space vector
    size_t ny = 2;
    CPPAD_TESTVECTOR( AD<double> ) ay(ny);
    for(size_t i = 0; i < ny; ++i)
        ay[i] = av[i];

    // create f: u -> y and stop tape recording
    CppAD::ADFun<double> f;
    f.Dependent (au, ay);  // f(u) = (u * u, u * p)

    // sequence properties
    ok &= f.size_dyn_ind() == 1;
    ok &= f.size_dyn_par() == 4;

    // optimize
    f.optimize();

    // sequence properties
    ok &= f.size_dyn_ind() == 1;

    // check
    double check;

    // check zero order forward mode
    size_t q;
    CPPAD_TESTVECTOR( double ) u_q(nu), y_q(ny);
    q      = 0;
    u_q[0] = u_0;
    y_q    = f.Forward(q, u_q);
    check  = u_0 * u_0;
    ok    &= NearEqual(y_q[0] , check,  eps, eps);
    check = u_0 * p[0];
    ok    &= NearEqual(y_q[1] , check,  eps, eps);

    // set new value for dynamic parameters
    p[0]  = 2.0 * p[0];
    f.new_dynamic(p);
    y_q   = f.Forward(q, u_q);
    check = u_0 * u_0;
    ok    &= NearEqual(y_q[0] , check,  eps, eps);
    check = u_0 * p[0];
    ok    &= NearEqual(y_q[1] , check,  eps, eps);

    return ok;
}
// ---------------------------------------------------------------------------
}  // End empty namespace

bool atomic_three(void)
{   bool ok = true;
    ok     &= check_dynamic_one();
    ok     &= check_dynamic_two();
    return ok;
}