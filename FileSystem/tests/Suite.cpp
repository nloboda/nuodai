/*
 * Suite.cpp
 *
 *  Created on: 24 Dec,2019
 *      Author: user
 */




#include <cppunit/ui/text/TextTestRunner.h>
#include <DirectoryTest.cpp>

int main(int argc, char* argv[])
{
	CppUnit::TextTestRunner runner;
	runner.addTest(new DirectoryTest());
	runner.run();
	return 0;
}
