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

//computes the week number based on a date parameter (does not line up with calendar week numbers)
typedef TDayTable* PDayTable;

TDayTable MonthDays[2] = {
{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

Word CDayMap[7] = {7, 1, 2, 3, 4, 5, 6};

const int DayMonday = 1;
const int DayTuesday = 2;
const int DayWednesday = 3;
const int DayThursday = 4;
const int DayFriday = 5;
const int DaySaturday = 6;
const int DaySunday = 7;

void __fastcall DivMod(int Dividend, Word Divisor, Word &Result, Word &Remainder)
{
Result = (Word) (Dividend / Divisor);
Remainder = (Word) (Dividend % Divisor);
}

Word __fastcall DayOfTheWeek(const TDateTime &AValue)
{
return ((DateTimeToTimeStamp(AValue).Date - 1) % 7) + 1;
}

void __fastcall DecodeDateWeek(const TDateTime &AValue, Word &AYear,
Word &AWeekOfYear, Word &ADayOfWeek)
{
int LDayOfYear;
Word LMonth, LDay;
TDateTime LStart;
Word LStartDayOfWeek, LEndDayOfWeek;
bool LLeap;

LLeap = DecodeDateFully(AValue, AYear, LMonth, LDay, ADayOfWeek);
ADayOfWeek = CDayMap[ADayOfWeek-1];
LStart = EncodeDate(AYear, 1, 1);
LDayOfYear = (int)(AValue - LStart + 1);
LStartDayOfWeek = DayOfTheWeek(LStart);
if( (LStartDayOfWeek >= DayFriday) && (LStartDayOfWeek <=
DaySunday) )
LDayOfYear -= (8 - LStartDayOfWeek);
else
LDayOfYear += (LStartDayOfWeek - 1);
if( LDayOfYear <= 0 )
DecodeDateWeek(LStart - 1, AYear, AWeekOfYear, LDay);
else
{
AWeekOfYear = (LDayOfYear / 7);
if( (LDayOfYear % 7) != 0 )
++AWeekOfYear;
if( AWeekOfYear > 52 )
{
LEndDayOfWeek = LStartDayOfWeek;
if( LLeap )
{
if( LEndDayOfWeek == DaySunday )
LEndDayOfWeek = DayMonday;
else
++LEndDayOfWeek;
}
if( (LEndDayOfWeek >= DayMonday) && (LEndDayOfWeek <=
DayWednesday) )
{
++AYear;
AWeekOfYear = 1;
}
}
}
}
//end find week number stuff

Word __fastcall WeekOfTheYear(const TDateTime &AValue)
{
Word LYear, LDOW, LResult;
DecodeDateWeek(AValue, LYear, LResult, LDOW);
return LResult;
}

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

			//depending on viewType, allow user to choose day, week, month or year respectively
			if (viewType == "Day")
			{
				//show calendar and allow user to choose a day
				dayCalendar->Visible = true;
			}
			else if (viewType == "Week")
			{
				//show calendar with week numbers on side
				dayCalendar->Visible = true;
			}
			else if (viewType == "Month")
			{
				//show monthPopupBox with months as items
				monthPopupBox->Visible = true;
			}
			else
			{
				//show yearPopupBox with years as items
				yearPopupBox->Visible = true;
            }

			break;
		//basic show
		case 2:
			//increase basicState and hide things
			basicState++;
			filtersLabel->Visible = false;
			backImageButton2->Visible = false;
			nextImageButton2->Visible = false;
			dayCalendar->Visible = false;
			monthPopupBox->Visible = false;
			yearPopupBox->Visible = false;

			//get whatever data was chosen by user (i.e. day, week, month or year)
			if (viewType == "Day")
			{
				//get the date selected from the calendar
				TDateTime dayChosen = dayCalendar->Date;
				String dbDayChosen = StrToDate(dayChosen).FormatString(L"yyyy-mm-dd");

				//testing
				filtersLabel->Text = dbDayChosen;
				filtersLabel->Visible = true;

				//call display function
			}
			else if (viewType == "Week")
			{
				//get the week selected by the day selected from the calendar
				TDateTime dayChosen = dayCalendar->Date;
				String dbDayChosen = StrToDate(dayChosen).FormatString(L"yyyy-mm-dd");
				int weekNum = WeekOfTheYear(dayChosen);
				//need to check if weekNum == 1 (could either be first week 1 of current year or next year)
				//just need to pull the year from the dayChosen

				//testing
				filtersLabel->Text = weekNum;
				filtersLabel->Visible = true;
			}
			else if (viewType == "Month")
			{
				//get the month selected from the drop down
			}
			else
			{
				//get the year selected from the drop down
			}

			//call and give parameters (roleVector, viewBy, day/week/month/year) to display desired data

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
			dayCalendar->Visible = false;
			dayCalendar->WeekNumbers = false;
			monthPopupBox->Visible = false;
			yearPopupBox->Visible = false;

            //handle signature things for view filter
			dayRadio->Visible = true;
			weekRadio->Visible = true;
			monthRadio->Visible = true;
			yearRadio->Visible = true;
			break;
	}
}
//---------------------------------------------------------------------------

//selects or deselects all items in the list
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

//ensures that the text of the selectAllButton stays correct and intuitive
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



