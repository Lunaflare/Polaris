//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit4.h"
#include "prototype.h"
#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//hides Form2 from view when this form is shown
void __fastcall TForm4::FormShow(TObject *Sender)
{
	//hide welcome form
	Form2->Hide();

	//things the same for both basic and advanced
    viewType = "Week";

    //get the hotelID and query hotel_ref to find out which table to input to and which table to read from
	String currentHotelID = Form1->getHotelID();
	inputTable = "";
	readTable = "";
	Form3->SQLQuery2->SQL->Text = "SELECT input_table, read_table, labor_table FROM hotel_ref WHERE hotelID = '"+currentHotelID+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	if (!Form3->SQLQuery2->Eof)
	{
		inputTable = Form3->SQLQuery2->Fields->Fields[0]->AsString;
		readTable = Form3->SQLQuery2->Fields->Fields[1]->AsString;
		laborTable = Form3->SQLQuery2->Fields->Fields[2]->AsString;
	}

	//if readLevel == 0, display basic read form (allow selection of roles)
	if (Form1->getReadLevel() == 0)
	{
		String bareRole = "";
		String bareRoleFormatted = "";
		basicState = 0;	//now filtering for roles

		//display first step basic read form items
		nextImageButton2->Visible = true;
		filtersLabel->Text = "Select Roles:";
		filtersLabel->Visible = true;

		//allow user to choose desired roles as filter
		Form3->SQLQuery2->SQL->Text = "SELECT Bare_Role_Name FROM baldwins_hotel_data.role_table WHERE hotelID = '"+currentHotelID+"';";
		Form3->SQLQuery2->Open();
		Form3->SQLQuery2->First();
		while (!Form3->SQLQuery2->Eof)
		{
			bareRole = Form3->SQLQuery2->Fields->Fields[0]->AsString;
			bareRoleFormatted = StringReplace(bareRole, "_", " ", TReplaceFlags() << rfReplaceAll);
			roleListBox->Items->Add(bareRoleFormatted);
			Form3->SQLQuery2->Next();
		}

		//show roleListBox once populated with roles
		selectAllButton->Text = "Select All";
		selectAllButton->Visible = true;
		roleListBox->Visible = true;
	}
	//otherwise display advanced read form
	else
	{

    }
}
//---------------------------------------------------------------------------

//avoids a never ending process by closing Form2 when this form is closed
void __fastcall TForm4::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form2->Close();
}
//---------------------------------------------------------------------------

//take user back to base menu when clicked
void __fastcall TForm4::homeImageButton4Click(TObject *Sender)
{
	Form4->Hide();
	Form2->Show();

	//make items not visible again so in good shape upon reentry
	nextImageButton2->Visible = false;
	filtersLabel->Visible = false;
	selectAllButton->Visible = false;
	dayRadio->Visible = false;
	weekRadio->Visible = false;
	monthRadio->Visible = false;
	yearRadio->Visible = false;
	dayRadio->IsChecked = false;
	weekRadio->IsChecked = false;
	monthRadio->IsChecked = false;
	yearRadio->IsChecked = false;
	selectAllButton->Text = "Select All";
	roleListBox->Items->Clear();
	roleVector.clear();
}

//---------------------------------------------------------------------------

void __fastcall TForm4::nextImageButton2Click(TObject *Sender)
{
	//check which state we are currently in (basicState?)
	switch (basicState)
	{
		//view filter
		case 0:
			//store all checked items from roleListBox
			for (int i = 0; i < roleListBox->Items->Count; ++i)
				if (roleListBox->ItemByIndex(i)->IsChecked)
					roleVector.push_back(roleListBox->Items->operator [](i));

			//handle things for view filter
			basicState++;
			filtersLabel->Text = "View By:";
			selectAllButton->Visible = false;
			roleListBox->Visible = false;
			backImageButton2->Visible = true;

			//handle signature things for view filter
			dayRadio->Visible = true;
			weekRadio->Visible = true;
			monthRadio->Visible = true;
			yearRadio->Visible = true;
			break;
		//week filter
		case 1:
			//hide things from view filter
			dayRadio->Visible = false;
			weekRadio->Visible = false;
			monthRadio->Visible = false;
			yearRadio->Visible = false;

			//store things from view filter
			if (dayRadio->IsChecked)
				viewType = "Day";
			else if (weekRadio->IsChecked)
				viewType = "Week";
			else if (monthRadio->IsChecked)
				viewType = "Month";
			else
				viewType = "Year";

			//increase basicState and display filterLabel appropriately
			basicState++;
			filtersLabel->Text = "Which " + viewType + "?";
			break;
		//basic show
		case 2:
			basicState++;
			filtersLabel->Visible = false;
			backImageButton2->Visible = false;
			nextImageButton2->Visible = false;
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm4::backImageButton2Click(TObject *Sender)
{
	//initialize local variabls used in switch statement (work around)
	String bareRole = "";
	String bareRoleFormatted = "";
	String currentHotelID = Form1->getHotelID();

	//check which state we are currently in (basicState?)
	switch (basicState)
	{
		case 1:
        	//hide things from view filter
			dayRadio->Visible = false;
			weekRadio->Visible = false;
			monthRadio->Visible = false;
			yearRadio->Visible = false;

			//role filter
			basicState--;
			filtersLabel->Text = "Select Roles:";

			//display first step basic read form items
			nextImageButton2->Visible = true;

			//show roleListBox once populated with roles
			backImageButton2->Visible = false;
			selectAllButton->Visible = true;
			roleListBox->Visible = true;
			break;
		case 2:
			// view filter
			basicState--;
			filtersLabel->Text = "View By:";
			selectAllButton->Visible = false;
			roleListBox->Visible = false;
			backImageButton2->Visible = true;

            //handle signature things for view filter
			dayRadio->Visible = true;
			weekRadio->Visible = true;
			monthRadio->Visible = true;
			yearRadio->Visible = true;
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm4::selectAllButtonClick(TObject *Sender)
{
	//check whether box is checked
	if (selectAllButton->Text == "Select All")
	{
		//check all items
		for (int i = 0; i < roleListBox->Items->Count; ++i)
			roleListBox->ItemByIndex(i)->IsChecked = true;

		selectAllButton->Text = "Select None";
	}
	else
	{
		//check none of the items
		for (int i = 0; i < roleListBox->Items->Count; ++i)
			roleListBox->ItemByIndex(i)->IsChecked = false;

		selectAllButton->Text = "Select All";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm4::roleListBoxChangeCheck(TObject *Sender)
{
	int counter = 0;

	for (int i = 0; i < roleListBox->Count; ++i)
	{
		if (roleListBox->ItemByIndex(i)->IsChecked == false)
		{
			break;
		}
		else
			++counter;
	}

	if (counter == roleListBox->Count)
	{
		selectAllButton->Text = "Select None";
	}
	else
	{
		selectAllButton->Text = "Select All";
    }
}
//---------------------------------------------------------------------------

