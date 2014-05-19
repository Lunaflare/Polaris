//---------------------------------------------------------------------------

#include <windows.h>
#include <Registry.hpp>
#include <fmx.h>
#pragma hdrstop

#include "prototype.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
#include "Unit6.h"
#include "Unit7.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//Will check whether username or password in db
//If so, will complete login
//If not, will display error message
void __fastcall TForm1::loginSubmitButtonClick(TObject *Sender)
{
	//checking for the sql comment to prevent hack
	string check=AnsiString(usernameEdit->Text).c_str();
	if(check.find("#")!=check.npos)
		{
				errorLabel->Text = "Username contains invalid symbol!";
				errorLabel->Visible = true;
			return;
		}
	check=AnsiString(passwordEdit->Text).c_str();
		if(check.find("#")!=check.npos)
		{
			errorLabel->Text = "Password contains invalid symbol!";
				errorLabel->Visible = true;
			return;
		}
	//query the db for username and password
	SQLQuery1->SQL->Text="select * from user_info where username='"+usernameEdit->Text+"' and password=md5('"+passwordEdit->Text+"');";
	SQLQuery1->Open();
	SQLQuery1->First();

	//check if empty return or not
	if(!SQLQuery1->Eof)
	{
		loadIndicator->Enabled=true;
		loadIndicator->Visible=true;

		//success (not empty), spawn next form, get rid of error message
		//first save some variables about the user
		errorLabel->Visible = false;
		setUsername(usernameEdit->Text);
		setPassword(passwordEdit->Text);
		setInputLevel(SQLQuery1->Fields->Fields[9]->AsString);
		setReadLevel(SQLQuery1->Fields->Fields[8]->AsString);
		setAccessLevel(SQLQuery1->Fields->Fields[4]->AsString);
		setUserID(SQLQuery1->Fields->Fields[0]->AsString);
		setHotelID(SQLQuery1->Fields->Fields[7]->AsString);
		setWeekStartDay(SQLQuery1->Fields->Fields[10]->AsInteger);

		//if rememberMe is set, update registry entry
		TRegistry* reg = new TRegistry(KEY_READ);
		reg->RootKey = HKEY_CURRENT_USER;
		reg->Access = KEY_WRITE;
		reg->OpenKey("Software\\Lunaflare\\Polaris\\", true);
		reg->WriteString("user", usernameEdit->Text);
		if(Switch1->IsChecked)
			reg->WriteInteger("remember", 1);
		else
			reg->WriteInteger("remember", 0);

		reg->CloseKey();
		reg->Free();

		//spawn the next form
		usernameEdit->Text = "";
		passwordEdit->Text = "";
		loadIndicator->Enabled=false;
		loadIndicator->Visible=false;

		usernameEdit->Enabled = false;
		usernameEdit->Enabled = true;

		passwordEdit->Enabled = false;
		passwordEdit->Enabled = true;
		Form1->Hide();
		Form2->Show();

	}
	else
	{
		//failure, alert user to incorrect entry
		errorLabel->Visible = true;

		//show recoverPasswordButton when user/pass is wrong(not blank)
		if(usernameEdit->Text!="" && passwordEdit->Text!="")
			{
				recoverPasswordButton->Visible=true;
				errorLabel->Text="Invalid username and/or password!";
				errorLabel->Visible=true;
			}
		else
			{
				recoverPasswordButton->Visible = false;
				errorLabel->Text = "Please fill out the fields below.";
				errorLabel->Visible = true;
			}
		//add glow effect to empty edit text boxes
		if (usernameEdit->Text == "")
			usernameGlowEffect->Enabled = true;
		else
			usernameGlowEffect->Enabled = false;

		if (passwordEdit->Text == "")
			passwordGlowEffect->Enabled = true;
		else
			passwordGlowEffect->Enabled = false;
	}
}
//---------------------------------------------------------------------------

//Opens browser with registration form when register button clicked (still need to create form)
void __fastcall TForm1::loginregisterButtonClick(TObject *Sender)
{
	ShellExecute(NULL, L"open", L"http://Lunaflare.com/register", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

//Opens browser with recover password form when register button clicked (still need to create form)
void __fastcall TForm1::recoverPasswordButtonClick(TObject *Sender)
{
	ShellExecute(NULL, L"open", L"http://Lunaflare.com/accountRecovery", NULL, NULL, SW_SHOWNORMAL);
}
//----------------------------------------------------------------------------

void __fastcall TForm1::passwordEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
if (Key==13)
	{
		loginSubmitButton->OnClick(NULL);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
//initialize registry settings
TRegistry* reg = new TRegistry(KEY_READ);
		reg->RootKey = HKEY_CURRENT_USER;
		reg->Access = KEY_READ;
		if(reg->OpenKey("Software\\Lunaflare\\Polaris\\", true))
		{
			if(reg->ValueExists("remember"))
			{
			  if(reg->ReadInteger("remember")==1)
			  {
				Switch1->IsChecked=true;
				usernameEdit->Text=reg->ReadString("user");
				passwordEdit->SetFocus();
			  }
			}
			else
			{
			reg->Access = KEY_WRITE;
			reg->OpenKey("Software\\Lunaflare\\Polaris\\", true);
			reg->WriteInteger("remember", 0);
			usernameEdit->SetFocus();
			}
		}
		reg->CloseKey();
		reg->Free();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Show()
{
	//initialize registry settings
	TRegistry* reg = new TRegistry(KEY_READ);
	reg->RootKey = HKEY_CURRENT_USER;
	reg->Access = KEY_READ;
	if(reg->OpenKey("Software\\Lunaflare\\Polaris\\", true))
	{
		if(reg->ValueExists("remember"))
		{
		  if(reg->ReadInteger("remember")==1)
		  {
			Switch1->IsChecked=true;
			usernameEdit->Text=reg->ReadString("user");
			passwordEdit->SetFocus();
		  }
		}
		else
		{
		reg->Access = KEY_WRITE;
		reg->OpenKey("Software\\Lunaflare\\Polaris\\", true);
		reg->WriteInteger("remember", 0);
		usernameEdit->SetFocus();
		}
	}
	reg->CloseKey();
	reg->Free();
	this->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	//terminate the app
	Application->Terminate();

	//loop until get confirmation from windows that the app has terminated
	int a = 12;
	int b = 40;
	int c = 1;
	for (int x = 0; x < 1000000; ++x)
	{
		//pointless calculation to extend timeeframe
		a = a * b * c + x;
		b = a + c + x;
		c = a + b;

		// check if need to exit
		Application->ProcessMessages();
		if (Application->Terminated)
		{
			break;
		}
	}

    //completely exit with good status to avoid windows errors
	exit(0);
}
//---------------------------------------------------------------------------

