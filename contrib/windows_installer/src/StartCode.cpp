#include "header.h"
#include "FileCode.h"
#include "ShellCode.h"
#include <commctrl.h>


#define PLAY_NICELY
#include "BlueZip\BlueHead.h"

#include "resource.h"
#include "Parameters.h"

//Really no clue whats up with handles, needs more investigating (culprit - BlueZip!!)



#define ErrBox(Msg)				MessageBox(hDlg, Msg, ProgramName " Installer", MB_ICONERROR)
#define QuestBox(Msg, Flag)		MessageBox(hDlg, Msg, ProgramName " Installer", MB_ICONQUESTION | Flag)
#define InfoBox(Msg)			MessageBox(hDlg, Msg, ProgramName " Installer", MB_ICONINFORMATION)

// GLOBAL STATE
HINSTANCE hInst;
bool InDoEvents = false;
// END GLOBAL STATE


void InitDialog(HWND hDlg)
{
	InitCommonControls();

	char Buffer[MyMaxPath];
	GetProgramFiles(hDlg, Buffer);
	strcat(Buffer, "\\");
	strcat(Buffer, InstallDir);

	SetDlgItemText(hDlg, txtEdit, Buffer);
	CheckDlgButton(hDlg, chkExecute, BST_CHECKED);
	CheckDlgButton(hDlg, chkShortcut, BST_CHECKED);
}

void PaintDialog(HWND hDlg)
{
	const char* Msg = ProgramName " - " Description "\n© " Copyright;
	static int MsgLen = strlen(Msg);

	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hDlg, &ps);

	SelectObject(hDC, GetStockObject(DEFAULT_GUI_FONT));

	RECT rc = {0, 0, 463, 54};

	Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
	FillRect(hDC, &rc, (HBRUSH) GetStockObject(WHITE_BRUSH));
	rc.top = 13;
	DrawText(hDC, Msg, MsgLen, &rc, DT_WORDBREAK | DT_CENTER);

	EndPaint(hDlg, &ps);
}

void ErrDialog(HWND hDlg, char* Msg, char* Variable)
{
	char ErrBuffer[MyMaxPath * 2];
	strcpy(ErrBuffer, Msg);
	strcat(ErrBuffer, Variable);
	ErrBox(ErrBuffer);
}

void DoEvents()
{
	InDoEvents = true;
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	}
	InDoEvents = false;
}


