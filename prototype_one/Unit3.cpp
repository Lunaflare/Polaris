//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "prototype.h"
#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//hides Form2 from view when this form is shown, sets up form for certain input level
void __fastcall TForm3::FormShow(TObject *Sender)
{
	//hide welcome form
	Form2->Hide();

	//set alreadyThere boolean to false
	alreadyThere = false;

	//get the current date in case accessLevel is 0
	TDateTime d = Now();
	pureDate = d;
	dateChosen = d.FormatString(L"yyyy-mm-dd");

	if (Form1->getAccessLevel() == 0)
	{
		//get the hotelID and query hotel_ref to find out which table to input to and which table to read from
		String currentHotelID = Form1->getHotelID();
		inputTable = "";
		readTable = "";
		SQLQuery2->SQL->Text = "SELECT input_table, read_table FROM hotel_ref WHERE hotelID = '"+currentHotelID+"';";
		SQLQuery2->Open();
		SQLQuery2->First();
		if (!SQLQuery2->Eof)
		{
			inputTable = SQLQuery2->Fields->Fields[0]->AsString;
			readTable = SQLQuery2->Fields->Fields[1]->AsString;
		}

		//show user input screen based on input level (i.e. default or advanced)
		if (Form1->getInputLevel() == 0)
		{
			//input level is "default", query db schema
			SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_hotel_data' AND TABLE_NAME = '"+inputTable+"';";

			//open query and temporarily skip first two column headings
			SQLQuery2->Open();
			SQLQuery2->First();
			SQLQuery2->Next();
			SQLQuery2->Next();

			//strings used in if for holdling column headings
			String originalHeading = "";
			String editedHeading = "";

			//set label to represent first editable column heading
			if(!SQLQuery2->Eof)
			{
				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

				//replace underscores with spaces and set label
				editedHeading = StringReplace(originalHeading, "_", " ",
					TReplaceFlags() << rfReplaceAll);
				dbFieldLabel->Text = editedHeading;
				dbFieldLabel->Visible = true;
				dbFieldEdit->Visible = true;
				dbFieldEdit->Text = "";
				nextImageButton->Visible = true;

				//initially set index and size to zero
				inputObject.currentIndex = 0;
				inputObject.size = 0;
			}
		}
		//input level is "advanced" for when userAccess == 0
		else
		{
			//hide form items
			dbFieldLabel->Visible = false;
			dbFieldEdit->Visible = false;
			nextImageButton->Visible = false;

			SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_hotel_data' AND TABLE_NAME = '"+inputTable+"';";

			//open query and temporarily skip first two column headings
			SQLQuery2->Open();
			SQLQuery2->First();
			SQLQuery2->Next();
			SQLQuery2->Next();
			displayGrid->Cells[0][0] = "Heading";
			displayGrid->Cells[1][0] = "Value";
			int count = 1;
			//strings used in if for holdling column headings
			String originalHeading = "";
			String editedHeading = "";

			while (!SQLQuery2->Eof)
			{
				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

				//replace underscores with spaces and set label
				editedHeading = StringReplace(originalHeading, "_", " ",
					TReplaceFlags() << rfReplaceAll);

				//show and populate displayGrid
				displayGrid->Cells[0][count] = editedHeading;
				displayGrid->Cells[1][count] = "";

				++count;
				SQLQuery2->Next();
			}

			displayGrid->RowCount = count;
			displayGrid->Visible = true;
			submitButton->Visible = true;
		}
	}
	else
	{
		//accessLevel is not 0, show date picker before progressing

		//hide uneccessary items from this page
		dbFieldLabel->Visible = false;
		dbFieldEdit->Visible = false;
		nextImageButton->Visible = false;

        //get today's date
		TDateTime d = Now();
		dateChosen = d.FormatString(L"yyyy-mm-dd");
		String currentDate = d.FormatString(L"mm/dd/yyyy");

		//populate the datePopupBox with the last week's dates (including today)
		datePopupBox->Items->Add(currentDate);
		for (int i = 0; i < 6; ++i)
		{
			datePopupBox->Items->Add((--d).FormatString(L"mm/dd/yyyy"));
		}

		//make date popupBox visible to user and set current date as chosen
		datePopupBox->ItemIndex = 0;
		chooseDateImageButton->Visible = true;
		datePopupBox->Visible = true;
		datePopupBoxLabel->Visible = true;
	}
}
//---------------------------------------------------------------------------

