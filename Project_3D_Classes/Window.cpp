#include "Window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY)
		PostQuitMessage(0);
	if (message == WM_CLOSE)
		PostQuitMessage(0);

	return ApplicationHandle->messageHandler(hWnd, message, wParam, lParam);
}

Window::Window()
{
	this->input = nullptr;
	this->graphics = nullptr;
}

Window::Window(const Window &)
{

}

Window::~Window()
{

}

bool Window::initWindow()
{
	WNDCLASSEX wcex = { 0 };

	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	hInstance = GetModuleHandle(NULL);

	// Give the application a name.
	applicationName = "Project_3D";

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = this->hInstance;
	wcex.lpszClassName = applicationName;
	if (!RegisterClassEx(&wcex))
		return false;

	// Place the window in the middle of the screen.
	int posX = (GetSystemMetrics(SM_CXSCREEN) - this->width) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - this->height) / 2;

	RECT rc = { 0, 0, this->width, this->height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	this->window = CreateWindow(
		applicationName,
		applicationName,
		WS_OVERLAPPEDWINDOW,
		posX,
		posY,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(this->window, SW_SHOW);
	SetForegroundWindow(this->window);
	SetFocus(this->window);

	return true;
}

bool Window::frame(float deltaTime)
{
	// Do the input frame processing.
	if (!this->input->frame())
	{
		return false;
	}

	if (!this->graphics->frame(deltaTime, this->input, this->width, this->height))
	{
		return false;
	}

	return true;
}

bool Window::initialize()
{
	this->initWindow();

	this->input = new Input();
	if (!this->input)
	{
		return false;
	}

	// Initialize the input object.
	this->input->initialize(this->hInstance, this->window, this->width, this->height);

	this->graphics = new Graphics();
	if (!this->graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	if (!this->graphics->initialize(this->width, this->height, this->window))
	{
		return false;
	}

	this->input->setMouseLocation(this->width / 2, this->height / 2);

	return true;
}

void Window::run()
{
	MSG msg;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	auto currentT = std::chrono::high_resolution_clock::now();
	auto preT = std::chrono::high_resolution_clock::now();
	float dt = 0.0f;

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			currentT = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float>(currentT - preT).count();
			preT = currentT;
			
			// If the window is in focus then update and render
			if (GetFocus() == this->window)
			{
				if (!this->frame(dt))
				{
					msg.message = WM_QUIT;
				} 
			}
		}

		// Check if the user pressed escape and wants to quit.
		if (this->input->isEscapePressed())
		{
			msg.message = WM_QUIT;
		}
	}
}

LRESULT CALLBACK Window::messageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::destroyWindow()
{
	if (this->graphics)
	{
		this->graphics->shutdown();
		delete this->graphics;
		this->graphics = nullptr;
	}

	if (this->input)
	{
		this->input->shutdown();
		delete this->input;
		this->input = nullptr;
	}

	// Remove the window.
	DestroyWindow(this->window);
	this->window = nullptr;

	// Remove the application instance.
	UnregisterClass(applicationName, hInstance);
	hInstance = nullptr;

	// Release the pointer to this class.
	ApplicationHandle = nullptr;
}
