libSBOLc: SBOL C library
========================

[libSBOLc](https://github.com/SynBioDex/libSBOLc) is a C library for working with the [Synthetic Biology Open Language (SBOL)](http://sbolstandard.org). It uses the XML schema and example files from [libSBOLj](https://github.com/SynBioDex/libSBOLj), but the rest of the code was developed separately. The two libraries should eventually present similar interfaces, except where language conventions differ.

Here is some example client code that will recreate valid04_dna_component_annotation.xml, which is one of the test cases from libSBOLj.

    #include "sbol.h"

	Document* CreateValid04() {
		Document* doc = createDocument();

		// components
		DNAComponent *dc1 = createDNAComponent(doc, "http://example.com/dc1");
		DNAComponent *dc2 = createDNAComponent(doc, "http://example.com/dc2");
		setDNAComponentDisplayID(dc1, "DC1");
		setDNAComponentDisplayID(dc2, "DC2");
		setDNAComponentName(dc1, "DnaComponent1");
		setDNAComponentName(dc2, "DnaComponent2");
		setDNAComponentDescription(dc1, "DnaComponent with one sequence annotation");
		setDNAComponentDescription(dc2, "Another DNA component");

		// sequence
		DNASequence *ds1 = createDNASequence(doc, "http://example.com/ds1");
		setDNASequenceNucleotides(ds1, "tccctatcagtgat");
		setDNAComponentSequence(dc1, ds1);

		// annotation
		SequenceAnnotation *sa1 = createSequenceAnnotation(doc, "http://example.com/sa1");
		setSequenceAnnotationSubComponent(sa1, dc2);
		addSequenceAnnotation(dc1, sa1);

		return doc;
	}

To make a complete program, you could add:

    void main() {
    	Document *doc = CreateValid04();
    	writeDocument(doc, "valid04_dna_component_annotation.xml");
    	deleteDocument(doc);
    }


The other examples follow the same format, and can be found [here](https://github.com/SynBioDex/libSBOLc/tree/master/examples/code).

The rest of this file is a quick guide to getting started with common tasks. For more detailed information about the code itself, see [the online documentation](http://synbiodex.github.com/libSBOLc).

Downloading the binaries
------------------------

Shared library files are available [here](https://github.com/SynBioDex/libSBOLc/downloads) for Windows, Mac OSX, and Linux/Unix.

Building from source
--------------------

To compile libSBOLc, you will need:

* [git](http://git-scm.com/) for downloading the source code
* [CMake](http://www.cmake.org/cmake/resources/software.html) for generating platform-specific build instructions
* a C compiler, such as [gcc](http://gcc.gnu.org/) or [MinGW](http://www.mingw.org/wiki/InstallationHOWTOforMinGW)

First, download them from the links above. Or if you're on Debian/Ubuntu this command should install everything:

    sudo apt-get install git cmake-qt-gui build-essential libxml2-dev

If you want to update the documentation you also need [Doxygen](http://www.stack.nl/~dimitri/doxygen/), and to generate the Python wrapper you need [SWIG](http://www.swig.org/). To install them on Debian/Ubuntu:

    sudo apt-get install doxygen-gui swig

Then, clone the repository:

    git clone git://github.com/SynBioDex/libSBOLc.git

This will create a <code>libSBOLc</code> directory with the code. Next, run CMake (cmake-qt-gui on linux). For "Where is the source code" enter the path to your <code>libSBOLc</code> folder. "Where to build the binaries" can technically be anywhere, but it's only been tested with <code>libSBOLc/build</code>.

Click <code>Configure</code>, and choose what type of compiler you want to generate instructions for. All the development has been done using "default native compilers" and MinGW on Windows or Unix makefiles on Mac/Linux. CMake should also be able to generate projects for Eclipse, Visual Studio, XCode, etc. However, that will probably involve adjusting some paths.

The first time you click <code>Configure</code> CMake will list variables, like <code>CMAKE_BUILD_TYPE</code> and <code>LIBXML2_INCLUDE_DIR</code>, in red. That means
they've been updated. To build the main SBOL library, just click <code>Configure</code> again until the red goes away. This is also where you set up the optional
targets: examples, tests, manual, and Python wrapper. To add them check the appropriate boxes (SBOL_BUILD_EXAMPLES, for example) and then <code>Configure</code> again to
adjust the settings. There's one other SBOL-specific option: <code>SBOL_DEBUG_STATEMENTS</code> will cause libSBOLc to be compiled with some extra debugging statements.
A lot of other options might be visibile too; checking <code>Grouped</code> at the top makes things more managable. Once it's all set, click <code>Generate</code> to create the compiler instructions.

The last step is to <code>cd</code> into the <code>libSBOLc/build</code> folder and run the compiler.

    make

or

    mingw32-make.exe

Binaries will be generated in the <code>libSBOLc/release</code> folder.

Testing
-------

Once you've configured and built libSBOLc, you can run some simple tests to check that everything works. <code>cd</code> into the <code>libSBOLc/release/tests</code> folder and run:

    ./sbol_run_tests

or 

    sbol_run_tests.exe

Incorporating SBOL into your code
-------------------------------------

To use libSBOLc in your own code, <code>#include "sbol.h"</code>. Then there are only a few important functions you need to know to get started reading, writing, and manipulating SBOL files:

* createDocument makes a new Document, and deleteDocument frees it from memory.
  The other SBOL objects are created with an existing Document as their first argument.

* readDocument imports SBOL objects from a file, and writeDocument serializes them again.

* isValidSBOL checks that an xmlDoc conforms to the SBOL schema. Using it involves parsing with libxml.
  There's an example of that in
  [sbol_validate.c](https://github.com/SynBioDex/libSBOLc/blob/master/examples/code/sbol_validate.c),
  but it shouldn't be necessary in most cases since readDocument and writeDocument
  both call it internally.

* There are constructors, destructors, getters, and setters for each type of SBOL object.
  For the most part they follow a pretty obvious formula:
  setDNAComponentDisplayID and getDNAComponentDisplayID, for example. But there are also some non-obvious ones, like 
  addPrecedesRelationship. For those the
  [index of all available functions](http://synbiodex.github.com/libSBOLc/globals_func.html)
  is a good place to look. There's also code to create each of the xml example files in
  [libSBOLc/examples/code](https://github.com/SynBioDex/libSBOLc/tree/master/examples/code).

Linking to the SBOL library
---------------------------

There are different ways to do this, but this is probably the simplest. To compile the sbol_validate example using MinGW on Windows:

    mingw32-gcc.exe -I headers sbol_validate.c -o validate libsbol.dll

and the equivalent command on Mac/Linux:

    gcc -I headers sbol_validate.c -o validate libsbol.so

The <code>-I</code> tells <code>gcc</code> where to look for headers, and <code>-o</code> gives the name of the generated executable. If you get an error like "library not loaded... image not found" on Mac OSX, you probably need to tell the system to look in the current folder for libsbol:

    export DYLD_LIBRARY_PATH=.:$DYLD_LIBRARY_PATH

The same thing should work in Linux, except it's <code>LD_LIBRARY_PATH</code>. Another possible problem is that gcc doesn't know to use libxml, which will result in "undefined symbols... \_xmlFree \_xmlFreeDoc" etc. Try adding <code>-lxml</code> or <code>-lxml2</code> to the compile command.

For more complex programs CMake is a good choice. It lets you write detailed build scripts that configure files, create directories, find system libraries, etc.

Updating the documentation
---------------------------

To update this file, just edit README.md.
[This description of markdown syntax](http://daringfireball.net/projects/markdown/) is helpful.
To update the full Doxygen documentation, see [here](http://synbiodex.github.com/libSBOLc#doxygen).
