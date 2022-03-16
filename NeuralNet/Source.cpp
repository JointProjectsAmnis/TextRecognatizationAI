#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//Window application
#include "Window application/WindowInterfaceForNeuralNet.h"
#include "Window application/GraphicsContextImage.h"
#include "Window application/Content.h"

//Compute shader
#include "WindowsClasses/DirectX3D/Shaders/ShaderCompute.h"

void main()
{
	setlocale(LC_ALL, "ru");
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	Content::Load();

	WindowInterfaceForNeuralNet* windowInterface = new WindowInterfaceForNeuralNet(Content::brushBackground, Content::brushIdentity, Content::brushHot, Content::brushSelected, *Content::font);
	windowInterface->ShowWindowInterface(1);

	GraphicsContextImage* graphicsContextImage = new GraphicsContextImage(windowInterface);
	windowInterface->SetGraphics(graphicsContextImage);

	//ShaderCompute shader = ShaderCompute(graphicsContextImage->graphics, L"WindowsClasses\\DirectX3D\\Shaders\\ShaderComputeTest\\ShaderComputeTest.hlsl");

	MSG msg = {};
	while (!windowInterface->windowMain->isClosed)
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		graphicsContextImage->Draw();
	}
}