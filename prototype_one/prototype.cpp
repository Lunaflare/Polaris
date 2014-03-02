//---------------------------------------------------------------------------

#include <windows.h>
#include <Registry.hpp>
#include <fmx.h>
#pragma hdrstop

#include "prototype.h"
#include "Unit2.h"
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
		username = usernameEdit->Text;
		password = passwordEdit->Text;
		inputLevel = SQLQuery1->Fields->Fields[9]->AsString;
		readLevel = SQLQuery1->Fields->Fields[8]->AsString;
		accessLevel = SQLQuery1->Fields->Fields[4]->AsString;
		userID = SQLQuery1->Fields->Fields[0]->AsString;
		hotelID = SQLQuery1->Fields->Fields[7]->AsString;

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
	ShellExecute(NULL, L"open", L"http://baldwinserver.com/Lunaflare/generateKey.php", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

//Opens browser with recover password form when register button clicked (still need to create form)
void __fastcall TForm1::recoverPasswordButtonClick(TObject *Sender)
{
	ShellExecute(NULL, L"open", L"http://www.facebook.com/l.php?u=http%3A%2F%2Flunaflare.com%2FaccountRecovery&h=_AQHbaAuc", NULL, NULL, SW_SHOWNORMAL);
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