bool DoInstall(char* InstallTo, bool RunOnEnd, HWND hDlg)
{
	char ZipFile[MyMaxPath];
	GetModuleFileName(hInst, ZipFile, MyMaxPath);

#ifdef _DEBUG
	//make practical debugging a reality
	strcat(ZipFile, ".zip");
#endif

	BlueZip b(ZipFile);
	if (!b.Read())
	{
		ErrBox("Corrupt installer data, please try redownloading");
		return true;
	}

	//Now replace / with \ to make it less confusing
	NormalPath(InstallTo);

	//And guarantee it has a trailing slash
	int BufLen = strlen(InstallTo);
	char* BufPos = &InstallTo[BufLen];
	if ((BufLen > 0) && (BufPos[-1] != '\\'))
	{
		BufPos[0] = '\\';
		BufPos++;
		BufPos[0] = 0;
	}
	

	//Now see if you can create the directory
	if (ExistsDir(InstallTo))
	{
		if (QuestBox("The directory already exists, continuing may overwrite some files.\n"
			     "\n"
				 "Do you wish to continue?", MB_YESNO) == IDNO)
			return false;
	}
	else
	{
		if (!CreateFolder(InstallTo))
		{
			ErrBox("Could not create the specified directory");
			return false;
		}
	}



	//Buffer is C:\\Program Files\\ProgramDir\\<space>
	//BufPos is <space>

	//Now test the primary file
	int TimeOut = 2; //seconds before giving up and asking the user
	if (PrimaryFile != NULL)
	{
		strcpy(BufPos, PrimaryFile);
		while (Exists(InstallTo) && !DeleteFile(InstallTo))
		{
			if (TimeOut > 0)
			{
				TimeOut--;
				Sleep(1000);
				DoEvents();
			}
			else
			{
				if (QuestBox("The program \"" PrimaryFile "\" is currently running, please exit it before continuing.",
					MB_RETRYCANCEL) == IDCANCEL)
					return false;
			}
		}
	}

	const int PrgFactor = 4096;
	int Sum = 0;
	for (zList* i = b.Files; i != NULL; i = i->next)
		Sum += i->CompressedSize();
	SendDlgItemMessage(hDlg, prgBar, PBM_SETRANGE, 0, MAKELPARAM(0, Sum / PrgFactor));
	Sum = 0;


	//now you have access to at least the file Str
	//extract all the files
	for (i = b.Files; i != NULL; i = i->next)
	{
		SendDlgItemMessage(hDlg, prgBar, PBM_SETPOS, Sum / PrgFactor, 0);
		Sum += i->CompressedSize();

		strcpy(BufPos, i->FileName);
		NormalPath(BufPos);
		SetDlgItemText(hDlg, lblInstallFile, InstallTo);
		DoEvents();
		
		//if the last char is a '\' then directory
		if (BufPos[strlen(BufPos)-1] == '\\')
		{
			if (!ExistsDir(InstallTo) && !CreateFolder(InstallTo))
			{
				ErrDialog(hDlg, "Could not create the directory\n\n", InstallTo);
				return false;
			}
		}
		else
		{
			if ( (Exists(InstallTo) && !DeleteFile(InstallTo)) ||
				 (!b.GetFile(i, InstallTo)))
			{
				ErrDialog(hDlg, "Could not extract the file\n\n", InstallTo);
				return false;
			}
		}
	}
	SendDlgItemMessage(hDlg, prgBar, PBM_SETPOS, Sum / PrgFactor, 0);
	SetDlgItemText(hDlg, lblInstallFile, "Finalising...");

	//now InstallTo points to the primary file
	strcpy(BufPos, PrimaryFile);
	if (IsDlgButtonChecked(hDlg, chkShortcut) == BST_CHECKED)
	{
		if (!CreateDesktopShortcut(hDlg, InstallTo))
			ErrBox("Could not create the desktop shortcut");
	}

	if (RunOnEnd)
	{
		if ((int) ShellExecute(hDlg, NULL, InstallTo, NULL, NULL, SW_SHOWDEFAULT) <= 32)
			ErrBox("Could not run file " PrimaryFile);
	}

	if (hDlg != NULL)
		InfoBox(ProgramName " successfully installed");
	return true;
}

void ShowProgress(HWND hDlg, bool State)
{
	int Show[] = {txtEdit, cmdBrowse, lblWelcome, 0};
	int Hide[] = {prgBar, lblInstallTo, lblInstallFile, 0};
	int Enable[] = {chkExecute, chkShortcut, IDOK, IDCANCEL, 0};

	int i;
	for (i = 0; Show[i] != 0; i++)
		ShowWindow(GetDlgItem(hDlg, Show[i]), (State ? SW_HIDE : SW_SHOW));
	for (i = 0; Hide[i] != 0; i++)
		ShowWindow(GetDlgItem(hDlg, Hide[i]), (State ? SW_SHOW : SW_HIDE));
	for (i = 0; Enable[i] != 0; i++)
		EnableWindow(GetDlgItem(hDlg, Enable[i]), !State);

	DoEvents();
}

bool TryInstall(HWND hDlg)
{
	char Buffer[MyMaxPath];
	GetDlgItemText(hDlg, txtEdit, Buffer, MyMaxPath);
	SetDlgItemText(hDlg, lblInstallFile, "Initialising...");
	SendDlgItemMessage(hDlg, prgBar, PBM_SETPOS, 0, 0);

	char Buff2[MyMaxPath];
	strcpy(Buff2, "Installing to ");
	strcat(Buff2, Buffer);
	SetDlgItemText(hDlg, lblInstallTo, Buff2);

	ShowProgress(hDlg, true);

	return DoInstall(Buffer, IsDlgButtonChecked(hDlg, chkExecute) == BST_CHECKED, hDlg);
}

int CALLBACK DlgFunc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		ShellInit();
		InitDialog(hDlg);
		break;

	case WM_DESTROY:
		ShellDest();
		break;

	case WM_PAINT:
		PaintDialog(hDlg);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			if (!InDoEvents)
				EndDialog(hDlg, 0);
			break;

		case IDOK:
			if (TryInstall(hDlg))
				EndDialog(hDlg, 0);
			else
				ShowProgress(hDlg, false);
			break;

		case cmdBrowse:
			Browse(hDlg, GetDlgItem(hDlg, txtEdit));
			break;
		}
		break;
	}

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(dlgInstall), NULL, DlgFunc);
	return 0;
}
