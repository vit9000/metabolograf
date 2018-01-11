#include "stdafx.h"
#include "resource.h"
#include "PlaygroundView.h"


PlaygroundView::PlaygroundView()
{
}


PlaygroundView::~PlaygroundView()
{
	
	DestroyWindow();
	
}



BEGIN_MESSAGE_MAP(PlaygroundView, CWnd)
	
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	
	ON_LBN_DBLCLK(IDC_VARIABLELIST, &PlaygroundView::OnLbnDblclkVariableslist)
	
	ON_EN_CHANGE(IDC_RICHEDITCTRL, &PlaygroundView::OnEnChangeEditor)
	
	ON_LBN_SELCHANGE(IDC_OUTPUTLIST, &PlaygroundView::OnLbnSelchangeOutputlist)
	ON_LBN_DBLCLK(IDC_OUTPUTLIST, &PlaygroundView::OnLbnDblclkOutputlist)
	
	
END_MESSAGE_MAP()

void PlaygroundView::OnPaint()
{
	CWnd::OnPaint();


	/*CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;
	double dpiX = getDPIX();

	CDC* cdc = GetDC();
	UGC ugc(cdc->m_hDC, 100, 100);
	ugc.SetDrawColor(255, 0, 0);
	ugc.FillRectangle(0, 0, rect.Width(), rect.Height());

	ReleaseDC(cdc);*/
}
int PlaygroundView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CWnd::OnCreate(lpCreateStruct);


	CFont* pFont = new CFont;
	pFont->CreateFont(
		14 * (double)DPIX(),                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH,  // nPitchAndFamily
		"Arial");                 // lpszFacename

	
	VariablesList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY| WS_VSCROLL, CRect(0,0,0,0), this, IDC_VARIABLELIST);
	VariablesList.SetFont(pFont);
	OutputList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY| WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_OUTPUTLIST);
	OutputList.SetFont(pFont);
	canvas.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOINTEGRALHEIGHT, CRect(0, 0, 0, 0), this, IDC_PLAYGROUNDCANVAS);

	Editor.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_RICHEDITCTRL);
	Editor.SetEventMask(Editor.GetEventMask() |
		ENM_CHANGE);
	Editor.ReplaceSel("{\n\n}");
	CFont m_font;
	m_font.CreatePointFont(105, "Times New Roman", NULL);
	Editor.SetFont(&m_font, false);
	LoadWorkScript();
	highlighter = Highlighter(&Editor);
	return 0;
}



void PlaygroundView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;
	double dpiX = DPIX();

	int tempX = 200 * dpiX;
	::SetWindowPos(GetDlgItem(IDC_VARIABLELIST)->m_hWnd, HWND_TOP,
		0,
		0,
		tempX,
		rect.bottom,
		NULL);
	
	int tempY = 200 * dpiX;
	::SetWindowPos(GetDlgItem(IDC_OUTPUTLIST)->m_hWnd, HWND_TOP,
		tempX,
		rect.Height() - tempY,
		rect.Width() - tempX - tempY,
		tempY,
		NULL);

	::SetWindowPos(GetDlgItem(IDC_RICHEDITCTRL)->m_hWnd, HWND_TOP,
		tempX,
		0,
		rect.Width() - tempX,
		rect.Height()-tempY,
		NULL);

	::SetWindowPos(GetDlgItem(IDC_PLAYGROUNDCANVAS)->m_hWnd, HWND_TOP,
		rect.Width() - tempY,
		rect.Height() - tempY,
		tempY,
		tempY,
		NULL);
}


void PlaygroundView::OnLButtonDblClk(UINT flags, CPoint point)
{
	CRect rc;
	canvas.GetWindowRect(&rc); //relative to the full screen 
	this->ScreenToClient(&rc);

	canvas.OnLButtonDblClk(this, point.x - rc.left, point.y - rc.top);
	//CDialogEx::OnLButtonDblClk(flags, point);
}


