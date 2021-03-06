//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "prototype.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit5.h"
#include "Unit7.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm7 *Form7;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm7::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form5->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm7::FormShow(TObject *Sender)
{
	//initially set state to zero
	state = 0;

	//query hotel_ref to get id, input, read, and labor
    currentHotelID = Form1->getHotelID();
	Form3->SQLQuery2->SQL->Text = "SELECT input_table, read_table, labor_table FROM hotel_ref WHERE hotelID = '"+currentHotelID+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	if (!Form3->SQLQuery2->Eof)
	{
		inputTable = Form3->SQLQuery2->Fields->Fields[0]->AsString;
		readTable = Form3->SQLQuery2->Fields->Fields[1]->AsString;
		laborTable = Form3->SQLQuery2->Fields->Fields[2]->AsString;
	}

	//populate rolePopupBox from role_table
	bool noAdd = false;
	String nonEditableRoles[3] = {"AM_Room_Attendants", "PM_Room_Attendants", "Laundry_Attendant"};
	Form3->SQLQuery2->SQL->Text = "SELECT Bare_Role_Name FROM role_table WHERE hotelID = '"+currentHotelID+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	while (!Form3->SQLQuery2->Eof)
	{
		for (int i = 0; i < 3; ++i)
			if (SameText(nonEditableRoles[i], Form3->SQLQuery2->Fields->Fields[0]->AsString))
				noAdd = true;

		if (!noAdd)
			rolePopupBox->Items->Add(StringReplace(Form3->SQLQuery2->Fields->Fields[0]->AsString, "_", " ", TReplaceFlags() << rfReplaceAll));

		Form3->SQLQuery2->Next();
		noAdd = false;
	}

	rolePopupBox->ItemIndex = 0;
	rolePopupBox->Text = rolePopupBox->Items->operator [](rolePopupBox->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TForm7::homeImageButton7Click(TObject *Sender)
{
	Form7->Hide();
	Form2->Show();

	//set things back to empty
	chooseRoleLabel->Text = "Choose a role to edit: ";
	rolePopupBox->Items->Clear();
	rolePopupBox->ItemIndex = 0;
	rolePopupBox->Text = "";
	rolePopupBox->Visible = true;
	roleNameLabel->Visible = false;
	roleNameEdit->Visible = false;
	roleWagesLabel->Visible = false;
	roleWagesEdit->Visible = false;
	wagesCallout->Visible=false;
	inputCallout->Visible=false;
	selectCallout->Visible=false;
	errorLabel->Visible = false;
	saveChangesButton->Visible = false;
	nextImageButton->Visible = true;
	dollaSign->Visible=false;

	roleExistsLabel->Visible=false;
	checkInputImage->Visible=false;
	editRoleLabel->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::backImageButtonClick(TObject *Sender)
{
	roleExistsLabel->Visible=false;
	checkInputImage->Visible=false;
	editRoleLabel->Visible=true;
		//fade out Error images
	 errorLabel->Visible=false;
	 inputCallout->Visible=false;
	 wagesCallout->Visible=false;
	 selectCallout->Visible=false;

	//should run this when in phase 0
	if (state == 0)
	{
		Form7->Hide();
		Form5->Show();

		//set things back to empty
		chooseRoleLabel->Text = "Choose a role to edit: ";
		rolePopupBox->Items->Clear();
		rolePopupBox->ItemIndex = 0;
		rolePopupBox->Text = "";
	}
	else
	{
		//state is 1, show state zero
		state--;
		chooseRoleLabel->Text = "Choose a role to edit: ";
		nextImageButton->Visible = true;
		saveChangesButton->Visible = false;
		rolePopupBox->Visible = true;
		roleNameLabel->Visible = false;
		roleNameEdit->Visible = false;
		roleWagesLabel->Visible = false;
		roleWagesEdit->Visible = false;
		errorLabel->Visible = false;
		dollaSign->Visible=false;
		roleExistsLabel->Visible=false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm7::nextImageButtonClick(TObject *Sender)
{
roleExistsLabel->Visible=false;
	checkInputImage->Visible=false;
	editRoleLabel->Visible=true;
	//hide/make visible certain buttons
	nextImageButton->Visible = false;
	saveChangesButton->Visible = true;
	rolePopupBox->Visible = false;
	roleNameLabel->Visible = true;
	roleNameEdit->Visible = true;
	roleWagesLabel->Visible = true;
	roleWagesEdit->Visible = true;
	dollaSign->Visible=true;
	roleExistsLabel->Visible=false;
		//fade out Error images
	 errorLabel->Visible=false;
	 inputCallout->Visible=false;
	 wagesCallout->Visible=false;
	 selectCallout->Visible=false;

	//change state value
	state++;

	//get chosen item from rolePopupBox
	roleChosen = rolePopupBox->Items->operator [](rolePopupBox->ItemIndex);

	//get wages for selected role
	Form3->SQLQuery2->SQL->Text = "SELECT Role_Wages FROM role_table WHERE hotelID = '"+currentHotelID+"' AND Bare_Role_Name = '"+StringReplace(roleChosen, " ", "_", TReplaceFlags() << rfReplaceAll)+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	if (!Form3->SQLQuery2->Eof)
		roleWages = Form3->SQLQuery2->Fields->Fields[0]->AsFloat;
	else
		roleWages = 0.0;

	//change items to represent current role information
	chooseRoleLabel->Text = "Currently editing " + roleChosen + ":";
	roleNameEdit->Text = roleChosen;
	roleWagesEdit->Text = roleWages;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::saveChangesButtonClick(TObject *Sender)
{

	//fade out Error images
	checkInputImage->Visible=false;
	 errorLabel->Visible=false;
	 inputCallout->Visible=false;
	 wagesCallout->Visible=false;
	 selectCallout->Visible=false;


		//check to see if string contains only digits and one decimal
		int numDecimals=0;
			//check for blank input
		if(roleWagesEdit->Text.IsEmpty() || roleNameEdit->Text.IsEmpty())
		{
			editRoleLabel->Visible=false;
			checkInputImage->Visible=true;
			return;
		}

		//check name input
		string inputString=AnsiString(roleNameEdit->Text).c_str();
		for (int i = 0; i < inputString.length(); i++)
		{

			//check to see if the character is within the a-Z range
			if(inputString[i]>=65 || 122<=inputString[i])
			{
			  if(90<inputString[i] && inputString[i]<97)
				{
				  inputCalloutLabel->Text="Can only contain letters!";
				  inputCallout->Visible=true;
				  checkInputImage->Visible=true;
				  editRoleLabel->Visible=false;
				  return;
				}
			}
			else if(inputString[i]==32)
				{
					//do not allow to start with space
					if(i==0)
						{
							inputCalloutLabel->Text="Cannot begin with a space!";
							inputCallout->Visible=true;
							checkInputImage->Visible=true;
							editRoleLabel->Visible=false;
							return;
						}

					//or end with a space
					if(i==inputString.size()-1)
					{
					  inputCalloutLabel->Text="Cannot end with a space!";
					  inputCallout->Visible=true;
					  checkInputImage->Visible=true;
					  editRoleLabel->Visible=false;
							return;
                    }

					//make sure it's not a double space
					if(inputString[i+1]==32)
						{
							inputCalloutLabel->Text="Cannot contain double space!";
							inputCallout->Visible=true;
							checkInputImage->Visible=true;
							editRoleLabel->Visible=false;
							return;
                        }
				}
			else
			{
				inputCalloutLabel->Text="Can only contain letters!";
				inputCallout->Visible=true;
				checkInputImage->Visible=true;
				editRoleLabel->Visible=false;
				return;
			}

		}
		//check wages input
		inputString=AnsiString(roleWagesEdit->Text).c_str();
		for (int i = 0; i < inputString.length(); i++)
		{

			//check to see if the character is 0-9 or a decimal
			if(inputString[i]<48 || inputString[i]>57)
			{
				if(inputString[i]==46)
				{
					numDecimals++;
					if(numDecimals>1)
					{
						roleWagesEdit->Text="";
						editRoleLabel->Visible=false;
						checkInputImage->Visible=true;
						wagesCallout->Visible=true;
						return;
					}
					if(inputString[i+1]<48 || inputString[i+1]>57)
					{
						roleWagesEdit->Text="";
						editRoleLabel->Visible=false;
						checkInputImage->Visible=true;
						wagesCallout->Visible=true;
						return;
					}
				}
				else
				{
						roleWagesEdit->Text="";
						editRoleLabel->Visible=false;
						checkInputImage->Visible=true;
						wagesCallout->Visible=true;
						return;
				}

			}
		}

	//check to make sure one of the drop down choices have been selected
	if(rolePopupBox->ItemIndex<0)
	{
		editRoleLabel->Visible=false;
		checkInputImage->Visible=true;
		selectCallout->Visible=true;
		return;
	}
	else
	{
		//store info
	   //	roleReference = StringReplace(rolePopupBox->Items->operator [](referencePopupBox->ItemIndex), " ", "_", TReplaceFlags() << rfReplaceAll);
	}


	//get values from edit boxes
	String roleNameNew = roleNameEdit->Text;
	String roleWagesNew = roleWagesEdit->Text;

	//get all existing role names and compare to name chosen
	bool halt = false;
	Form3->SQLQuery2->SQL->Text = "SELECT Bare_Role_Name FROM role_table WHERE hotelID = '"+currentHotelID+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	while (!Form3->SQLQuery2->Eof)
	{
		if (SameText(StringReplace(roleNameNew, " ", "_", TReplaceFlags() << rfReplaceAll), Form3->SQLQuery2->Fields->Fields[0]->AsString) && !SameText(roleNameNew, roleChosen))
			halt = true;

		Form3->SQLQuery2->Next();
	}

	//check if name already exists in db
	if (!halt)
	{
		//construct strings useful in update queries below
		String roleNameNewUnderScores = StringReplace(roleNameNew, " ", "_", TReplaceFlags() << rfReplaceAll);
		String roleNameNewHoursPaid = roleNameNewUnderScores + "_Hours_Paid";
		String roleNameNewStandardHours = roleNameNewUnderScores + "_Standard_Hours";
		String roleNamePercentPerformance = roleNameNewUnderScores + "_Percent_Performance";

		//update query for role table
		Form3->SQLQuery2->SQL->Text = "UPDATE baldwins_hotel_data.role_table SET Role_Name = '"+roleNameNewHoursPaid+"', Bare_Role_Name = '"+roleNameNewUnderScores+"', Role_Wages = '"+roleWagesNew+"' WHERE hotelID = '"+currentHotelID+"' AND Bare_Role_Name = '"+StringReplace(roleChosen, " ", "_", TReplaceFlags() << rfReplaceAll)+"';";
		Form3->SQLQuery2->ExecSQL();

		//update query for labor standards table
		Form3->SQLQuery2->SQL->Text = "ALTER TABLE baldwins_hotel_data."+laborTable+" CHANGE COLUMN "+StringReplace(roleChosen, " ", "_", TReplaceFlags() << rfReplaceAll)+" "+roleNameNewUnderScores+" DOUBLE NOT NULL;";
		Form3->SQLQuery2->ExecSQL();

		//update query for read table
		Form3->SQLQuery2->SQL->Text = "ALTER TABLE baldwins_hotel_data."+readTable+" CHANGE COLUMN "+StringReplace(roleChosen, " ", "_", TReplaceFlags() << rfReplaceAll) + "_Hours_Paid"+" "+roleNameNewHoursPaid+" DOUBLE NOT NULL, CHANGE COLUMN "+StringReplace(roleChosen, " ", "_", TReplaceFlags() << rfReplaceAll) + "_Standard_Hours"+" "+roleNameNewStandardHours+" DOUBLE NOT NULL, CHANGE COLUMN "+StringReplace(roleChosen, " ", "_", TReplaceFlags() << rfReplaceAll) + "_Percent_Performance"+" "+roleNamePercentPerformance+" DOUBLE NOT NULL;";
		Form3->SQLQuery2->ExecSQL();

		//update query for input table
		Form3->SQLQuery2->SQL->Text = "ALTER TABLE baldwins_hotel_data."+inputTable+" CHANGE COLUMN "+StringReplace(roleChosen, " ", "_", TReplaceFlags() << rfReplaceAll) + "_Hours_Paid"+" "+roleNameNewHoursPaid+" DOUBLE NOT NULL;";
		Form3->SQLQuery2->ExecSQL();

		//recalculate Average_Pay_Per_Hourour and Overtime_Per_Hour int hotel ref table to accommodate new role's wages
		String empty = "";
		double wagesTotal = 0.0;
		double numWages = 0.0;
		Form3->SQLQuery2->SQL->Text = "SELECT Role_Wages FROM role_table WHERE hotelID = '"+currentHotelID+"';";
		Form3->SQLQuery2->Open();
		Form3->SQLQuery2->First();
		while (!Form3->SQLQuery2->Eof)
		{
			++numWages;
			wagesTotal += Form3->SQLQuery2->Fields->Fields[0]->AsFloat;

			Form3->SQLQuery2->Next();
		}
		double avgPayPerHour = wagesTotal / numWages;
		double overtimePerHour = avgPayPerHour * 1.5;

		//update hotel_ref with new values
		Form3->SQLQuery2->SQL->Text = "UPDATE baldwins_hotel_data.hotel_ref SET Average_Pay_Per_Hour = '"+empty+avgPayPerHour+"', Overtime_Per_Hour = '"+empty+overtimePerHour+"' WHERE hotelID = '"+currentHotelID+"';";
		Form3->SQLQuery2->ExecSQL();

		//take back to settings page (intentionally called twice)
		backImageButton->OnClick(NULL);
		backImageButton->OnClick(NULL);
	}
	else
	{
		//errorLabel->Text = "Error: the role " + roleNameNew + " already exists. Please try again...";
		//errorLabel->Visible = true;
		roleExistsLabel->Visible=true;
    }
}

//---------------------------------------------------------------------------
void __fastcall TForm7::homeImageButton7MouseEnter(TObject *Sender)
{
swapImage->Bitmap=homeImageButton7->Bitmap;
homeImageButton7->Bitmap=homeOverImage->Bitmap;
homeShadow->Visible=true;
homeLabel->Visible=true;

}
//---------------------------------------------------------------------------

void __fastcall TForm7::homeImageButton7MouseLeave(TObject *Sender)
{
homeImageButton7->Bitmap=swapImage->Bitmap;
homeShadow->Visible=false;
homeLabel->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::backImageButtonMouseEnter(TObject *Sender)
{
swapImage->Bitmap=backImageButton->Bitmap;
backImageButton->Bitmap=backOverImage->Bitmap;


backLabel->Visible=true;
backShadow->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::backImageButtonMouseLeave(TObject *Sender)
{
backImageButton->Bitmap=swapImage->Bitmap;
backShadow->Visible=false;
backLabel->Visible=false;

}
//---------------------------------------------------------------------------

void __fastcall TForm7::nextImageButtonMouseEnter(TObject *Sender)
{
swapImage->Bitmap=nextImageButton->Bitmap;
nextImageButton->Bitmap=nextOverImage->Bitmap;
nextShadow->Visible=true;
nextLabel->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::nextImageButtonMouseLeave(TObject *Sender)
{
nextImageButton->Bitmap=swapImage->Bitmap;
nextShadow->Visible=false;
nextLabel->Visible=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm7::signOutButtonOnClick(TObject *Sender)
{
	Form7->Hide();
	Form1->Show();
}
//---------------------------------------------------------------------------

