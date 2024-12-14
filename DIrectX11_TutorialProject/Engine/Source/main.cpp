
#include "System/Systemclass.h"

// 참고자료
// https://ppparkje.tistory.com/ [빠재의 노트:티스토리]

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdShow)
{
	USystemClass* System;
	bool result;

	System = new USystemClass();
	if (!System)
	{
		return 0;
	}

	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	System->Shutdown();
	
	delete System;
	System = nullptr;

	return 0;
}