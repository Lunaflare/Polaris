//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit5.h"
#include "prototype.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//hides Form2 from view when this form is shown
void __fastcall TForm5::FormShow(TObject *Sender)
{
	Form2->Hide();

	//if admin, show the edit DLR button
	if (Form1->getAccessLevel() == 2)
		editDLRButton->Visible = true;

	//change radio buttons (input and read) to represent current database values
	if (Form1->getInputLevel() == 0)
	{
		inputDefaultRadio->IsChecked = true;
		inputAdvancedRadio->IsChecked = false;
	}
	else
	{
		inputDefaultRadio->IsChecked = false;
		inputAdvancedRadio->IsChecked = true;
	}

	if (Form1->getReadLevel() == 0)
	{
		readDefaultRadio->IsChecked = true;
		readAdvancedRadio->IsChecked = false;
	}
	else
	{
		readDefaultRadio->IsChecked = false;
		readAdvancedRadio->IsChecked = true;
	}
}
//---------------------------------------------------------------------------

//avoids a never ending process by closing Form2 when this form is closed
void __fastcall TForm5::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form2->Close();
}
//---------------------------------------------------------------------------

//take user back to base menu when clicked, doesn't save changes
void __fastcall TForm5::homeImageButton5Click(TObject *Sender)
{
	Form5->Hide();
	Form2->Show();
}
//---------------------------------------------------------------------------

//on click should save radio button changes to db, if password box not empty should prompt user to confirm password change
//will take user back to base menu when clicked
void __fastcall TForm5::saveChangesButtonClick(TObject *Sender)
{
	//if password edit blank, save as normal, otherwise prompt as to whether really wish to change password, if so, change
	if (passwordChangeEdit->Text != "")
	{
		if (passwordChangeEdit->Text != confirmPasswordEdit->Text)
		{
			ShowMessage(L"Passwords do not match");
		}
		else
		{
			int msgboxID = MessageBox(
				NULL,
				L"Would you really like to change your password?",
				L"Confirm Password Change",
				MB_ICONEXCLAMATION | MB_YESNO
			);
			if (msgboxID == IDYES)
			{
				//yes selected, save password and radios
                //set input and read levels appropriately based on status upon home button click
				String currentInputLevel;
				String currentReadLevel;
				String newPassword = passwordChangeEdit->Text;
				if (inputDefaultRadio->IsChecked)
					currentInputLevel = "0";
				else
					currentInputLevel = "1";
				if (readDefaultRadio->IsChecked)
					currentReadLevel = "0";
				else
					currentReadLevel = "1";

				//query database and change input and read levels appropriately
				Form1->SQLQuery1->SQL->Text="UPDATE baldwins_beta.user_info SET preference_read='"+currentReadLevel+"', password=md5('"+newPassword+"'), preference_write='"+currentInputLevel+"' WHERE user_info.userID='"+Form1->getUserID()+"';";
				Form1->SQLQuery1->ExecSQL();

				//set read and input levels appropriately given the desired changes (so populated upon return to this form)
				Form1->setReadLevel(currentReadLevel);
				Form1->setInputLevel(currentInputLevel);
				Form1->setPassword(newPassword);

				//hide current form and show base menu again
				Form5->Hide();
				Form2->Show();
			}
			else
			{
				//no selected, save nothing and show form again (do nothing)
			}
		}
	}
	else
	{
		//set input and read levels appropriately based on status upon home button click
		String currentInputLevel;
		String currentReadLevel;
		if (inputDefaultRadio->IsChecked)
			currentInputLevel = "0";
		else
			currentInputLevel = "1";
		if (readDefaultRadio->IsChecked)
			currentReadLevel = "0";
		else
			currentReadLevel = "1";

		//query database and change input and read levels appropriately
		Form1->SQLQuery1->SQL->Text="UPDATE baldwins_beta.user_info SET preference_read='"+currentReadLevel+"', preference_write='"+currentInputLevel+"' WHERE user_info.userID='"+Form1->getUserID()+"';";
		Form1->SQLQuery1->ExecSQL();

		//set read and input levels appropriately given the desired changes (so populated upon return to this form)
		Form1->setReadLevel(currentReadLevel);
		Form1->setInputLevel(currentInputLevel);

		//hide current form and show base menu again
		Form5->Hide();
		Form2->Show();
    }

}
//---------------------------------------------------------------------------