//avoids a never ending process by closing Form2 when this form is closed
void __fastcall TForm3::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form2->Close();
}
//---------------------------------------------------------------------------

//take user back to main menu when clicked
void __fastcall TForm3::homeImageButton3Click(TObject *Sender)
{
	//hide certain items
	backImageButton->Visible = false;
	displayGrid->Visible = false;
	submitButton->Visible = false;

	//clear datePopupBox items (need to do in other places also)
	datePopupBox->Items->Clear();

	//clear all items from inputObject
	inputObject.valueMap.clear();
	inputObject.size = 0;
	inputObject.currentIndex = 0;

	//hide current form and show welcome screen
	Form3->Hide();
	Form2->Show();
}
//---------------------------------------------------------------------------

//go through column headings as desired by the user
void __fastcall TForm3::nextImageButtonClick(TObject *Sender)
{
	if (inputObject.currentIndex == 0 && inputObject.size == 0)
	{
		//run when displayObject is empty

        //strings used in if for holdling column headings
		String originalHeading = "";
		String editedHeading = "";

		//get initial column heading without formatting (i.e. removing "_")
		originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

		//replace underscores with spaces and set label
		editedHeading = StringReplace(originalHeading, "_", " ",
			TReplaceFlags() << rfReplaceAll);

		//add data to inputObject and increase index/size
		vector<String> temp;
		temp.push_back(originalHeading);
		temp.push_back(editedHeading);
		temp.push_back(dbFieldEdit->Text);
		inputObject.valueMap[inputObject.currentIndex] = temp;
		dbFieldEdit->Text = "";
		(inputObject.currentIndex)++;
		(inputObject.size)++;
		originalHeading = "";
		editedHeading = "";

		//advance query
		SQLQuery2->Next();

		//get initial column heading without formatting (i.e. removing "_")
		originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

		//replace underscores with spaces and set label
		editedHeading = StringReplace(originalHeading, "_", " ",
			TReplaceFlags() << rfReplaceAll);
		dbFieldLabel->Text = editedHeading;

		backImageButton->Visible = true;
	}
	else if (inputObject.currentIndex == inputObject.size)
	{
		//will run when it is time to get the next column from the db

		//strings used in if for holdling column headings
		String originalHeading = "";
		String editedHeading = "";

		if(!SQLQuery2->Eof)
		{
			//get initial column heading without formatting (i.e. removing "_")
			originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

			//replace underscores with spaces and set label
			editedHeading = StringReplace(originalHeading, "_", " ",
				TReplaceFlags() << rfReplaceAll);

			//fill inputObject with data and increse index/size
			vector<String> temp;
			temp.push_back(originalHeading);
			temp.push_back(editedHeading);
			temp.push_back(dbFieldEdit->Text);
			inputObject.valueMap[inputObject.currentIndex] = temp;
			dbFieldEdit->Text = "";
			(inputObject.currentIndex)++;
			(inputObject.size)++;
			originalHeading = "";
			editedHeading = "";

			//advance query cursor
			SQLQuery2->Next();

			//check if now at the end of the query
			if(SQLQuery2->Eof)
			{
				//spawn next part that displays desired changes and asks if sure

				//hide all other items on form besides home button
				dbFieldLabel->Visible = false;
				dbFieldEdit->Visible = false;
				nextImageButton->Visible = false;

				//show and populate displayGrid
				displayGrid->RowCount = inputObject.valueMap.size() + 1;
				displayGrid->Cells[0][0] = "Heading";
				displayGrid->Cells[1][0] = "Value";
				for (int i = 0; i < inputObject.size; ++i)
				{
					displayGrid->Cells[0][i+1] = inputObject.valueMap[i][1];
					displayGrid->Cells[1][i+1] = inputObject.valueMap[i][2];
				}

				displayGrid->Visible = true;
				submitButton->Visible = true;
			}
			else
			{
				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

				//replace underscores with spaces and set label
				editedHeading = StringReplace(originalHeading, "_", " ",
					TReplaceFlags() << rfReplaceAll);
				dbFieldLabel->Text = editedHeading;
				backImageButton->Visible = true;
			}
		}
	}
	else
	{
		//don't query db, only go to next item in struct, replace item before advancing
		inputObject.valueMap[inputObject.currentIndex].pop_back();
		inputObject.valueMap[inputObject.currentIndex].push_back(dbFieldEdit->Text);
		dbFieldEdit->Text = "";
		(inputObject.currentIndex)++;

		if (inputObject.currentIndex == inputObject.size)
		{
			//accounts for case when coming back after pressing previous if data not previously captured

			//strings used in if for holdling column headings
			String originalHeading = "";
			String editedHeading = "";

			if(!SQLQuery2->Eof)
			{
				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = "";
				editedHeading = "";

				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

				//replace underscores with spaces and set label
				editedHeading = StringReplace(originalHeading, "_", " ",
					TReplaceFlags() << rfReplaceAll);
				dbFieldLabel->Text = editedHeading;

				backImageButton->Visible = true;
			}
			else
			{
              	//spawn next part that displays desired changes and asks if sure

				//hide all other items on form besides home button
				dbFieldLabel->Visible = false;
				dbFieldEdit->Visible = false;
				nextImageButton->Visible = false;

				//show and populate displayGrid
				for (int i = 0; i < inputObject.size; ++i)
				{
					displayGrid->Cells[0][i+1] = inputObject.valueMap[i][1];
					displayGrid->Cells[1][i+1] = inputObject.valueMap[i][2];
				}

				submitButton->Visible = true;
				displayGrid->Visible = true;
			}
		}
		else
		{
			//when next pressed and data for this index has previously been captured
			dbFieldEdit->Text = inputObject.valueMap[inputObject.currentIndex][2];
			dbFieldLabel->Text = inputObject.valueMap[inputObject.currentIndex][1];
			backImageButton->Visible = true;
        }

    }
}
//---------------------------------------------------------------------------

