//#include <windows.h>

//#include <d3d11.h>
//#include <d3dcompiler.h>
//#include <DirectXMath.h>
//#include <chrono>

//#include "bth_image.h"
#include "Window.h"

/*using namespace DirectX;

// a resource to store Vertices in the GPU
ID3D11Buffer* gVertexBuffer = nullptr;

ID3D11InputLayout* gVertexLayout = nullptr;

// resources that represent shaders
ID3D11VertexShader* gVertexShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;

// World Matrix
ID3D11Buffer* gWorldMatrix = nullptr;
XMMATRIX gWorld;

// View Matrix
ID3D11Buffer* gViewMatrix = nullptr;
XMMATRIX gView;

// Proj Matrix
ID3D11Buffer* gProjMatrix = nullptr;
XMMATRIX gProj;

ID3D11Buffer* gMatrixs = nullptr;
struct stBuffer {
	XMMATRIX gWorld;
	XMMATRIX gView;
	XMMATRIX gProj;
};
stBuffer globalValues;

ID3D11Texture2D* gTexture = nullptr;
ID3D11ShaderResourceView* gTextureView = nullptr;
ID3D11SamplerState* gSampler = nullptr;

ID3D11Buffer* gLight = nullptr;
struct lightBuffer {
	XMVECTOR pos;
	XMVECTOR color;
};
lightBuffer light;

float rad = 0.0f;

void CreateConstantBuffers()
{
	// initialize the description of the buffer for all three matrices.
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(globalValues);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// check if the creation failed for any reason
	HRESULT hr = 0;
	hr = gDevice->CreateBuffer(&bufferDesc, nullptr, &gMatrixs);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}

	// Make the constatnt buffer for the light.
	D3D11_BUFFER_DESC lightDesc;
	memset(&lightDesc, 0, sizeof(lightDesc));
	lightDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightDesc.ByteWidth = sizeof(light);
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightDesc.StructureByteStride = 0;

	hr = gDevice->CreateBuffer(&lightDesc, nullptr, &gLight);
	if (FAILED(hr))
	{
		exit(-1);
	}
}

HRESULT createTexture()
{
	// Create texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = BTH_IMAGE_WIDTH;
	textureDesc.Height = BTH_IMAGE_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = BTH_IMAGE_DATA;
	textureData.SysMemPitch = BTH_IMAGE_WIDTH * 4;
	textureData.SysMemSlicePitch = 0;

	HRESULT hr = gDevice->CreateTexture2D(&textureDesc, &textureData, &gTexture);
	if (FAILED(hr))
	{
		OutputDebugStringA("hr1\n");
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
	textureViewDesc.Format = textureDesc.Format;
	textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	textureViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	textureViewDesc.Texture2D.MostDetailedMip = 0;

	hr = gDevice->CreateShaderResourceView(gTexture, &textureViewDesc, &gTextureView);
	if (FAILED(hr))
	{
		OutputDebugStringA("hr2\n");
		return hr;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	hr = gDevice->CreateSamplerState(&samplerDesc, &gSampler);
	if (FAILED(hr))
	{
		OutputDebugStringA("hr3\n");
		return hr;
	}
	return -1;
}

HRESULT CreateShaders()
{
	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	HRESULT result = D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gVertexShader
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"TEXCOORD",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);
	if (FAILED(result))
	{
		OutputDebugStringA("Error when creating vertexLayout\n");
		return result;
	}

	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	// Create geometry shader
	ID3DBlob* pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"Geometry.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShader);
	pGS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"Fragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	return S_OK;
}

void CreateTriangleData()
{
	struct TriangleVertex
	{
		float x, y, z;
		float u, v;
	};

	// Array of Structs (AoS)
	TriangleVertex triangleVertices[6] =
	{
		-0.5f, 0.5f, 0.0f,	//v0 pos
		0.0f, 0.0f,	        //v0 color

		0.5f, 0.5f, 0.0f,	//v1
		1.0f, 0.0f,         //v1 color

		0.5f, -0.5f, 0.0f,  //v2
		1.0f, 1.0f,			//v2 color

		-0.5f, 0.5f, 0.0f,	//tri2 v0 pos
		0.0f, 0.0f,			//tri2 v0 color

		0.5f, -0.5f, 0.0f,	//tri2 v1
		1.0f, 1.0f,			//tri2 v1 color

		-0.5f, -0.5f, 0.0f, //tri2 v2
		0.0f, 1.0f,			//tri2 v2 color
	};

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;

	// create a Vertex Buffer
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
}

void changeWorldViewProj(float dt)
{
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	gDeviceContext->Map(gMatrixs, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	// World matrix
	//rad += 0.05f;
	rad += (XM_PI / 4)* dt;
	if (rad > XM_PI * 2) rad = 0.0f;
	globalValues.gWorld = XMMatrixTranspose(XMMatrixRotationY(rad));

	// View matrix
	XMFLOAT3 eyePos = XMFLOAT3(0.0f, 0.0f, -2.0f);
	XMFLOAT3 focusPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 upDir = XMFLOAT3(0.0f, 1.0f, 0.0f);
	globalValues.gView = XMMatrixTranspose(XMMatrixLookAtLH(XMLoadFloat3(&eyePos), XMLoadFloat3(&focusPos), XMLoadFloat3(&upDir)));

	// Proj matrix
	globalValues.gProj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PI * 0.45f, 640.0f / 480.0f, 0.1f, 20.f));

	// Copy the data to the buffer and unmap dataPtr
	memcpy(dataPtr.pData, &globalValues, sizeof(globalValues));
	gDeviceContext->Unmap(gMatrixs, 0);

	gDeviceContext->GSSetConstantBuffers(0, 1, &gMatrixs);
}

void setLight()
{
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	gDeviceContext->Map(gLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	XMFLOAT3 lp = XMFLOAT3(0.0f, 0.0f, -2.0f);
	XMFLOAT4 lc = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light.pos = XMLoadFloat3(&lp);
	light.color = XMLoadFloat4(&lc);

	memcpy(dataPtr.pData, &light, sizeof(light));
	gDeviceContext->Unmap(gLight, 0);

	gDeviceContext->PSSetConstantBuffers(0, 1, &gLight);
}

void Render(float dt)
{
	// clear the back buffer to a deep blue
	float clearColor[] = { 0, 0, 0, 1 };

	// use DeviceContext to talk to the API
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);

	gDeviceContext->ClearDepthStencilView(gDepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// specifying NULL or nullptr we are disabling that stage
	// in the pipeline
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	UINT32 vertexSize = sizeof(float) * 5;// sizeof(float) * 6
	UINT32 offset = 0;
	// specify which vertex buffer to use next.
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);

	gDeviceContext->PSSetShaderResources(0, 1, &gTextureView);
	gDeviceContext->PSSetSamplers(0, 1, &gSampler);

	changeWorldViewProj(dt);

	setLight();

	// specify the topology to use when drawing
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// specify the IA Layout (how is data passed)
	gDeviceContext->IASetInputLayout(gVertexLayout);

	// issue a draw call of 3 vertices (similar to OpenGL)
	gDeviceContext->Draw(6, 0);
}*/

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{

	Window window;
	if (window.initialize()) //1. Skapa fönster
	{
		window.run();
	}
	window.destroyWindow();

	/*if (window.getWindow())
	{
		CreateDirect3DContext(); //2. Skapa och koppla SwapChain, Device och Device Context

		SetViewport(); //3. Sätt viewport

		CreateShaders(); //4. Skapa vertex- och pixel-shaders

		CreateTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		CreateConstantBuffers();

		createTexture();

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

				Render(dt); //8. Rendera


				gSwapChain->Present(0, 0); //9. Växla front- och back-buffer
			}
		}

		gVertexBuffer->Release();

		gTexture->Release();
		gTextureView->Release();
		gSampler->Release();

		gVertexLayout->Release();
		gVertexShader->Release();
		gGeometryShader->Release();
		gPixelShader->Release();

		gBackbufferRTV->Release();
		gDepthBuffer->Release();
		gSwapChain->Release();
		gDevice->Release();
		gDeviceContext->Release();

		window.destroyWindow();
	}*/

	return 0;
}