//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "prototype.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit5.h"
#include "Unit6.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm6 *Form6;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm6::backImageButtonClick(TObject *Sender)
{
	Form6->Hide();
	Form5->Show();

	//set things back to empty
	roleNameEdit->Text = "";
	roleWagesEdit->Text = "";
	referencePopupBox->Items->Clear();
	referencePopupBox->ItemIndex = 0;
	errorLabel->Visible = false;
	checkInputImage->Visible=false;
	inputCallout->Visible=false;
	 wagesCallout->Visible=false;
	 selectCallout->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm6::saveChangesButtonClick(TObject *Sender)
{
	//get info from edit boxes and popup box
	String roleName = roleNameEdit->Text;
	String roleWages = roleWagesEdit->Text;
	String roleReference="";

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
			addRoleLabel->Visible=false;
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
				  addRoleLabel->Visible=false;
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
							addRoleLabel->Visible=false;
							return;
						}

					//or end with a space
					if(i==inputString.size()-1)
					{
					  inputCalloutLabel->Text="Cannot end with a space!";
					  inputCallout->Visible=true;
					  checkInputImage->Visible=true;
					  addRoleLabel->Visible=false;
							return;
                    }

					//make sure it's not a double space
					if(inputString[i+1]==32)
						{
							inputCalloutLabel->Text="Cannot contain double space!";
							inputCallout->Visible=true;
							checkInputImage->Visible=true;
							addRoleLabel->Visible=false;
							return;
                        }
				}
			else
			{
				inputCalloutLabel->Text="Can only contain letters!";
				inputCallout->Visible=true;
				checkInputImage->Visible=true;
				addRoleLabel->Visible=false;
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
						addRoleLabel->Visible=false;
						checkInputImage->Visible=true;
						wagesCallout->Visible=true;
						return;
					}
					if(inputString[i+1]<48 || inputString[i+1]>57)
					{
						roleWagesEdit->Text="";
						addRoleLabel->Visible=false;
						checkInputImage->Visible=true;
						wagesCallout->Visible=true;
						return;
					}
				}
				else
				{
						roleWagesEdit->Text="";
						addRoleLabel->Visible=false;
						checkInputImage->Visible=true;
						wagesCallout->Visible=true;
						return;
				}

			}
		}

	//check to make sure one of the drop down choices have been selected
	if(referencePopupBox->ItemIndex<0)
	{
		addRoleLabel->Visible=false;
		checkInputImage->Visible=true;
		selectCallout->Visible=true;
		return;
	}
	else
	{
		//store info
		roleReference = StringReplace(referencePopupBox->Items->operator [](referencePopupBox->ItemIndex), " ", "_", TReplaceFlags() << rfReplaceAll);
	}

	//get all existing role names and compare to name chosen
	bool halt = false;
	Form3->SQLQuery2->SQL->Text = "SELECT Bare_Role_Name FROM role_table WHERE hotelID = '"+currentHotelID+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	while (!Form3->SQLQuery2->Eof)
	{
		if (SameText(StringReplace(roleName, " ", "_", TReplaceFlags() << rfReplaceAll), Form3->SQLQuery2->Fields->Fields[0]->AsString))
			halt = true;

		Form3->SQLQuery2->Next();
	}

	//check if name is already in db
	if (!halt)
	{
		//get different forms of role name
		String bareRoleName = StringReplace(roleName, " ", "_", TReplaceFlags() << rfReplaceAll);
		String roleHoursPaid = bareRoleName + "_Hours_Paid";
		String roleStandardHours = bareRoleName + "_Standard_Hours";
		String rolePercentPerformance = bareRoleName + "_Percent_Performance";

		//add to role table
		String roleValues = "'" + currentHotelID + "', '" + roleHoursPaid + "', '" + bareRoleName + "', '" + roleWages + "', '" + roleReference + "'";
		Form3->SQLQuery2->SQL->Text = "INSERT INTO baldwins_hotel_data.role_table (hotelID, Role_Name, Bare_Role_Name, Role_Wages, Standard_Hours_Reference) VALUES ("+roleValues+");";
		Form3->SQLQuery2->ExecSQL();

		//add to input table
		Form3->SQLQuery2->SQL->Text = "ALTER TABLE baldwins_hotel_data."+inputTable+" ADD COLUMN "+roleHoursPaid+" DOUBLE NOT NULL AFTER Director_And_Two_Managers_Hours_Paid;";
		Form3->SQLQuery2->ExecSQL();

		//add to read table
		Form3->SQLQuery2->SQL->Text = "ALTER TABLE baldwins_hotel_data."+readTable+" ADD COLUMN "+roleHoursPaid+" DOUBLE NOT NULL AFTER Director_And_Two_Managers_Percent_Performance, ADD COLUMN "+roleStandardHours+" DOUBLE NOT NULL AFTER "+roleHoursPaid+", ADD COLUMN "+rolePercentPerformance+" DOUBLE NOT NULL AFTER "+roleStandardHours+";";
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

		//take back to settings page
		backImageButton->OnClick(NULL);
	}
	else
	{
		//display error message and stay at form
		//errorLabel->Text = "Error: the role " + roleName + " already exists. Please try again...";
		addRoleLabel->Visible=false;
		errorLabel->Visible = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm6::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form5->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm6::homeImageButton6Click(TObject *Sender)
{
	Form6->Hide();
	Form2->Show();

	//set things back to empty
	roleNameEdit->Text = "";
	roleWagesEdit->Text = "";
	referencePopupBox->Items->Clear();
	referencePopupBox->ItemIndex = 0;
	addRoleLabel->Visible=true;
	errorLabel->Visible = false;
	checkInputImage->Visible=false;
	inputCallout->Visible=false;
	 wagesCallout->Visible=false;
	 selectCallout->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm6::FormShow(TObject *Sender)
{
	//fill the rolePopupBox with possible labor standards references
	//query hotel ref to see which labor standards to pull references from
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

	//query information schema of labor standards table to get possible reference names
	Form3->SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_hotel_data' AND TABLE_NAME = '"+inputTable+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	Form3->SQLQuery2->Next();
	Form3->SQLQuery2->Next();
	vector<String> referenceVector;
	while (!Form3->SQLQuery2->Eof)
	{
		referenceVector.push_back(Form3->SQLQuery2->Fields->Fields[0]->AsString);
		Form3->SQLQuery2->Next();
	}
	for (int i = 0; i < 7; ++i)
		referencePopupBox->Items->Add(StringReplace(referenceVector[i], "_", " ", TReplaceFlags() << rfReplaceAll));
	addRoleLabel->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm6::homeImageButton6MouseEnter(TObject *Sender)
{
swapImage->Bitmap=homeImageButton6->Bitmap;
homeImageButton6->Bitmap=homeOverImage->Bitmap;
homeShadow->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm6::backImageButtonMouseEnter(TObject *Sender)
{
swapImage->Bitmap=backImageButton->Bitmap;
backImageButton->Bitmap=backOverImage->Bitmap;
backShadow->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm6::backImageButtonMouseLeave(TObject *Sender)
{
backImageButton->Bitmap=swapImage->Bitmap;
backShadow->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm6::homeImageButton6MouseLeave(TObject *Sender)
{
homeImageButton6->Bitmap=swapImage->Bitmap;
homeShadow->Visible=false;
}
//---------------------------------------------------------------------------


