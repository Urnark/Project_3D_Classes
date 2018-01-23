#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "Input.h"
#include "Graphics.h"
#include <chrono>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class Window
{
private:
	int width = 640, height = 480;

	HWND window;
	LPCSTR applicationName;
	HINSTANCE hInstance;

	Input* input;
	Graphics* graphics;

	bool initWindow();
	bool frame(float deltaTime);
public:
	Window();
	Window(const Window&);
	~Window();

	bool initialize();
	void run();
	void destroyWindow();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);

	// Getters
	HWND getWindow() { return this->window; };
	int getWidth() { return this->width; };
	int getHeight() { return this->height; };
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Window* ApplicationHandle = 0;

#endif