# ifndef CPPAD_CREATE_DLL_LIB_HPP
# define CPPAD_CREATE_DLL_LIB_HPP
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
$begin create_dll_lib$$

$section Create a Dynamic Link Library$$

$head Syntax$$
$codei%# include <cppad/utility/create_dll_lib.hpp>
%$$
$icode%err_msg% = create_dyn_lib(%dll_file%, %csrc_files%)
%$$

$head Prototype$$
$srcthisfile%0// BEGIN_CREATE_DLL_LIB%//END_CREATE_DLL_LIB%1%$$

$head include$$
As with all the CppAD utilities, $code create_dll_lib.hpp$$ is included
by $code <cppad/cppad.hpp>$$, and can also be included separately.

$head dll_file$$
This is the file where the dynamic link library file named $icode dll_file$$.
This file name must have the proper extension for a dynamic link library
($code .so$$ on unix and $code .dll$$ on windows).

$head StringVector$$
The type $icode StringVector$$ is a simple vector with elements
of type $code std::string$$.

$head csrc_files$$
The vector $icode csrc_files$$ contians the names of the C soruce
files that are compiled and linked to the library.
These files do not have to have a specific extension.

$head err_msg$$
If this string is empty, no error occurred.
Otherwise the processing aboted and $icode err_msg$$ is the corresponding
error message.

$end
*/
# include <cppad/local/temp_file.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// create
// BEGIN_CREATE_DLL_LIB
template <class StringVector>
std::string create_dll_lib(
    const std::string&  dll_file   ,
    const StringVector& csrc_files )
// END_CREATE_DLL_LIB
{   using std::string;
    //
    // err_msg
    string err_msg = "";
    //
    // check the the std::system function exists
    int flag = std::system(nullptr);
    if( flag == 0 )
    {   err_msg = "C++ std::system function not availablee\n";
        return err_msg;
    }
    //
    // check the file extensions
# ifdef _WIN32
    string dll_ext = ".dll";
# else
    string dll_ext = ".so";
# endif
    size_t last_match = dll_file.rfind(dll_ext);
    size_t expected   = dll_file.size() - dll_ext.size();
    if( last_match != expected )
    {   err_msg  = "create_dll_lib: ";
        err_msg += dll_file + " does not end with " + dll_ext;
        return err_msg;
    }
    //
    // o_file_list, o_file_vec;
    string       o_file_list;
    StringVector o_file_vec( csrc_files.size() );
    //
    // i_csrc
    for(size_t i_csrc = 0; i_csrc < csrc_files.size(); ++i_csrc)
    {   //
        // c_file
        string c_file = csrc_files[i_csrc];
        //
        // o_file
        string o_file = local::temp_file();
        //
        // cmd
# ifdef _MSC_VER
        string cmd = "cl /Fo\"" + o_file + "\" /EHs /EHc /c /LD /Tc ";
        cmd       += c_file + " 1> nul 2> nul";
# else
        string cmd = "gcc -o " + o_file + " -c -g -fPIC "  + c_file;
# endif
        //
        // o_file
        // compile c_file and put result in o_file
        flag = std::system( cmd.c_str() );
        if(  flag != 0 )
        {   err_msg = "create_dll_lib: following system comamnd failed\n";
            err_msg += cmd;
            return err_msg;
        }
        //
        // o_file_list
        o_file_list += " " + o_file;
        //
        // o_file_vec
        o_file_vec[i_csrc] = o_file;
    }
# ifdef _MSC_VER
    string cmd = "link /DLL "   + o_file_list + " /OUT:" + dll_file;
    cmd       += " 1> nul 2> nul";
# else
    string cmd = "gcc -shared " + o_file_list + " -o "   + dll_file;;
# endif
    flag = std::system( cmd.c_str() );
    if(  flag != 0 )
    {   err_msg = "create_dll_lib: following system comamnd failed\n";
        err_msg += cmd;
        return err_msg;
    }
    //
    // remove o_file
    for(size_t i = 0; i < o_file_vec.size(); ++i)
    {   flag = std::remove( o_file_vec[i].c_str() );
        if(  flag != 0 )
        {   err_msg = "create_dll_lib: following system comamnd failed\n";
            err_msg += cmd;
            return err_msg;
        }
    }
    return err_msg;
}

} // END_CPPAD_NAMESPACE

# endif
