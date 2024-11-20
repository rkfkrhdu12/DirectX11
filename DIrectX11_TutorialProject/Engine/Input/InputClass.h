#pragma once

#ifndef  _INPUTCLASS_H_
#define _INPUTCLASS_H_

class UInputClass
{
public:
	UInputClass() = default;
	UInputClass(const UInputClass&);
	~UInputClass() = default;

	void Initialize();
	void Shutdown();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool _keys[256];
};

#endif // ! _INPUTCLASS_H_
