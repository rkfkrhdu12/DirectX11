#include "InputClass.h"

UInputClass::UInputClass()
{
}

UInputClass::UInputClass(const UInputClass&)
{
}

UInputClass::~UInputClass()
{
}

void UInputClass::Initialize()
{
	for (int i = 0; i < 256; ++i)
		_keys[i] = false;
}

void UInputClass::Shutdown()
{
}

void UInputClass::KeyDown(unsigned int inputKey)
{
	_keys[inputKey] = true;
}

void UInputClass::KeyUp(unsigned int inputKey)
{
	_keys[inputKey] = false;
}

bool UInputClass::IsKeyDown(unsigned int inputKey)
{
	return _keys[inputKey];
}
