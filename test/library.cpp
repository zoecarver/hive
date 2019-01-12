#include <iostream>

#ifdef _WIN32
#	define DLLEXPORT __declspec(dllexport)
#else
#	define DLLEXPORT
#endif

using namespace std;

bool IsOdd(int i) {
	return ((i % 2) == 1);
}

typedef int (*functiontype)(int);
extern "C" DLLEXPORT int* filter(int* arr, int size, functiontype sort_func) {
	//  cout << "size of int " << sizeof(int) << " size of arr " << sizeof(arr) << "\n";

	int* pbegin = arr;
	int* pend = arr + size;

	//  std::cout << "[before] the range contains:";
	//  for (int* p=pbegin; p!=pend; ++p)
	//    std::cout << ' ' << *p;
	//  std::cout << '\n';

	pend = remove_if(pbegin, pend, sort_func);

	//  std::cout << "[after] the range contains:";
	//  for (int* p=pbegin; p!=pend; ++p)
	//    std::cout << ' ' << *p;
	//  std::cout << '\n';

	return pbegin;
}

/// print - printf that takes an int prints it as "%i\n", returning 0.
extern "C" DLLEXPORT int print(int X) {
	fprintf(stderr, "%i\n", X);
	return 0;
}

/// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT int printd(double X) {
	fprintf(stderr, "%f\n", X);
	return 0;
}