//go to previous item in struct, show in label field
void __fastcall TForm3::backImageButtonClick(TObject *Sender)
{
	//for when grid displayed and back button pressed
	if (displayGrid->Visible)
	{
		displayGrid->Visible = false;
		submitButton->Visible = false;
		dbFieldLabel->Visible = true;
		dbFieldEdit->Visible = true;
		nextImageButton->Visible = true;
	}

	(inputObject.currentIndex)--;
	if (inputObject.currentIndex == 0)
		backImageButton->Visible = false;

	dbFieldEdit->Text = inputObject.valueMap[inputObject.currentIndex][2];
	dbFieldLabel->Text = inputObject.valueMap[inputObject.currentIndex][1];
}
//---------------------------------------------------------------------------

//ask user if sure, yes == submit and no == do nothing
void __fastcall TForm3::submitButtonClick(TObject *Sender)
{
	int msgboxID = MessageBox(
		NULL,
		L"Would you really like to submit?",
		L"Confirm Input Submission",
		MB_ICONEXCLAMATION | MB_YESNO
	);

	if (msgboxID == IDYES)
	{
		//yes selected, save information in grid to database

		//create empty strings to add to query once populated from grid
		String headings = "";
		String values = "";
		String update = "";
		String updateHeading = "";

		//get day of week
		int Day = DayOfWeek(pureDate);
		String DayName;
		switch(Day)
		{
		case 1:
			DayName = L"Sunday";
			break;
		case 2:
			DayName = L"Monday";
			break;
		case 3:
			DayName = L"Tuesday";
			break;
		case 4:
			DayName = L"Wednesday";
			break;
		case 5:
			DayName = L"Thursday";
			break;
		case 6:
			DayName = L"Friday";
			break;
		case 7:
			DayName = L"Saturday";
		}

		//populate date and day of week (probably temporary solution)
		headings += ("Date, Day_Of_Week, ");
		values += ("'" + dateChosen + "', '" + DayName + "', ");

		//construct headings and values for an update query instead of insert query
		if (alreadyThere)
		{
			//populate strings only from the grid
			for (int i = 0; i < displayGrid->RowCount - 1; ++i)
			{
                updateHeading = StringReplace(displayGrid->Cells[0][i+1], " ", "_", TReplaceFlags() << rfReplaceAll);

				if (i + 1 == displayGrid->RowCount - 1)
				{
					update += (updateHeading + " = '" + displayGrid->Cells[1][i+1] + "'");
				}
				else
				{
					update += (updateHeading + " = '" + displayGrid->Cells[1][i+1] + "', ");
				}
			}
		}
		//check if basic input was used, if so populate headings/values from inputObject
		else if (Form1->getInputLevel() == 0)
		{
			//populate rest of strings from grid
			for (int i = 0; i < inputObject.size; ++i)
			{
				if (i + 1 == inputObject.size)
				{
					headings += ("" + inputObject.valueMap[i][0] + "");
					values += ("'" + displayGrid->Cells[1][i+1] + "'");
				}
				else
				{
					headings += ("" + inputObject.valueMap[i][0] + ", ");
					values += ("'" + displayGrid->Cells[1][i+1] + "', ");
				}
			}
		}
		//if inputLevel is not basic, get both headings and values from the grid
		else
		{
			//populate strings only from the grid (might not be "minus 1")
			for (int i = 0; i < displayGrid->RowCount - 1; ++i)
			{
				if (i + 1 == displayGrid->RowCount - 1)
				{
					headings += ("" + StringReplace(displayGrid->Cells[0][i+1], " ", "_", TReplaceFlags() << rfReplaceAll) + "");
					values += ("'" + displayGrid->Cells[1][i+1] + "'");
				}
				else
				{
					headings += ("" + StringReplace(displayGrid->Cells[0][i+1], " ", "_", TReplaceFlags() << rfReplaceAll) + ", ");
					values += ("'" + displayGrid->Cells[1][i+1] + "', ");
				}
			}
		}

		//run update query if item already in database
		if (alreadyThere)
		{
			SQLQuery2->SQL->Text = "UPDATE baldwins_hotel_data."+inputTable+" SET "+update+" WHERE "+inputTable+".Date = '"+dateChosen+"';";
        	SQLQuery2->ExecSQL();
		}
		//if item not there, then run the normal insert query
		else
		{
			//query input_table db and insert new row
			SQLQuery2->SQL->Text = "INSERT INTO baldwins_hotel_data."+inputTable+" ("+headings+") VALUES ("+values+");";
			SQLQuery2->ExecSQL();

			//query read_table db and insert new row
			SQLQuery2->SQL->Text = "INSERT INTO baldwins_hotel_data."+readTable+" ("+headings+") VALUES ("+values+");";
			SQLQuery2->ExecSQL();
        }

		//Take back to welcome screen (form2)
		Form3->Hide();
		Form2->Show();
	}
	else
	{
		//no selected, save nothing and show form again (do nothing)
	}

	//hide unecessary items
	displayGrid->Visible = false;
	submitButton->Visible = false;
	backImageButton->Visible = false;
}
//---------------------------------------------------------------------------

