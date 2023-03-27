#include <iostream>
#include <conio.h>
int main()
{
	while(1)
	{
		std::cout<<"*";
		if(kbhit())
		{
			if(getch()==27)
				break;
		}
	}
	return 0;
}
