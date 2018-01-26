#ifndef INPUTCLASS_H
#define INPUTCLASS_H

/*class Input
{
private:
	bool m_keys[256];

public:
	Input();
	Input(const Input&);
	~Input();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

};*/

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class Input
{
private:
	IDirectInput8* directInput;
	IDirectInputDevice8* keyboard;
	IDirectInputDevice8* mouse;

	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState;

	int screenWidth, screenHeight;
	int mouseX, mouseY;
	HWND window;
	
	bool readKeyboard();
	bool readMouse();
	void processInput();
public:
	Input();
	Input(const Input&);
	~Input();

	bool initialize(HINSTANCE hinstance, HWND window, int screenWidth, int screenHeight);
	void shutdown();
	bool frame();

	bool isEscapePressed();
	void getMouseLocation(int &mouseX, int &mouseY);
	bool isKeyPressed(int key);
	bool isMousePressed();
	void setMouseLocation(int mouseX, int mouseY);
};

#endif