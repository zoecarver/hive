#include <iostream>
 
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

using namespace std;

/// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT int print(int X) {
  fprintf(stderr, "%i\n", X);
  return 0;
}

