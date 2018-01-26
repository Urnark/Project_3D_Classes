#include "Input.h"


Input::Input()
{
	this->directInput = nullptr;
	this->keyboard = nullptr;
	this->mouse = nullptr;
}


Input::Input(const Input& other)
{
}


Input::~Input()
{
}


bool Input::initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;
	this->window = hwnd;

	// Store the screen size which will be used for positioning the mouse cursor.
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	this->mouseX = 0;
	this->mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = this->directInput->CreateDevice(GUID_SysKeyboard, &this->keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = this->keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = this->keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = this->keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}
	
	// Initialize the direct input interface for the mouse.
	result = this->directInput->CreateDevice(GUID_SysMouse, &this->mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = this->mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = this->mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = this->mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Input::shutdown()
{
	// Release the mouse.
	if (this->mouse)
	{
		this->mouse->Unacquire();
		this->mouse->Release();
		this->mouse = 0;
	}

	// Release the keyboard.
	if (this->keyboard)
	{
		this->keyboard->Unacquire();
		this->keyboard->Release();
		this->keyboard = 0;
	}

	// Release the main interface to direct input.
	if (this->directInput)
	{
		this->directInput->Release();
		this->directInput = 0;
	}

	return;
}
bool Input::frame()
{
	// Read the current state of the keyboard.
	if (!readKeyboard())
	{
		return false;
	}

	// Read the current state of the mouse.
	if (!readMouse())
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	processInput();

	return true;
}

bool Input::readKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = this->keyboard->GetDeviceState(sizeof(this->keyboardState), (LPVOID)&this->keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			this->keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::readMouse()
{
	HRESULT result;

	// Read the mouse device.
	result = this->mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&this->mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			this->mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Input::processInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	this->mouseX += this->mouseState.lX;
	this->mouseY += this->mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (this->mouseX < 0) this->mouseX = 0;
	if (this->mouseY < 0) this->mouseY = 0;

	if (this->mouseX > this->screenWidth) this->mouseX = this->screenWidth;
	if (this->mouseY > this->screenHeight) this->mouseY = this->screenHeight;

	return;
}

bool Input::isEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (this->keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

void Input::getMouseLocation(int &mouseX, int &mouseY)
{
	mouseX = this->mouseX;
	mouseY = this->mouseY;
	return;
}

bool Input::isKeyPressed(int key)
{
	if (this->keyboardState[key] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::isMousePressed()
{
	if (this->mouseState.rgbButtons[0])
	{
		return true;
	}

	return false;
}

void Input::setMouseLocation(int mouseX, int mouseY)
{
	RECT rect = { NULL };
	GetWindowRect(this->window, &rect);
	SetCursorPos(rect.left + mouseX, rect.top + mouseY);
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}
