//---------------------------------------------------------------------------

#include <windows.h>
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
	//query the db for username and password
	SQLQuery1->SQL->Text="select * from user_info where username='"+usernameEdit->Text+"' and password=md5('"+passwordEdit->Text+"');";
	SQLQuery1->Open();
	SQLQuery1->First();

	//check if empty return or not
	if(!SQLQuery1->Eof)
	{
		//success, spawn next form
		errorLabel->Visible = false;
		Form2->Show();

		//form2 (welcome form) is now the currently visible form, display username hello message
		String username = usernameEdit->Text;
		Form2->welcomeUserLabel->Text = "Hello " + username + "!";
        Form2->welcomeUserLabel->Visible = true;
	}
	else
	{
		//failure, alert user to incorrect entry
		errorLabel->Visible = true;

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
	ShellExecute(NULL, L"open", L"http://baldwinserver.com/Lunaflare/generateKey.php", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

