
#include "System/Systemclass.h"

// �����ڷ�
// https://ppparkje.tistory.com/ [������ ��Ʈ:Ƽ���丮]

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