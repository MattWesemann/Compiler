Generate an AST
===============

Building
--------

You'll need CMake, Flex, Bison, and your favorite C++ compiler.

Clone or unzip the source somewhere.

    $ mkdir Hellman
    $ cd Hellman
    $ git clone https://github.com/MattWesemann/Compiler.git
    
Try running:

    $ ./rebuild
    
to automatically `cmake`, `make`, and create the `./build` directory (with `frontend` in it).

Make a build directory (naming doesn't matter). While the code should work in or out of the source directory, it may be more convenient to keep everything packaged under another directory.

    $ mkdir build
    $ cd build

Call CMake, passing in your generator and the path to the source you cloned or unzipped earlier.

    $ cmake -G "Unix Makefiles" ../Compiler/

Make the directory for auto-generated files. CMake doesn't do this and we're working on it.

    $ mkdir flex-bison-autogen

Run `make`

    $ make

The binary you're interested in is called `frontend`.

    $ ./frontend
    int x;
    int y;
    x = 5;
    y = 10;
    int z = x * y ^ 7;
    207 Program
    206 Declarations
    203  'int'
    204  'x'
    211 Declarations
    208  'int'
    209  'y'
    213 Assignment
    214  'y'
    212  '5'
    216 Assignment
    217  'y'
    215  '10'

    207 206 211 213 216
    206 203 204
    211 208 209
    213 214 212
    216 217 215
    
Batch file testing
------------------

To batch file test, and after cloning and building:
    
    $ ./make-png-loop.sh {path-to-directory-containing-test-files}

Ensure that the path information contains a `/` (slash) at the end.

The console will tell you if the test language passed, and all generated files will be in the `./output/{test-file0}`, `./output/{test-file1}`, ..., etc. directories.


What we're supposed to do
------------------------

- Parse a language like this:

        //language example 0
        const int a = 1;
        int x = 1<<a;
        int y, z = 3;

        y = z - x;
        if ( y <= 0 ) {
           z = (x+2) + z*z ;
        } else {
           z = z / y;
        }
        return z;

- Generate line/column annotated error messages to stderr.

- Print the AST to stdout.

- Return 0 on successful AST creation.
