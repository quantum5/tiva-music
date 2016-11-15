#include "orbit/OrbitOled.h"
#include "orbit/OrbitOledChar.h"

void menu_test(void) {
	OrbitOledInit();
	OrbitOledPutString("Hello, World!");
	OrbitOledUpdate();
}
