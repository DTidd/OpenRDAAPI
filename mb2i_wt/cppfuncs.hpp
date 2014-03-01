#include <iostream>

class foo {

	public:
		int i;
		foo() {i=0; }
		~foo() {}
		void increment() { ++i; }
		void see() { std::cout << "--------------I am inside class ------------" << std::endl << "My integer is " << i << std::endl; }

};

