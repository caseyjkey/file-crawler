#include <iostream>
#include <fstream>

#include <string.h>
#include <stdlib.h>

#include "Debug.h"
/** @mainpage C++ stream debugging
  */

/** @file: Debug.cpp
 * @brief A simple C++ class for debugging
 *
 * @details This class provides a mechanism for adding debugging output
 * to C++ code using a stream syntax. The user does not use the class directly.
 * Rather, the macros <tt>debug, lDebug(), vDebug()</tt> are used. These
 * expand into use of the class. The macros rely on the preprocessor to
 * provide meta information (file name, line nmber, function name); In order
 * for the macro to operate a <tt>-DDEBUG</tt> must be included when compiling
 * the code. Otherwise, all debug code may be optimized out.
 */

/** A global controlling the amount of debug output. It <b>may</b> be
 *  set in the <tt>Debug::init()</tt> method. A value <tt>0</tt> means
 *  no output. Higher values mean more output. Used in conjunction with
 *  <tt>lDebug()</tt>.
 */
int Debug::debugLevel = 0;

/** a <b>pointer</b> to the stream used for output. It defaults to
 *  <tt>std::cerr</tt>. Based on suggestion and code found
 *  <a href="https://stdcxx.apache.org/doc/stdlibug/34-2.html">here</a>.
 */
std::ostream* Debug::dbOS = 0;

/** The prefix of the debug flag (must be in <tt>argv[1]</tt> passed to the
 *  <tt>init()</tt> function. The flag may be optionally followed by a number
 *  (e.g. <tt>-debug5</tt> to sets the <tt>debugLevel</tt> to the value 5.
 *  If not specified, it is set to 1.
 */
static const char* prefix = "-debug";

/** Initialize the variable <tt>Debug::debugLevel</tt> depending on the value
 *  of <tt>argv[1]</tt>. Normally called from <tt>main</tt> with the program
 *  arguments. If <tt>argv[1]</tt> is or begins with <tt>-debug</tt>, the
 *  value of <tt>Debug::debugLevel</tt> is set and <tt>argc, argv</tt> are
 *  modified appropriately. An entry of <tt>-debug5</tt> sets the level to 5.
 *  If the function is not called, the user is responsible for setting
 *  <tt>Debug::debugLevel</tt> in other code.
 *  @param argc the number of command line arguments
 *  @param argv the array of command line arguments.
 */
void Debug::init(int& argc, const char*argv[]) {
  Debug::dbOS = &std::cerr;

  if (argc > 1) {
    const char* arg1 = argv[1];
    size_t len = strlen(prefix);
 
    if (strncmp(arg1, prefix, len) == 0) {
      Debug::debugLevel = 1;

      if (strlen(arg1) > len)
        Debug::debugLevel = atoi(arg1 + len);

      argc--; // decrement number of arguments

      for (int i = 1; i < argc; i++) // remove first argument
        argv[i] = argv[i+1];
    }
  }
}

/** Send debugging output to a file.
 *  @param fileName name of file to send output to
 */
void Debug::toFile (const char* fileName) {
  Debug::close();
  Debug::dbOS = new std::ofstream(fileName);
}

/** Close the output file if it was set in <tt>Debug::toFile()</tt> */
void Debug::close(void) {
  if (Debug::dbOS != &std::cerr) {
    delete Debug::dbOS;
    Debug::dbOS = &std::cerr;
  }
}

/** Constructor for the <tt>Debug</tt> class.
 *  It sends the file name, line number and function name to the debug stream.
 *  This is based on code by Alberto Lepe found
 *  <a href="http://stackoverflow.com/questions/5028302/small-logger-class">
 *  here</a>. An anonymous instance of the class is created and used for the
 *  debugging output and the destructor adds the <tt>endl</tt>.
 *  @param file name of file (supplied by <tt>__FILE__</tt>)
 *  @param line line number in file (supplied by <tt>__LINE__</tt>)
 *  @param func name of method (supplied by <tt>__func__</tt>)
 */
Debug::Debug (const char* file, int line, const char* func) {
  if (! Debug::dbOS) // guarantee initialized, removes need to call init()
    Debug::dbOS = &std::cerr;

  (*Debug::dbOS) << "DEBUG " << file << "[" << line << "] " << func << "() ";
}

/** The destructor automatically sends an <tt>endl</tt> to the output stream
 *  The constructor created an anonymous instance which is destructed at the end
 *  of the debug statement.
 */
Debug::~Debug() {
  (*Debug::dbOS) << std::endl;
}

