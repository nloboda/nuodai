/*
 * What the fuck is this? Oh my holy fucking shit cock!
 * That's a goddamn test suite! Just look at all those test cases!
 * Every fucking each of them is soo damn good your mom would suck my dick
 * just to run it.
 *
 *
 *  Created on: 03 Nov,2019
 *      Author: user
 */

#include <cppunit/ui/text/TextTestRunner.h>
#include "FsBlockTest.cpp"
#include "BlockMapperTest.cpp"
#include "PlainFsTest.cpp"
#include "CryptoLayerTest.cpp"
#include "BlockSemaphoreTest.cpp"

int main(int argc, char* argv[])
{
	CppUnit::TextTestRunner runner;
	runner.addTest(new FsBlockTest());
	runner.addTest(new BlockMapperTest());
	runner.addTest(new PlainFsTest());
	runner.addTest(new CryptoLayerTest());
	runner.addTest(new BlockSemaphoreTest());
	runner.run();
	return 0;
}
