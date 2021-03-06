#include "Shader.h"

bool Shader::initializeShader(ID3D11Device * device, HWND window, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	
	// Compile the vertex shader code.
	result = D3DCompileFromFile(
		vsFilename, 
		nullptr, 
		nullptr,
		"VS_main", 
		"vs_5_0", 
		D3D10_SHADER_ENABLE_STRICTNESS, 
		0,
		&vertexShaderBuffer, 
		&errorMessage
	);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, window, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(window, (LPCSTR)vsFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(
		psFilename, 
		NULL, 
		NULL, 
		"PS_main", 
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 
		0,
		&pixelShaderBuffer, 
		&errorMessage
	);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, window, psFilename);
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(window, (LPCSTR)psFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}
	
	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &this->vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->pixelShader);
	if (FAILED(result))
	{
		return false;
	}
	
	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	
	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &this->layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
	
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &this->matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &this->sampleState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Shader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND window, WCHAR* shaderFilename)
{
	// Get a pointer to the error message text buffer.
	char* compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	unsigned long bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	ofstream fout;
	fout.open("shader-error.txt");

	// Write out the error message.
	for (unsigned long i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = nullptr;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(window, "Error compiling shader.  Check shader-error.txt for message.", (LPCSTR)shaderFilename, MB_OK);
}

bool Shader::setShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX &worldMatrix, DirectX::XMMATRIX &viewMatrix, DirectX::XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(this->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = DirectX::XMMatrixTranspose(worldMatrix);
	dataPtr->view = DirectX::XMMatrixTranspose(viewMatrix);
	dataPtr->projection = DirectX::XMMatrixTranspose(projectionMatrix);

	// Unlock the constant buffer.
	deviceContext->Unmap(this->matrixBuffer, 0);
	
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &this->matrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void Shader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(this->layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(this->vertexShader, nullptr, 0);
	deviceContext->PSSetShader(this->pixelShader, nullptr, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &this->sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

Shader::Shader()
{
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->layout = nullptr;
	this->matrixBuffer = nullptr;
	this->sampleState = nullptr;
}

Shader::Shader(const Shader &)
{
}

Shader::~Shader()
{
}

bool Shader::initialize(ID3D11Device* device, HWND window)
{
	if (!initializeShader(device, window, L"VertexShader.hlsl", L"PixelShader.hlsl"))
	{
		return false;
	}

	return true;
}

void Shader::shutdown()
{
	if (this->sampleState)
	{
		this->sampleState->Release();
		this->sampleState = nullptr;
	}

	if (this->vertexShader)
	{
		this->vertexShader->Release();
		this->vertexShader = nullptr;
	}

	if (this->pixelShader)
	{
		this->pixelShader->Release();
		this->pixelShader = nullptr;
	}

	if (this->layout)
	{
		this->layout->Release();
		this->layout = nullptr;
	}

	if (this->matrixBuffer)
	{
		this->matrixBuffer->Release();
		this->matrixBuffer = nullptr;
	}
}

bool Shader::render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX &worldMatrix, DirectX::XMMATRIX &viewMatrix, DirectX::XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture)
{
	// Set the shader parameters that it will use for rendering.
	if (!setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture))
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	renderShader(deviceContext, indexCount);

	return true;
}
