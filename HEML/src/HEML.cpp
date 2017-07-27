#include <Params.h>
#include <PubKey.h>
#include <Scheme.h>
#include <SchemeAlgo.h>
#include <SchemeAux.h>
#include <SecKey.h>
#include <TimeUtils.h>
#include <TestScheme.h>

#include "TestAK.h"
#include "TestJH.h"
#include "TestKW.h"

void test() {

}

int main() {

//	test();

	//logN, logl, logp, L

	//15: logp * L + l <= 620 (secure parameters)
	//16: logp * L + l <= 1241 (secure parameters)
	//17: logp * L + l <= 2483 (secure parameters)

	//Highest possible parameters for different number of itterations
	// each iteration is about (factorDim * N / 3) ms

	// 2 iterations: L = 16 (15,04,56,11)
	// 3 iterations: L = 16 (15,12,38,16)

	// 4 iterations: L = 21 (16,02,59,21)
	// 5 iterations: L = 26 (16,19,47,26)
	// 6 iterations: L = 31 (16,01,40,31)
	// 7 iterations: L = 36 (16,17,34,36)
	// 8 iterations: L = 41 (16,11,30,41)

	// 9 iterations: L = 46 (17,45,53,46)
	//10 iterations: L = 51 (17,35,48,51)
	//11 iterations: L = 56 (17,19,44,56)
	//12 iterations: L = 61 (17,43,40,61)
	//13 iterations: L = 66 (17,41,37,66)
	//14 iterations: L = 71 (17,69,34,71)
	//15 iterations: L = 76 (17,51,32,76)
	//16 iterations: L = 76 (17,53,30,81)

	TestAK::testAK(16,10,35,21);
//	TestCH::testJH(16, 60, 30, 40);
//	TestKW::testKW(16, 60, 30, 40);

//	TestScheme::testExponentBatch(13, 5, 30, 5, 7, 12);

	//-----------------------------------------

	return 0;
}