//check if enter button is pressed from inside dbFieldEdit
void __fastcall TForm3::dbFieldEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
	//if enter button pressed, click next button
	if (Key == 13)
	{
     	nextImageButton->OnClick(NULL);
	}
}
//---------------------------------------------------------------------------

//find out which date is chosen and then progress like normal depending on inputLevel
void __fastcall TForm3::datePopupBoxChange(TObject *Sender)
{
	//find out the item chosen as the date
	int temp = datePopupBox->ItemIndex;
	pureDate = StrToDate(datePopupBox->Items->operator [](temp));
	dateChosen = StrToDate(datePopupBox->Items->operator [](temp)).FormatString(L"yyyy-mm-dd");
}
//---------------------------------------------------------------------------

void __fastcall TForm3::chooseDateImageButtonClick(TObject *Sender)
{
	//hide date related items
	chooseDateImageButton->Visible = false;
	datePopupBoxLabel->Visible = false;
	datePopupBox->Visible = false;

	//at this point dateChosen has whatever date is desired
	//need to now do normal basic or advanced input depending on accessLevel

	//query input_table to see if dateChosen is filled already or not
	//if filled, show advanced view with data filled in
	//if not filled, show basic view and allow normal progression

	//check if current date already input for most basic user
	String currentHotelID = Form1->getHotelID();
	SQLQuery2->SQL->Text = "SELECT input_table, read_table FROM hotel_ref WHERE hotelID = '"+currentHotelID+"';";
	SQLQuery2->Open();
	SQLQuery2->First();

	if (!SQLQuery2->Eof)
	{
		//get what the inputTable is and what read table is (for future insert)
		inputTable = SQLQuery2->Fields->Fields[0]->AsString;
		readTable = SQLQuery2->Fields->Fields[1]->AsString;

		//query to see if tuple exists for dateChosen
		SQLQuery2->SQL->Text = "SELECT * FROM "+inputTable+" WHERE Date = '"+dateChosen+"';";
		SQLQuery2->Open();
		SQLQuery2->First();

		//if tuple exists, display in grid similar to advanced view
		if (!SQLQuery2->Eof)
		{
			//hide form items
			dbFieldLabel->Visible = false;
			dbFieldEdit->Visible = false;
			nextImageButton->Visible = false;

			SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_hotel_data' AND TABLE_NAME = '"+inputTable+"';";
            SQLQuery3->SQL->Text = "SELECT * FROM "+inputTable+" WHERE Date = '"+dateChosen+"';";

			//open query and temporarily skip first two column headings (Date/Day_Of_Week)
			SQLQuery2->Open();
			SQLQuery2->First();
			SQLQuery2->Next();
			SQLQuery2->Next();

			//open second query
			SQLQuery3->Open();
			SQLQuery3->First();

			displayGrid->Cells[0][0] = "Heading";
			displayGrid->Cells[1][0] = "Value";
			int count = 1;
			//strings used in if for holdling column headings
			String originalHeading = "";
			String editedHeading = "";

			while (!SQLQuery2->Eof)
			{
				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

				//replace underscores with spaces and set label
				editedHeading = StringReplace(originalHeading, "_", " ",
					TReplaceFlags() << rfReplaceAll);

				//show and populate displayGrid
				displayGrid->Cells[0][count] = editedHeading;
				displayGrid->Cells[1][count] = SQLQuery3->Fields->Fields[count+1]->AsString;

				//increase count and cursors
				++count;
				SQLQuery2->Next();
			}

			displayGrid->RowCount = count;
			displayGrid->Visible = true;
			submitButton->Visible = true;

			//set alreadyThere boolean to true (signifying the need for an update query when submit is clicked)
			alreadyThere = true;
		}
		//if inputLevel is advanced and no input for this date, show blank advanced view
		else if (Form1->getInputLevel() == 1)
		{
			//hide form items
			dbFieldLabel->Visible = false;
			dbFieldEdit->Visible = false;
			nextImageButton->Visible = false;

			SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_hotel_data' AND TABLE_NAME = '"+inputTable+"';";

			//open query and temporarily skip first two column headings (Date/Day_Of_Week)
			SQLQuery2->Open();
			SQLQuery2->First();
			SQLQuery2->Next();
			SQLQuery2->Next();

			displayGrid->Cells[0][0] = "Heading";
			displayGrid->Cells[1][0] = "Value";
			int count = 1;
			//strings used in if for holdling column headings
			String originalHeading = "";
			String editedHeading = "";

			while (!SQLQuery2->Eof)
			{
				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

				//replace underscores with spaces and set label
				editedHeading = StringReplace(originalHeading, "_", " ",
					TReplaceFlags() << rfReplaceAll);

				//show and populate displayGrid
				displayGrid->Cells[0][count] = editedHeading;
				displayGrid->Cells[1][count] = "";

				++count;
				SQLQuery2->Next();
			}

			displayGrid->RowCount = count;
			displayGrid->Visible = true;
			submitButton->Visible = true;
		}
		//otherwise, run basic input for this chosenDate
		else
		{
			//hide/show uneccessary/necessary items from this page
			dbFieldLabel->Visible = true;
			dbFieldEdit->Visible = true;
			nextImageButton->Visible = true;
			chooseDateImageButton->Visible = false;
			datePopupBox->Visible = false;
			datePopupBoxLabel->Visible = false;

         	//input level is "default", query db schema
			SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_hotel_data' AND TABLE_NAME = '"+inputTable+"';";

			//open query and temporarily skip first two column headings
			SQLQuery2->Open();
			SQLQuery2->First();
			SQLQuery2->Next();
			SQLQuery2->Next();

			//strings used in if for holdling column headings
			String originalHeading = "";
			String editedHeading = "";

			//set label to represent first editable column heading
			if(!SQLQuery2->Eof)
			{
				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

				//replace underscores with spaces and set label
				editedHeading = StringReplace(originalHeading, "_", " ",
					TReplaceFlags() << rfReplaceAll);
				dbFieldLabel->Text = editedHeading;
				dbFieldLabel->Visible = true;
				dbFieldEdit->Visible = true;
				dbFieldEdit->Text = "";
				nextImageButton->Visible = true;

				//initially set index and size to zero
				inputObject.currentIndex = 0;
				inputObject.size = 0;
			}
		}
	}

	//clear datePopupBox items (need to do in other places also)
	datePopupBox->Items->Clear();
}
//---------------------------------------------------------------------------


