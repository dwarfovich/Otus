#include <iostream>

#include "project_meta_data.h"
#include "lib.h"

using namespace std;

int main()
{
	cout << "Hello, world!\n";
	cout << "Project name: " << PROJECT_NAME << '\n';
	cout << "Version: " << version() << '\n';

	return 0;
}
