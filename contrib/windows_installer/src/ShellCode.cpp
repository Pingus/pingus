#include "Header.h"
#include "Parameters.h"
#include <shlobj.h>


bool OleReady;

void ShellInit()
{
	HRESULT hres = OleInitialize(NULL);
	OleReady = ((hres == S_FALSE) || (hres == S_OK));
}

void ShellDest()
{
	if (OleReady)
		OleUninitialize();
}


bool CreateShortcut(char* Destination, char* Target, char* StartIn, char* Parameters, char* Desc)
{
	if (!OleReady)
		return false;

    HRESULT hres;
    IShellLink* psl;

    // Get a pointer to the IShellLink interface.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                            IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath(Target);
		if (Parameters != NULL) psl->SetArguments(Parameters);
        if (Desc != NULL) psl->SetDescription(Desc);
		if (StartIn != NULL) psl->SetWorkingDirectory(StartIn);

        // Query IShellLink for the IPersistFile interface for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            WCHAR wsz[MAX_PATH];

            // Ensure that the string is Unicode.
            MultiByteToWideChar(CP_ACP, 0, Destination, -1, wsz, MAX_PATH);

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(wsz, TRUE);
            ppf->Release();
        }
        psl->Release();
    }
    return (SUCCEEDED(hres) ? true : false);
}


bool GetFolder(HWND hDlg, int nFolder, char* Buffer)
{
	LPITEMIDLIST idl;
	SHGetSpecialFolderLocation(hDlg, nFolder, &idl);
	if (idl == 0) return false;

	BOOL res = SHGetPathFromIDList(idl, Buffer);
	CoTaskMemFree(idl);
	return (res != FALSE);
}


bool CreateDesktopShortcut(HWND hDlg, char* Folder)
{
	char Destination[MyMaxPath];
	if (!GetFolder(hDlg, CSIDL_DESKTOP, Destination))
		return false;

	strcat(Destination, "\\" ProgramName ".lnk");

	char Target[MyMaxPath];
	strcpy(Target, Folder);
	strcat(Target, "\\" PrimaryFile);

	return CreateShortcut(Destination, Target, Folder, NULL, ProgramName " - " Description);
}

bool CreateStartMenuShortcut(HWND hDlg, char* Folder)
{
	char Destination[MyMaxPath];
	if (!GetFolder(hDlg, CSIDL_PROGRAMS, Destination))
		return false;

	strcat(Destination, "\\" ProgramName);
	if (!CreateDirectory(Destination, NULL))
		return false;

	char* i = &Destination[strlen(Destination)];
	i[0] = '\\';
	i++;

	char Target[MyMaxPath];
	strcpy(Target, Folder);
	strcat(Target, "\\" PrimaryFile);

	strcpy(i, ProgramName ".lnk");
	bool res = CreateShortcut(Destination, Target, Folder, NULL, ProgramName " - " Description);

	strcpy(i, "Readme.lnk");
	strcpy(&Target[strlen(Folder)+1], "readme.htm");
	res &= CreateShortcut(Destination, Target, NULL, NULL, ProgramName " - Read Me");

	return res;
}


void GetProgramFiles(HWND hDlg, char* Buffer)
{
	char* s = getenv("PROGRAMFILES");
	strcpy(Buffer, (s != NULL ? s : "C:\\Program Files"));
}




void Browse(HWND hDlg, HWND hText)
{
	const int bif_NEWDIALOGSTYLE = 0x40;

	BROWSEINFO bi;
	bi.hwndOwner = hDlg;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = "Please select the installation folder";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | bif_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST idl = SHBrowseForFolder(&bi);

	if (idl != NULL)
	{
		char Buffer[MyMaxPath];
		SHGetPathFromIDList(idl, Buffer);
		SetWindowText(hText, Buffer);
		CoTaskMemFree(idl);
	}
}
