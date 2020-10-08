#include "simplex.h"
#include <vector>

using namespace std;
using namespace Eigen;

void wiki_test() {
	Simplex simplex;
	MatrixType a;
	a.resize(2, 3);
	a << 3, 2, 1, 2, 5, 3;
	std::cout << a << std::endl;

	MatrixType b;
	b.resize(2, 1);
	b << 10, 15;
	MatrixType c;
	c.resize(1, 3);
	c << -2, -3, -4;

	simplex.AddObject(c, Simplex::ObjectType::Minimize);
	simplex.Init(a, b);
	simplex.Solve();
}

void example() {
	Simplex simplex;
	MatrixType a;
	a.resize(3, 2);
	a << 0.25, 1, 1.25, 0.5, 1, 1;
	std::cout << a << std::endl;

	MatrixType b;
	b.resize(3, 1);
	b << 65, 90, 85;
	MatrixType c;
	c.resize(1, 2);
	c << 15, 10;

	simplex.AddObject(c, Simplex::ObjectType::Maximize);
	simplex.Init(a, b);
	simplex.Solve();
}

void negtive_example() {
	Simplex simplex;
	MatrixType a;
	a.resize(3, 3);
	a << 0, -1, 2, 1, 0, 1, 2, -3, 7;
	std::cout << a << std::endl;

	MatrixType b;
	b.resize(3, 1);
	b << 1, 1, 5;
	MatrixType c;
	c.resize(1, 3);
	c << 3, -2, 5;

	simplex.AddObject(c, Simplex::ObjectType::Minimize);
	simplex.Init(-a, -b);
	simplex.Solve();
}

void cycling_test() {
	Simplex simplex;
	MatrixType a;
	a.resize(3, 4);
	a << 1, -2, -1, 2, 2, -3, -1, 1, 0, 0, 1, 0;
	std::cout << a << std::endl;

	MatrixType b;
	b.resize(3, 1);
	b << 0, 0, 1;
	MatrixType c;
	c.resize(1, 4);
	c << 3, -5, 1, -2;

	simplex.AddObject(c, Simplex::ObjectType::Maximize);
	simplex.Init(a, b);
	simplex.Solve();
}

int main()
{
	cout << "Hello CMake." << endl;
	example();
	wiki_test();
	negtive_example();
	cycling_test();
	return 0;
}
