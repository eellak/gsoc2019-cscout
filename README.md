
## Introduction
CScout is a source code analyzer and refactoring browser for collections
of C programs.  It can process workspaces of multiple projects (a project
is defined as a collection of C source files that are linked together)
mapping the complexity introduced by the C preprocessor back into
the original C source code files.  CScout takes advantage of modern
hardware (fast processors and large memory capacities) to analyze
C source code beyond the level of detail and accuracy provided
by  current compilers and linkers.  The analysis CScout performs takes
into account the identifier scopes introduced by the C preprocessor and
the C language proper scopes and namespaces.  CScout has already been
applied on projects of tens of thousands of lines to millions of lines,
like the Linux, OpenSolaris, and FreeBSD kernels, and the Apache web
server. 

More info about the project can be found in this project's [wiki](https://github.com/eellak/gsoc2019-CScout/wiki) and the original project's [page](https://www.spinellis.gr/cscout/).

This project has a [gist](https://gist.github.com/dimstil/3a65a18b675235e367eeefee2e554ccb).

## Building, Testing, Installing, Using
CScout has been compiled and tested on GNU/Linux (Ubuntu 18.04). In order to
build and use CScout you need a Unix (like) system
with a modern C++ compiler, GNU make, Perl and npm with NodeJS and 
[Microsoft's C++ Rest SDK](https://github.com/microsoft/cpprestsdk). 
To test CScout you also need to be able to run Java from the command line,
in order to use the HSQLDB database.
To view CScout's diagrams you must have the
[GraphViz](http://www.graphviz.org) dot command in
your executable file path.
* To install npm with NodeJS run `sudo apt-get install npm nodejs` and for Microsoft's C++ Rest SDK run `sudo apt-get install libcpprest-dev`.
* To build run `sudo make`. You can also use the `-j` make option to increase the build's speed.
* To build and test, run `make test`.
* To test Server, run `make testServ`.
* To install (typically after building and testing), run `sudo make install`.
* To see CScout in action run `make example`.

Under FreeBSD use `gmake` rather than `make`.

Testing requires an installed version of _HSQLDB_.
If this is already installed in your system, specify to _make_
the absolute path of the *hsqldb* directory, e.g.
`make HSQLDB_DIR=/usr/local/lib/hsqldb-2.3.3/hsqldb`.
Otherwise, _make_ will automatically download and unpack a local
copy of _HSQLDB_ in the current directory.


## Contributing
* You can contribute to any of the [open issues](https://github.com/dspinellis/cscout/issues) or you can open a new one describing what you want to di.
* For small-scale improvements and fixes simply submit a GitHub pull request.
Each pull request should cover only a single feature or bug fix.
The changed code should follow the code style of the rest of the program.
If you're contributing a feature don't forget to update the documentation.
If you're submitting a bug fix, open a corresponding GitHub issue,
and refer to the issue in your commit.
Avoid gratuitous code changes.
Ensure that the tests continue to pass after your change.
If you're fixing a bug or adding a feature related to the language, add a corresponding test case.
* Before embarking on a large-scale contribution, please open a GitHub issue.

## Contributors
Mentors: Diomidis Spinellis [dspinellis](https://github.com/dspinellis)  
Mentors: ΑΝΑΣΤΑΣΙΑ ΔΕΛΙΓΚΑ  
GSOC-2019 Participant: Dimitrios Styliaras [dimstil](https://github.com/dimstil)  
Orginization: Open Technologies Alliance - GFOSS  

## Links
For more details, examples, and documentation visit the project's
[web site](http://www.spinellis.gr/cscout).
[Original Repo](https://github.com/dspinellis/cscout).
