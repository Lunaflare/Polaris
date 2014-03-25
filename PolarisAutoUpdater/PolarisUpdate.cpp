//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "PolarisUpdate.h"
#include "windows.h"
#include "Shellapi.h"
#include "sys/stat.h"
#include <fstream>
#include "time.h"
#include <map>
#include <Urlmon.h>
#include <string>
#include <wininet.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
#pragma comment(lib, "UrlMon.lib")
#pragma comment(lib, "Wininet.lib")

using namespace std;
TForm5 *Form5;
bool fexists(const char *filename);
bool needsRegistering=false;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm5::ThreadTerminated(TObject *Sender)
{
 exitTimer->Enabled=true;
}
__fastcall TupdateThread::TupdateThread()
	: TThread(true)
{
	FreeOnTerminate = true;
	// setup other thread parameters as needed...
}

void __fastcall TForm5::checkFiles()
{
Form5->statusLabel->Text="Checking Files ...";

 ifstream myfile;
 //ofstream outfile;
  myfile.open ("update.list");
  //outfile.open("results.txt");

  string temp="";
  string completeURL="";
  string file="";
  int lastChange;
  //this checks the file modified date
  struct tm* tmModifiedTime;
  struct stat attrib;
 //iterates through the list checking everyfile
   while(!myfile.eof())
  {
	//progressBar->Value+=5;
	myfile>>temp;
	myfile>>lastChange;

	//outfile<<temp<<" "<<lastChange;

	//This method checks file size differences!
	FILE * pFile;
	long size;

  pFile = fopen (temp.c_str(),"rb");

	if (pFile==NULL)
		size=0;
	else
		{
            fseek (pFile, 0, SEEK_END);   // non-portable
			size=ftell (pFile);
			fclose (pFile);
        }

	if(size==lastChange && fexists(temp.c_str()))
	{
		Form5->statusLabel->Text="checking "+AnsiString(temp.c_str())+" ...";
		//outfile<<" up to date \n";

	}

	else
	{

		//if(temp.find(".dll")!=temp.npos)
			if(temp=="midas.dll")
			needsRegistering=true;


		Form5->statusLabel->Text="downloading "+AnsiString(temp.c_str())+" ...";
	   //	outfile<<" \nDownloading "+temp+"\n";
		completeURL = "http://baldwinserver.com/Lunaflare/polaris/"+temp;

		wstring wideURL = std::wstring(completeURL.begin(), completeURL.end());
		wstring widetemp = std::wstring(temp.begin(), temp.end());
		const wchar_t* url2 = wideURL.c_str();
		const wchar_t* filePath2 = widetemp.c_str();



	// invalidate cache, so file is always downloaded from web site
	// (if not called, the file will be retieved from the cache if
	// it's already been downloaded.)
		DeleteUrlCacheEntry(url2);

		 HRESULT hr = URLDownloadToFile(
			NULL,   // A pointer to the controlling IUnknown interface (not needed here)
			url2,
			filePath2,
			0,      // Reserved. Must be set to 0.
			NULL ); // status callback interface (not needed for basic use)
			if(hr==0)
			{
				Form5->progressBar->Value+=10;
				//outfile<<tmModifiedTime->tm_sec<<" Success! \n";
			}
			else
			   {
				Form5->statusLabel->Text="Encountered an error ";
				ShowMessage(GetLastError());
				//outfile<<" ERROR! \n";
				//Form5->exitTimer->Enabled=true;
				return;

			   }

	   }
  }
  myfile.close();
  //outfile.close();
  //register
if(needsRegistering)
{
	statusLabel->Text="registering midas.dll ...";
   // OLD WAY	ShellExecute(NULL, NULL, L"regsvr32",L"midas.dll",NULL,SW_HIDE);
	ShellExecute(NULL,L"open",L"rundll32.exe", L"midas.dll, DllRegisterServer",NULL,SW_HIDE);
	// DEBUGGING ShowMessage(GetLastError());
}
  statusLabel->Text="Update complete!";
 progressBar->Value=100;

}


void __fastcall TupdateThread::Execute()
{
	Form5->checkFiles();
}
void __fastcall TForm5::FormShow(TObject *Sender)
{
statusLabel->Text="Contacting server ...";
progressBar->Value+=20;

	 const TCHAR url[] = _T("http://baldwinserver.com/Lunaflare/polaris/update.list");
	const TCHAR filePath[] = _T("update.list");

	// invalidate cache, so file is always downloaded from web site
	// (if not called, the file will be retieved from the cache if
	// it's already been downloaded.)
	DeleteUrlCacheEntry(url);

	HRESULT hr = URLDownloadToFile(
		NULL,   // A pointer to the controlling IUnknown interface (not needed here)
		url,
		filePath,
		0,      // Reserved. Must be set to 0.
		NULL ); // status callback interface (not needed for basic use)
	if(hr==0)
	{
	 int Continue=0;
	}
	else
	{
		statusLabel->Text="Could not contact update server";
		progressBar->Value=100;
		exitTimer->Enabled=true;
		return;
	}
	TupdateThread *thrdUpdate = new TupdateThread();
	thrdUpdate->OnTerminate = &ThreadTerminated;
	thrdUpdate->Resume();
}
//---------------------------------------------------------------------------
void __fastcall TForm5::exitTimerTimer(TObject *Sender)
{
Form5->Close();
}
//---------------------------------------------------------------------------
bool fexists(const char *filename)
{
  ifstream ifile(filename);
  return ifile;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::FormCloseQuery(TObject *Sender, bool &CanClose)
{
ShellExecute(NULL, NULL, L"Polaris.exe", NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------

