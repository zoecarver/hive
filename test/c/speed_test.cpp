#include <iostream>
#include <vector>

int isone(int x) {
	return x - 1;
}

int main() {
	int arg[] = {1, 0, 1, 2, 6, 0};
	std::vector<int> arr(arg, arg + 6);

	arr.erase(std::remove_if(arr.begin(), arr.end(), isone), arr.end());

	return 0;
}