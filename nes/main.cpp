#include "cpu6502.h"
#include "MemorySimulator.h"
#include <iostream>

int main()
{
	MemorySimulator m;
	cpu6502 c;
	c.SetMemorySimulator(&m);
	//c.display();

	//std::string inp;
	//while (1) {
	//	std::cin >> inp;
	//	if (inp.compare("s") == 0) {
	//		c.readPC();
	//		std::cout << std::endl;
	//		c.display();
	//	}
	//	else
	//		break;		
	//}
	int i = 0;
	for (uint16_t curr = 0x0000; curr < 0x4000; curr++) {
		c.readPC();
		//std::cout << i << std::endl;
		//i++;
	}
		
	return 0;
}