#include "WindowInterfaceForNeuralNet.h"
#include "GraphicsContextImage.h"

WindowInterfaceForNeuralNet::WindowInterfaceForNeuralNet(
	Brush* brushBackground, 
	Brush* brushIdentity, 
	Brush* brushHot, 
	Brush* brushSelected,
	HFONT font)
{
	this->brushBackground = brushBackground;
	this->brushIdentity = brushIdentity;
	this->brushHot = brushHot;
	this->brushSelected = brushSelected;

	//������� ����
	windowMain = new Panel(L"Sample window", brushBackground->GetBrush(), NULL, NULL, WS_OVERLAPPEDWINDOW);
	windowMain->Create(CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800);

	//������ � ������������
	context = new Panel(L"Context", brushIdentity->GetBrush(), windowMain);
	context->Create(8, 8, 1200 - 30, 600);
	context->Show(1);

	//������ � �������
	textPanel = new Panel(L"Context1", brushIdentity->GetBrush(), windowMain);
	textPanel->Create(115, context->sizeY + 15, 400, 140);
	textPanel->Show(1);

	lable = new Lable(L"����� ����� ������������ ��������� ������ ����...", RGB(255, 255, 255), font, textPanel);
	lable->Create(10, 5, textPanel->sizeX - 10, textPanel->sizeY - 10);
	lable->Show(1);

	//���� ������ �����
	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&openDialog);

	COMDLG_FILTERSPEC rgSpec[] =
	{
		{L"jpg, png", L"*.jpg;*.jpeg;*.png"}
	};
	openDialog->SetFileTypes(1, rgSpec);

	//������ ������ �����
	button = new ButtonChooserImage(L"�������� ����", brushIdentity->GetBrush(), brushHot->GetBrush(), brushSelected->GetBrush(), windowMain, openDialog);
	button->SetFont(font, RGB(255, 255, 255));
	button->Create(8, 620, 100, 20);
	button->Show(1);
}

WindowInterfaceForNeuralNet::~WindowInterfaceForNeuralNet()
{
	//������
	delete windowMain;
	delete context;
	delete textPanel;

	//������
	delete lable;

	//������
	delete button;
}

void WindowInterfaceForNeuralNet::SetGraphics(GraphicsContextImage* graphicsContext)
{
	this->graphicsContext = graphicsContext;
	button->SetGraphics(graphicsContext->graphics);
}

void WindowInterfaceForNeuralNet::ShowWindowInterface(int nCmdShow)
{
	windowMain->Show(nCmdShow);
}
