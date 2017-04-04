#include <stdio.h>
#include <stdlib.h>
#include "qh_vector.h"

void test_vector()
{
	using qh::vector;

	bool all_test_ok = true;

	vector<int> test;
	test.push_back(1);
	assert(test[0] == 1);
	vector<int> test_v(10, 1);
	assert(test_v.size() == 10);

	for (int i = 0; i < 10; i++) {
		assert(test_v[i] == 1);
	}

	test_v.resize(5);
	assert(test_v.size() == 5);

	test_v.push_back(6);
	assert(test_v.size() == 6);
	assert(test_v[5] == 6);

	test_v.pop_back();
	assert(test_v.size() == 5);

	test_v.clear();
	assert(test_v.size() == 0);
	assert(test_v.empty());

	if (all_test_ok)
	{
		printf("%s All test OK!\n", __FUNCTION__);
	}
	else
	{
		printf("%s test failed!\n", __FUNCTION__);
	}
}

int main(int argc, char* argv[])
{
	test_vector();

#ifdef WIN32
	system("pause");
#endif
	return 0;
}