void PlaygroundView::UpdateVariablesList()
{
	VariablesList.ResetContent();
	for (const auto& it : database->getVariables())
	{
		/*string type = (it.second.GetType()) ? "" : "Scalar - ";
		type += it.first.c_str();
		VariablesList.AddString(type.c_str());*/
		VariablesList.AddString(it.first.c_str());
	}
}
//-----------------------------------------------------------------------------
void PlaygroundView::OnLbnDblclkVariableslist()
{
	char* text = new char[256];
	VariablesList.GetText(VariablesList.GetCurSel(), text);
	Editor.ReplaceSel(text, TRUE);
	delete[] text;
	Editor.SetFocus();
}
//-----------------------------------------------------------------------------
void PlaygroundView::OnRunScript(bool repeat=false)
{
	int len = Editor.GetTextLength() + 1;
	char * buf = new char[len + 1];
	Editor.GetWindowTextA(buf, len);
	string text = buf;
	delete[] buf;

	OutputList.ResetContent();
	if(!repeat)
		database->Clear();

	canvas.Clear();
	Analizator analizator(database, &dictionary, &canvas);

	size_t found = -1;
	while (1)
	{

		int i = 0;
		size_t start = text.find("{", found + 1);
		if (start == std::string::npos)
			break;
		size_t end = text.find("}", start + 1);
		if (end == std::string::npos)
			break;

		string code = text.substr(start + 1, end - start - 1);
		string res = BuildCode(code, analizator);
		text.replace(start, end - start + 1, res);
	}
	protocol = text;
	UpdateVariablesList();

	canvas.RedrawWindow();
}
//----------------------------------------------------------------------------------
string PlaygroundView::BuildCode(string& code, Analizator& analizator)
{
	string result = "";
	for (int i = 0; i<code.length(); i++)
	{
		if (code[i] == '\n' || code[i] == '\r') code[i] = ' ';
	}

	stringstream ss(code);
	string line;
	while (getline(ss, line, ';'))
	{

		string output = "";
		try
		{
			if (line.length()>0)
			{
				output = analizator.Run(line);
			}
		}
		catch (exception& ex)
		{
			string err = ex.what();
			output += "err: " + err;
		}
		if (!output.empty())
		{
			if (output.empty())
				OutputList.AddString(analizator.debug_string.c_str());
			else OutputList.AddString(output.c_str());
			result.append(output);


		}

		line.clear();
	}
	return result;

}
//----------------------------------------------------------------------------------------------------
void PlaygroundView::OnEnChangeEditor()
{
	highlighter.RestartTimer(this->m_hWnd);
}
//----------------------------------------------------------------------------------------------------
void PlaygroundView::OnTimer(UINT_PTR nIdEvent)
{
	highlighter.Run();
	highlighter.StopTimer(this->m_hWnd);
	CWnd::OnTimer(nIdEvent);
}
//----------------------------------------------------------------------------------------------------
void PlaygroundView::SaveScript(string filename)
{
	int len = Editor.GetTextLength() + 1;
	char * buf = new char[len + 1];
	Editor.GetWindowTextA(buf, len);

	fstream fs;
	fs.open(filename, fstream::out);
	if (fs)
	{
		fs << buf;

		fs.close();
	}
	delete[] buf;
}

void PlaygroundView::OnSaveScript()
{
	CFileDialog fileDialog(FALSE, "script.txt", NULL, NULL, "*.txt");
	string filename;
	if (fileDialog.DoModal() == IDOK)
	{
		//model->sendToFile(CSToString(FileDlg.GetPathName()));
		CString str = fileDialog.GetFolderPath() + "\\" + fileDialog.GetFileName();

		filename = str.GetBuffer(str.GetLength());
		SaveScript(filename);
	}
	


	
}
//----------------------------------------------------------------------------------------------------
void PlaygroundView::OnNewScript()
{
	CHARRANGE cr = { 0, Editor.GetTextLength() };
	Editor.SetSel(cr);
	Editor.ReplaceSel("{\n\n}");
}
//----------------------------------------------------------------------------------------------------
void PlaygroundView::LoadScript(string filename)
{
	fstream fs;
	fs.open(filename, fstream::in);
	if (!fs) return;
	string text((istreambuf_iterator<char>(fs)),
		istreambuf_iterator<char>());


	fs.close();

	//int l = Editor.GetTextLength();
	CHARRANGE cr = { 0, Editor.GetTextLength() };
	Editor.SetSel(cr);

	Editor.ReplaceSel(text.c_str());
}
void PlaygroundView::OnLoadScript()
{
	CFileDialog fileDialog(TRUE, "Файл скрипта", "*.txt");	//объект класса выбора файла
	int result = fileDialog.DoModal();	//запустить диалоговое окно
	string filename;
	if (result == IDOK)	//если файл выбран
	{
		//AfxMessageBox(fileDialog.GetPathName()); // показать полный путь
		CString str = fileDialog.GetFolderPath() + "\\" + fileDialog.GetFileName();

		filename = str.GetBuffer(str.GetLength());
		LoadScript(filename);
	}
	

	
}
//----------------------------------------------------------------------------------------------------
void PlaygroundView::SaveHTML(string& text)
{
	//делаем переводы строки
	size_t found = -1;
	while (1)
	{

		found = text.find("\n", found + 1);
		if (found == std::string::npos)
			break;
		text.replace(found, 1, "\n\r<br>");
	}
	// меняем 'График *' на <img src='График *.png'/>
	found = 0;
	while (1)
	{
		size_t start = text.find("'График", found);
		if (start == std::string::npos)
			break;
		size_t end = text.find("'", start + 1);
		if (end == std::string::npos)
			break;
		string temp = string("<img src = ") + text.substr(start, end - start) + string(".png'/>");

		text.replace(start, end - start + 1, temp);

		found = end + 1;
	}





	//пишем в файл
	string path = "c:\\temp_html\\";
	canvas.SaveImages(path);

	string fname = path;
	fname += "index.html";
	fstream fs;
	fs.open(fname, fstream::out);
	fs << database->GetHTMLHeader() << endl;
	fs << text << endl;
	fs << "</table>" << endl;
	fs.close();
	ShellExecute(0, NULL, fname.c_str(), NULL, NULL, SW_RESTORE);
}
//----------------------------------------------------------------------------------------------------
void PlaygroundView::SaveHTML()
{
	SaveHTML(protocol);
}
//----------------------------------------------------------------------------------------------------
void PlaygroundView::OnLbnSelchangeOutputlist()
{
	if (OutputList.GetCurSel() < 0) return;
	char* text = new char[256];
	OutputList.GetText(OutputList.GetCurSel(), text);
	string item = text;
	delete[] text;

	item.erase(item.length() - 1);
	item.erase(0, 1);
	canvas.SetPicture(item);
	canvas.RedrawWindow();
}
//----------------------------------------------------------------------------------------------------
void PlaygroundView::OnLbnDblclkOutputlist()
{
	canvas.EnlargePlot();
}



