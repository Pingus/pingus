#include "header.h"
#include "FileCode.h"
#include "ShellCode.h"
#include <commctrl.h>
#include <stdio.h>


#define PLAY_NICELY
#include "BlueZip\BlueHead.h"

#include "resource.h"
#include "Parameters.h"



#define ErrBox(Msg)				MessageBox(hDlg, Msg, ProgramName " Installer", MB_ICONERROR)
#define QuestBox(Msg, Flag)		MessageBox(hDlg, Msg, ProgramName " Installer", MB_ICONQUESTION | Flag)
#define InfoBox(Msg)			MessageBox(hDlg, Msg, ProgramName " Installer", MB_ICONINFORMATION)

// GLOBAL STATE
HINSTANCE hInst;
bool InDoEvents = false;
BlueZip zip;

int TotalCompSize;
int TotalRealSize;
// END GLOBAL STATE


//Predefines
void PathChanged(HWND hDlg);
//end



void InitDialog(HWND hDlg)
{
	InitCommonControls();

	char Buffer[MyMaxPath];
	GetProgramFiles(hDlg, Buffer);
	strcat(Buffer, "\\");
	strcat(Buffer, InstallDir);

	SetDlgItemText(hDlg, txtEdit, Buffer);
	CheckDlgButton(hDlg, chkExecute, BST_CHECKED);
	CheckDlgButton(hDlg, chkShortcutDesktop, BST_CHECKED);
	CheckDlgButton(hDlg, chkShortcutStart, BST_CHECKED);

	char ZipFile[MyMaxPath];
	GetModuleFileName(hInst, ZipFile, MyMaxPath);

#ifdef _DEBUG
	//make practical debugging a reality
	strcat(ZipFile, ".zip");
#endif

	zip.SetZipFile(ZipFile);
	if (!zip.Read())
	{
		ErrBox("Corrupt installer data, please try redownloading");
		DestroyWindow(hDlg);
		return;
	}

	TotalCompSize = 0;
	TotalRealSize = 0;
	for (zList* i = zip.Files; i != NULL; i = i->next)
	{
		TotalCompSize += i->CompressedSize();
		TotalRealSize += i->OriginalSize();
	}
	PathChanged(hDlg);
}

void PathChanged(HWND hDlg)
{
	static char LastPath = -1;
	char Buffer[MyMaxPath];

	GetDlgItemText(hDlg, txtEdit, Buffer, MyMaxPath);
	char NewLastPath;
	if (Buffer[1] == ':')
		NewLastPath = toupper(Buffer[0]);
	else
		NewLastPath = 0;
	if ((NewLastPath > 'Z') || (NewLastPath < 'A'))
		NewLastPath = 0;

	if (LastPath == NewLastPath)
		return;
	LastPath = NewLastPath;

	char Buf[MaxFileSizeBuf];
	FileSize(TotalRealSize, Buf);
	int Len = sprintf(Buffer, "Space required is %s", Buf);

	if (LastPath != 0)
	{
		ULARGE_INTEGER DriveSpace;
		char Buf[4] = {LastPath, ':', '\\', 0};
		if (!GetDiskFreeSpaceEx(Buf, &DriveSpace, NULL, NULL))
			DriveSpace.QuadPart = 0;

		FileSize(DriveSpace.QuadPart, Buf);
		sprintf(&Buffer[Len], ", installing on drive %c: which has %s free", LastPath, Buf);
	}

	SetDlgItemText(hDlg, lblSpace, Buffer);
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
	//Now replace / with \ to make it less confusing
	//And guarantee it has no trailing slash
	NormalPath(InstallTo);


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
		if (!EnsureFolder(InstallTo))
		{
			ErrBox("Could not create the specified directory");
			return false;
		}
	}

	strcat(InstallTo, "\\");
	int BufLen = strlen(InstallTo);
	char* BufPos = &InstallTo[BufLen];
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
	SendDlgItemMessage(hDlg, prgBar, PBM_SETRANGE, 0, MAKELPARAM(0, TotalCompSize / PrgFactor));
	int Done = 0;


	//now you have access to at least the file Str
	//extract all the files
	for (zList* i = zip.Files; i != NULL; i = i->next)
	{
		Done += i->CompressedSize();

		strcpy(BufPos, i->FileName);
		char LastChar = BufPos[strlen(BufPos)-1];
		bool IsFolder = ((LastChar == '\\') || (LastChar == '/'));
		NormalPath(BufPos);
		SetDlgItemText(hDlg, lblInstallFile, BufPos);
		DoEvents();

		//if the last char is a '\' then directory
		if (IsFolder)
		{
			if (!EnsureFolder(InstallTo))
			{
				ErrDialog(hDlg, "Could not create the directory\n\n", InstallTo);
				return false;
			}
		}
		else
		{
			if ( (Exists(InstallTo) && !DeleteFile(InstallTo)) ||
				 (!zip.GetFile(i, InstallTo)))
			{
				ErrDialog(hDlg, "Could not extract the file\n\n", InstallTo);
				return false;
			}
		}
		SendDlgItemMessage(hDlg, prgBar, PBM_SETPOS, Done / PrgFactor, 0);
	}
	SetDlgItemText(hDlg, lblInstallFile, "Finalising...");

	//now InstallTo is the install directory, plus a \\ character
	BufPos[-1] = 0;
	if (IsDlgButtonChecked(hDlg, chkShortcutDesktop) == BST_CHECKED)
	{
		if (!CreateDesktopShortcut(hDlg, InstallTo))
			ErrBox("Could not create the Desktop shortcut");
	}
	if (IsDlgButtonChecked(hDlg, chkShortcutStart) == BST_CHECKED)
	{
		if (!CreateStartMenuShortcut(hDlg, InstallTo))
			ErrBox("Could not create the Start Menu shortcut");
	}
	BufPos[-1] = '\\';

	//now InstallTo is the directory
	strcpy(BufPos, PrimaryFile);
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
	int Enable[] = {chkExecute, chkShortcutDesktop, chkShortcutStart, IDOK, IDCANCEL, 0};

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

		case txtEdit:
			if ((HIWORD(wParam) == EN_CHANGE) && (TotalRealSize != 0))
				PathChanged(hDlg);
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
