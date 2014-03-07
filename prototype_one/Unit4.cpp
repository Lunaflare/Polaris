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

//calculate what a value is as a percent to one decimal point
float __fastcall TForm4::makePercent(float value)
{
	/*if (value < 1)
		return value;
	else*/
		return floor(value * 100.0 * 10.0 + 0.5)/10.0;
}

//round a decimal value to 2 places
float __fastcall TForm4::roundTwo(float value)
{
	/*if (value < 1)
		return value;
	else*/
		return floor(value * 100.0 + 0.5)/100.0;
}

int __fastcall TForm4::nearestDollar(float value)
{
	return floor(value +.5);
}

string __fastcall TForm4::IntToStr(long int n)
{
	ostringstream result;
	result << n;
	return result.str();
}

String __fastcall TForm4::commas(string num)
{
	string temp;
	int endstring = num.length(), i;
	for( i = endstring - 3; i >= 0; i -= 3)
	{
		if (i > 0)
		{
			temp = ","+ num.substr(i, 3) + temp;
		} else
		{
			temp = num.substr(i, 3) + temp;
		}

	}
	if (i < 0)
	{
		temp = num.substr(0, 3+i) + temp;
	}

    return String(temp.c_str());
}

double __fastcall TForm4::toDouble(String gridString)
{
	gridString = StringReplace(gridString, ",", "", TReplaceFlags() << rfReplaceAll);
	string temp = AnsiString(StringReplace(gridString, "$", "", TReplaceFlags() << rfReplaceAll)).c_str();

	return atof(temp.c_str());
}

//populate grid for when viewing by month, week, year, or arbitrary date range
void __fastcall TForm4::populateGrid(vector<String> rVector, int monthChosen, String rangeStart, String rangeEnd, String type)
{
	//hide columns that will not be used
	for (int i = readGrid->ColumnCount - 1; i > 2; --i)
		readGrid->ColumnByIndex(i)->Visible = true;

	//set rowCount to reflect the number of roles we will be adding (just added end add need to get the number of roles dynamically)
	readGrid->RowCount = ((readGrid->RowCount + 5 * rVector.size()) - 3) + (11 - rVector.size());

	String hoursPaid = "";
	String standardHours = "";
	String percentPerformance = "";
	String withUnderscores = "";
	String select = "";
	int columnIndex = 0;

	//add items to select that will be required no matter what
	String firstColumnHeadings[9] = {"Date", "Day_Of_Week", "SUM(Offset_Rooms_Occupied)", "SUM(AM_Rooms_Cleaned)", "SUM(PM_Rooms_Cleaned)", "SUM(Rooms_Sold)", "SUM(Total_Rooms_Cleaned)", "SUM(Guestroom_Carpets_Cleaned)", "SUM(Documented_Inspections_Completed)"};
	for (int i = 0; i < 9; ++i)
	{
		if (i + 1 == 9)
			select += firstColumnHeadings[i];
		else
			select += firstColumnHeadings[i] + ", ";
	}

	//used for productivity goal computations
	int productivityAMIndex = 0;
	int productivityPMIndex = 0;
	int productivityLaundryIndex = 0;
	String spaceTaker = "";

	//construct select query
	for (int i = 0; i < rVector.size(); ++i)
	{
		//adjust to create three different strings used in query
		withUnderscores = StringReplace(rVector[i], " ", "_", TReplaceFlags() << rfReplaceAll);
		hoursPaid = withUnderscores + "_Hours_Paid";
		standardHours = withUnderscores + "_Standard_Hours";
		percentPerformance = withUnderscores + "_Percent_Performance";

		//check the index for productivity goal
		if (SameText(standardHours, "AM_Room_Attendants_Standard_Hours"))
			productivityAMIndex = 8 + 3 * i + 2;
		else if (SameText(standardHours, "PM_Room_Attendants_Standard_Hours"))
			productivityPMIndex = 8 + 3 * i + 2;
		else if (SameText(standardHours, "Laundry_Attendant_Standard_Hours"))
			productivityLaundryIndex = 8 + 3 * i + 2;

		//append necessary strings to query
		select += (", SUM(" + hoursPaid + "), SUM(" + standardHours + "), SUM(" + percentPerformance + ")");

		//populate column zero with role name headings
		columnIndex = 10 + i * 4;
		readGrid->Cells[0][columnIndex] = rVector[i];
	}

	//add more static items to column zero headings
	String nonRoleHeadings[3] = {"Overtime Premium Cost", "Total Labor Hours", "Total Labor Cost"};
	for (int i = 0; i < 3; ++i)
	{
		columnIndex += 4;
		readGrid->Cells[0][columnIndex] = nonRoleHeadings[i];
	}

	//add more static items to column zero headings
	columnIndex += 2;
	String productivityHeadings[6] = {"Hours Variance (Act. minus Std.)", "Cost Variance (Act. Minus Std.)", "Man Minutes per Room Cleaned", "Rooms Cleaned per AM GRA", "Rooms Cleaned per PM GRA", "Rooms per Laundry Attendant"};
	for (int i = 0; i < 6; ++i)
	{
		columnIndex++;
		readGrid->Cells[0][columnIndex] = productivityHeadings[i];
	}

	//add more items to select that will be required no matter what
	select += ", SUM(Overtime_Hours_Paid) ,  SUM(Overtime_Standard_Hours) , SUM(Overtime_Percent_Performance)";
	select += ", SUM(Total_Labor_Hours_Hours_Paid) , SUM(Total_Labor_Hours_Standard_Hours) , SUM(Total_Labor_Hours_Percent_Performance)";
	select += ", SUM(Total_Labor_Cost_Hours_Paid) ,  SUM(Total_Labor_Cost_Standard_Hours) , SUM(Total_Labor_Cost_Percent_Performance)";

	//construct actual query
	if (SameText(type, "month"))
	{
		Form3->SQLQuery2->SQL->Text = "SELECT " + select + " FROM baldwins_hotel_data." + readTable + " WHERE MONTH(Date) = '" + monthChosen + "' GROUP BY Day_Of_Week;";
		Form3->SQLQuery2->Open();
		Form3->SQLQuery2->First();
	}
	else if (SameText(type, "year"))
	{
		Form3->SQLQuery2->SQL->Text = "SELECT " + select + " FROM baldwins_hotel_data." + readTable + " WHERE YEAR(Date) = '" + monthChosen + "' GROUP BY Day_Of_Week;";
		Form3->SQLQuery2->Open();
		Form3->SQLQuery2->First();
	}
	else if (SameText(type, "week"))
	{
		Form3->SQLQuery2->SQL->Text = "SELECT " + select + " FROM baldwins_hotel_data." + readTable + " WHERE Date BETWEEN '" + rangeStart + "' AND '" + rangeEnd + "' GROUP BY Day_Of_Week;";
		Form3->SQLQuery2->Open();
		Form3->SQLQuery2->First();
    }

	//start filling column 1
	String firstColumnNoDayWeek[8] = {"Offset_Rooms_Occupied", "Occupancy_Percent", "AM_Rooms_Cleaned", "PM_Rooms_Cleaned", "Rooms_Sold", "Total_Rooms_Cleaned", "Guestroom_Carpets_Cleaned", "Documented_Inspections"};
	for (int i = 0; i < 8; ++i)
		readGrid->Cells[1][i] = StringReplace(firstColumnNoDayWeek[i], "_", " ", TReplaceFlags() << rfReplaceAll);

	//continue filling column 1 with headings
	String roleTypes[3] = {"Actual Hours", "Standard Hours", "% Performance"};
	columnIndex = 9;
	for (int i = 0; i < rVector.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			readGrid->Cells[1][columnIndex++] = roleTypes[j];
		}

		++columnIndex;
	}

	//continue filling column 1 with headers
	String overtimeHeaders[3] = {"Overtime Hours", "Overtime Cost", "Overtime Premium Cost"};
	for (int i = 0; i < 3; ++i)
	{
       	readGrid->Cells[1][columnIndex++] = overtimeHeaders[i];
	}
	++columnIndex;

	//continue filling column 1 with headers
	for (int i = 0; i < 3; ++i)
	{
		readGrid->Cells[1][columnIndex++] = roleTypes[i];
	}
	++columnIndex;

	//continue filling column 1 with headers
	for (int i = 0; i < 3; ++i)
	{
		readGrid->Cells[1][columnIndex++] = StringReplace(roleTypes[i], "Hours", "Cost", TReplaceFlags() << rfReplaceAll);
	}
	readGrid->Cells[1][columnIndex++] = "Productivity Goals";

	//get productivity index to be used later
    int productivityStartIndex = columnIndex;

	//create index indicating which column I am currently filling
	int indexOn = 2;

	//iterate through cursor until empty
	columnIndex = 0;
	String blank = "";

	//index indicating where in query cursor you currently are (i.e. which field)
	//just hardcoded for now but could be size of a return of another query
	int queryIndex = 9;

	//running calculations for productivity goals
	double manMinutesRoomsCleaned = 0;
	int roomsCleanedAM = 0;
	int roomsCleanedPM = 0;
	int roomsCleanedAMSingleInstance = 0;
	int roomsCleanedPMSingleInstance = 0;
	double productivityRoomsCleanedAM = 0;
	double productivityRoomsCleanedPM = 0;
	double productivityLaundry = 0;
	double productivityNonGoalAM = 0;
	double productivityNonGoalPM = 0;
	double productivityNonGoalLaundry = 0;
	int totalRoomsCleaned = 0;
	int totalRoomsCleanedSingleInstance = 0;
	double hoursVarianceOne = 0;
	double hoursVarianceTwo = 0;
	double costVarianceOne = 0;
	double costVarianceTwo = 0;

	while (!Form3->SQLQuery2->Eof)
	{
		//set indexOn so that week days appear in correct order (may need to do something in case a specific week day is not returned)
		if (SameText(Form3->SQLQuery2->Fields->Fields[1]->AsString, "Saturday"))
			indexOn = 2;
		else if (SameText(Form3->SQLQuery2->Fields->Fields[1]->AsString, "Sunday"))
			indexOn = 3;
		else if (SameText(Form3->SQLQuery2->Fields->Fields[1]->AsString, "Monday"))
			indexOn = 4;
		else if (SameText(Form3->SQLQuery2->Fields->Fields[1]->AsString, "Tuesday"))
			indexOn = 5;
		else if (SameText(Form3->SQLQuery2->Fields->Fields[1]->AsString, "Wednesday"))
			indexOn = 6;
		else if (SameText(Form3->SQLQuery2->Fields->Fields[1]->AsString, "Thursday"))
			indexOn = 7;
		else if (SameText(Form3->SQLQuery2->Fields->Fields[1]->AsString, "Friday"))
			indexOn = 8;

		//populate the header for this column with the day of the week
		readGrid->ColumnByIndex(indexOn)->Header = Form3->SQLQuery2->Fields->Fields[1]->AsString;

		//continue populating items
		//readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[0]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[2]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[2]->AsFloat / 310.0) + " %";

		//fill with rest of non role type stuff
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[3]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[4]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[5]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[6]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[7]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[8]->AsString;

		//fill with role items
		++columnIndex;
		for (int i = 0; i < rVector.size(); ++i)
		{
			//need to round when appropriate and make it a percentage
			for (int j = 0; j < 3; ++j)
			{
				//if j == 2, these should be percents
				if (j == 2)
				{
					if (Form3->SQLQuery2->Fields->Fields[queryIndex-2]->AsFloat != 0)
						readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[queryIndex-1]->AsFloat / Form3->SQLQuery2->Fields->Fields[queryIndex-2]->AsFloat) + " %";
					else
						readGrid->Cells[indexOn][columnIndex++] = "0%";
					++queryIndex;
				}
				else if (j == 1)
				{
					//compute productivity man hours
					manMinutesRoomsCleaned += Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					//compute productivity rooms cleaned AM divisor
					if (queryIndex == productivityAMIndex)
						productivityRoomsCleanedAM += Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (queryIndex == productivityPMIndex)
						productivityRoomsCleanedPM += Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (queryIndex == productivityLaundryIndex)
						productivityLaundry += Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					readGrid->Cells[indexOn][columnIndex++] = roundTwo(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
				}
				else
				{
					if (queryIndex == (productivityAMIndex - 1))
						productivityNonGoalAM = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (queryIndex == (productivityPMIndex - 1))
						productivityNonGoalPM = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (queryIndex == (productivityLaundryIndex - 1))
						productivityNonGoalLaundry = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					readGrid->Cells[indexOn][columnIndex++] = roundTwo(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
				}
			}

			//skip spaces when appropriate
			++columnIndex;
		}

		//fill with overtime, total labor hours, total labor cost
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (i == 0)
				{
					if (j == 0)
						readGrid->Cells[indexOn][columnIndex++] = roundTwo(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
					else
                        readGrid->Cells[indexOn][columnIndex++] = blank + "$" + nearestDollar(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
                }
				else if (i == 1)
				{
					//productivity computation
					if (j == 0)
						hoursVarianceOne = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (j == 1)
                        hoursVarianceTwo = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					if (j == 2)
					{
						if (Form3->SQLQuery2->Fields->Fields[queryIndex-2]->AsFloat != 0)
							readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[queryIndex-1]->AsFloat / Form3->SQLQuery2->Fields->Fields[queryIndex-2]->AsFloat) + " %";
						else
							readGrid->Cells[indexOn][columnIndex++] = "0%";
						++queryIndex;
						//readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat) + " %";
					}
					else
						readGrid->Cells[indexOn][columnIndex++] = nearestDollar(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
				}
				else
				{
					//productivity computation
					if (j == 0)
						costVarianceOne = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (j == 1)
						costVarianceTwo = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					if (j == 2)
					{
						if (Form3->SQLQuery2->Fields->Fields[queryIndex-2]->AsFloat != 0)
							readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[queryIndex-1]->AsFloat / Form3->SQLQuery2->Fields->Fields[queryIndex-2]->AsFloat) + " %";
						else
							readGrid->Cells[indexOn][columnIndex++] = "0%";
						//readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat) + " %";
						++queryIndex;
					}
					else
						readGrid->Cells[indexOn][columnIndex++] = blank + "$" + commas(IntToStr(nearestDollar(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat)));
				}
			}

			//skip spaces when appropriate
			++columnIndex;
		}

		//keep running total for productivity goal computation
		totalRoomsCleaned += Form3->SQLQuery2->Fields->Fields[6]->AsInteger;
		totalRoomsCleanedSingleInstance = Form3->SQLQuery2->Fields->Fields[6]->AsInteger;
		roomsCleanedAM += Form3->SQLQuery2->Fields->Fields[3]->AsInteger;
		roomsCleanedAMSingleInstance = Form3->SQLQuery2->Fields->Fields[3]->AsInteger;
		roomsCleanedPM += Form3->SQLQuery2->Fields->Fields[4]->AsInteger;
		roomsCleanedPMSingleInstance = Form3->SQLQuery2->Fields->Fields[4]->AsInteger;

		//fill in all productivity things for each iteration of cursor
		readGrid->Cells[indexOn][columnIndex++] = nearestDollar(hoursVarianceOne - hoursVarianceTwo);
		readGrid->Cells[indexOn][columnIndex++] = nearestDollar(costVarianceOne - costVarianceTwo);
		if (totalRoomsCleanedSingleInstance == 0)
			readGrid->Cells[indexOn][columnIndex++] = 0;
		else
			readGrid->Cells[indexOn][columnIndex++] = roundTwo((hoursVarianceOne * 60.0) / totalRoomsCleanedSingleInstance);
		productivityNonGoalAM /= 8.0;
		if (productivityNonGoalAM == 0)
			readGrid->Cells[indexOn][columnIndex++] = 0;
		else
			readGrid->Cells[indexOn][columnIndex++] = roundTwo(((double) roomsCleanedAMSingleInstance) / productivityNonGoalAM);
		productivityNonGoalPM /= 8.0;
		if (productivityNonGoalPM == 0)
			readGrid->Cells[indexOn][columnIndex++] = 0;
		else
			readGrid->Cells[indexOn][columnIndex++] = roundTwo(((double) roomsCleanedPMSingleInstance) / productivityNonGoalPM);
		productivityNonGoalLaundry /= 7.5;
		if (productivityNonGoalLaundry == 0)
			readGrid->Cells[indexOn][columnIndex++] = 0;
		else
			readGrid->Cells[indexOn][columnIndex++] = roundTwo(((double) totalRoomsCleanedSingleInstance) / productivityNonGoalLaundry);

		//advance cursor (for daily, should only be one iteration of cursor)
		queryIndex = 9;
		columnIndex = 0;
		Form3->SQLQuery2->Next();

		//put some counters back to zero
		/*manMinutesRoomsCleaned
		roomsCleanedPM
		roomsCleanedAM
		totalRoomsCleaned
		productivityLaundry*/
	}

	//get what the productivity column is (probably ok to stay hardcoded)
	int productivityColumn = 1;

	//change first two productivity things to 0.00
	readGrid->Cells[productivityColumn][productivityStartIndex++] = "0.00";
	readGrid->Cells[productivityColumn][productivityStartIndex++] = "0.00";

	//compute productivity for man hours
	if (totalRoomsCleaned != 0)
		manMinutesRoomsCleaned = roundTwo((manMinutesRoomsCleaned * 60.0) / totalRoomsCleaned);
	else
		manMinutesRoomsCleaned = 0;
	readGrid->Cells[productivityColumn][productivityStartIndex++] = manMinutesRoomsCleaned;

	//compute productivity for roomsCleanedAM
	productivityRoomsCleanedAM /= 8.0;
	if (productivityRoomsCleanedAM == 0)
		productivityRoomsCleanedAM = 0;
	else
		productivityRoomsCleanedAM = roundTwo(((double) roomsCleanedAM) / productivityRoomsCleanedAM);
	readGrid->Cells[productivityColumn][productivityStartIndex++] = productivityRoomsCleanedAM;

	//compute productivity for roomsCleanedPM
	productivityRoomsCleanedPM /= 8.0;
	if (productivityRoomsCleanedPM == 0)
		productivityRoomsCleanedPM = 0;
	else
		productivityRoomsCleanedPM = roundTwo(((double) roomsCleanedPM) / productivityRoomsCleanedPM);
	readGrid->Cells[productivityColumn][productivityStartIndex++] = productivityRoomsCleanedPM;

	//compute productivity for laundryAttendantProductivity
	productivityLaundry /= 7.5;
	if (productivityLaundry == 0)
		productivityLaundry = 0;
	else
		productivityLaundry = roundTwo(((double) totalRoomsCleaned) / productivityLaundry);
	readGrid->Cells[productivityColumn][productivityStartIndex++] = productivityLaundry;

	//populate total column (last one)
	indexOn = 9;
	columnIndex = 0;
	if (SameText(type, "month"))
	{
		//get the month name for the header
		String DayName = "";
		switch(monthChosen)
		{
			case 1:
				DayName = L"January";
				break;
			case 2:
				DayName = L"February";
				break;
			case 3:
				DayName = L"March";
				break;
			case 4:
				DayName = L"April";
				break;
			case 5:
				DayName = L"May";
				break;
			case 6:
				DayName = L"June";
				break;
			case 7:
				DayName = L"July";
				break;
			case 8:
				DayName = L"August";
				break;
			case 9:
				DayName = L"September";
				break;
			case 10:
				DayName = L"October";
				break;
			case 11:
				DayName = L"November";
				break;
			case 12:
				DayName = L"December";
				break;
		}

		//make the header the month
		readGrid->ColumnByIndex(indexOn)->Header = DayName;
	}
	else if (SameText(type, "year"))
	{
		//make the header the year (monthChosen actuall stores the year in this case)
		readGrid->ColumnByIndex(indexOn)->Header = monthChosen;
	}
	else if (SameText(type, "week"))
	{
		//make the header the date range
        readGrid->ColumnByIndex(indexOn)->Header = privateDayChosenStart + " - " + privateDayChosenEnd;
	}

	//fill with top portion (non roles) with totals
	double totalCounter = 0;
	double occupancyPercent = 0;
	double totalRoomsCleanedTotal = 0;
	double amRoomsCleanedTotal = 0;
	double pmRoomsCleanedTotal = 0;
	double offsetRoomsOccupiedTotal = 0;
	for (columnIndex = 0; columnIndex < 8; ++columnIndex)
	{
		for (int j = 2; j < 9; ++j)
			totalCounter += toDouble(readGrid->Cells[j][columnIndex]);

		if (columnIndex == 0)
			occupancyPercent = totalCounter / 8680.0;

		if (columnIndex == 1)
			readGrid->Cells[indexOn][columnIndex] = blank + makePercent(occupancyPercent) + "%";
		else
			readGrid->Cells[indexOn][columnIndex] = totalCounter;
		totalCounter = 0;

		//get some values for productivity calculations at bottom
		if (SameText(readGrid->Cells[1][columnIndex], "Total Rooms Cleaned"))
			totalRoomsCleanedTotal = toDouble(readGrid->Cells[indexOn][columnIndex]);
		else if (SameText(readGrid->Cells[1][columnIndex], "AM Rooms Cleaned"))
			amRoomsCleanedTotal = toDouble(readGrid->Cells[indexOn][columnIndex]);
		else if (SameText(readGrid->Cells[1][columnIndex], "PM Rooms Cleaned"))
			pmRoomsCleanedTotal = toDouble(readGrid->Cells[indexOn][columnIndex]);
		else if (SameText(readGrid->Cells[1][columnIndex], "Offset Rooms Occupied"))
			offsetRoomsOccupiedTotal = toDouble(readGrid->Cells[indexOn][columnIndex]);
	}
	//fill rest of totals besides productivity (i.e. roles, overtime, totla labor hours, total labor cost)
	int modThree = 0;
	double amRoomAttendantsTotal = 0;
	double pmRoomAttendantsTotal = 0;
	double laundryAttendantTotal = 0;
	double actualCostTotal = 0;
	for (columnIndex; columnIndex < readGrid->RowCount - 7; ++columnIndex)
	{
		for (int j = 2; j < 9; ++j)
		{
			totalCounter += toDouble(readGrid->Cells[j][columnIndex]);
		}

		if (!SameText(readGrid->Cells[1][columnIndex], ""))
		{
			//increase counter
			++modThree;

			//fill grid with computed value, if counter multiple of 3 then treat as percent
			if (SameText(readGrid->Cells[1][columnIndex], "Overtime Premium Cost") || SameText(readGrid->Cells[1][columnIndex], "Overtime Cost"))
				readGrid->Cells[indexOn][columnIndex] = blank + "$" + totalCounter;
			else if (SameText(readGrid->Cells[1][columnIndex], "Actual Cost") || SameText(readGrid->Cells[1][columnIndex], "Standard Cost"))
				readGrid->Cells[indexOn][columnIndex] = blank + "$" + commas(IntToStr(nearestDollar(totalCounter)));
			else if (modThree % 3 == 0)
				if (readGrid->Cells[indexOn][columnIndex-2] != 0)
					readGrid->Cells[indexOn][columnIndex] = blank + makePercent(readGrid->Cells[indexOn][columnIndex-1] / readGrid->Cells[indexOn][columnIndex-2]) + "%";
				else
                   	readGrid->Cells[indexOn][columnIndex] = "0%";
			else
				readGrid->Cells[indexOn][columnIndex] = totalCounter;
		}

		//get some values for productivity calculations at bottom
		if (SameText(readGrid->Cells[0][columnIndex], "AM Room Attendants"))
			amRoomAttendantsTotal = toDouble(readGrid->Cells[indexOn][columnIndex-1]);
		else if (SameText(readGrid->Cells[0][columnIndex], "PM Room Attendants"))
			pmRoomAttendantsTotal = toDouble(readGrid->Cells[indexOn][columnIndex-1]);
		else if (SameText(readGrid->Cells[0][columnIndex], "Laundry Attendant"))
			laundryAttendantTotal = toDouble(readGrid->Cells[indexOn][columnIndex-1]);
		else if (SameText(readGrid->Cells[1][columnIndex], "Actual Cost"))
			actualCostTotal = toDouble(readGrid->Cells[indexOn][columnIndex]);

		totalCounter = 0;
	}
	//increase columnIndex for productivity totals
	++columnIndex;

	//fill productivity for hours variance
	if (readGrid->Cells[indexOn][columnIndex - 8] == 0)
		readGrid->Cells[indexOn][columnIndex++] = 0;
	else
		readGrid->Cells[indexOn][columnIndex++] = readGrid->Cells[indexOn][columnIndex-8] - readGrid->Cells[indexOn][columnIndex-7];

	//fill productivity for cost variance
	if (readGrid->Cells[indexOn][columnIndex - 8] == 0)
		readGrid->Cells[indexOn][columnIndex++] = "$0";
	else
		readGrid->Cells[indexOn][columnIndex++] = commas(IntToStr(nearestDollar(toDouble(readGrid->Cells[indexOn][columnIndex-5]) - toDouble(readGrid->Cells[indexOn][columnIndex-4]))));

	//fill productivity for man minutes per room cleaned
	if (readGrid->Cells[indexOn][columnIndex - 10] == 0)
		readGrid->Cells[indexOn][columnIndex++] = 0;
	else
		readGrid->Cells[indexOn][columnIndex++] = roundTwo((readGrid->Cells[indexOn][columnIndex-10] * 60) / totalRoomsCleanedTotal);

	//fill productivity for rooms cleaned per AM gra
	if (amRoomAttendantsTotal == 0)
		readGrid->Cells[indexOn][columnIndex++] = 0;
	else
		readGrid->Cells[indexOn][columnIndex++] = roundTwo(amRoomsCleanedTotal / (amRoomAttendantsTotal / 8.0));

	//fill productivity for rooms cleaned per PM gra
	if (pmRoomAttendantsTotal == 0)
		readGrid->Cells[indexOn][columnIndex++] = 0;
	else
		readGrid->Cells[indexOn][columnIndex++] = roundTwo(pmRoomsCleanedTotal / (pmRoomAttendantsTotal / 8.0));

	//fill productivity for rooms per laundry attendant
	if (laundryAttendantTotal == 0)
		readGrid->Cells[indexOn][columnIndex++] = 0;
	else
		readGrid->Cells[indexOn][columnIndex++] = roundTwo(totalRoomsCleanedTotal / (laundryAttendantTotal / 7.5));

	//fill cells in top left with extra statistics
	readGrid->Cells[0][0] = "$12.96 Budget";
	if (actualCostTotal < 0)
		readGrid->Cells[0][2] = blank + "$" + roundTwo((actualCostTotal * -1) / offsetRoomsOccupiedTotal) + " Payroll cost to clean room";
	else
		readGrid->Cells[0][2] = blank + "$" + roundTwo(actualCostTotal / offsetRoomsOccupiedTotal) + " Payroll cost to clean room";
	if (toDouble(readGrid->Cells[0][2]) < 0)
		readGrid->Cells[0][1] = blank + "$" + roundTwo((toDouble(readGrid->Cells[0][2])*-1) - toDouble(readGrid->Cells[0][0])) + " Over/Under per rooom";
	else
		readGrid->Cells[0][1] = blank + "$" + roundTwo(toDouble(readGrid->Cells[0][2]) - toDouble(readGrid->Cells[0][0])) + " Over/Under per rooom";
	readGrid->Cells[0][3] = "$76,995 Accounting";
	if (toDouble(readGrid->Cells[0][3]) < 0)
		readGrid->Cells[0][4] = blank + "$" + commas(IntToStr(nearestDollar((toDouble(readGrid->Cells[0][3])*-1) - actualCostTotal))) + " Variance";
	else
		readGrid->Cells[0][4] = blank + "$" + commas(IntToStr(nearestDollar(toDouble(readGrid->Cells[0][3]) - actualCostTotal))) + " Variance";

	//make grid visible to user
  	readGrid->Visible = true;
}

//populate grid for when viewing by day
void __fastcall TForm4::populateGrid(vector<String> rVector, String currentDate)
{
	//set rowCount to reflect the number of roles we will be adding
	//readGrid->RowCount = (readGrid->RowCount + 5 * rVector.size()) - 2;
	readGrid->RowCount = ((readGrid->RowCount + 5 * rVector.size()) - 2) + (11 - rVector.size());

	String hoursPaid = "";
	String standardHours = "";
	String percentPerformance = "";
	String withUnderscores = "";
	String select = "";
	int columnIndex = 0;

	//hide columns that will not be used
	for (int i = readGrid->ColumnCount - 1; i > 2; --i)
		readGrid->ColumnByIndex(i)->Visible = false;

	//add items to select that will be required no matter what
	String firstColumnHeadings[9] = {"Date", "Day_Of_Week", "Offset_Rooms_Occupied", "AM_Rooms_Cleaned", "PM_Rooms_Cleaned", "Rooms_Sold", "Total_Rooms_Cleaned", "Guestroom_Carpets_Cleaned", "Documented_Inspections_Completed"};
	for (int i = 0; i < 9; ++i)
	{
		if (i + 1 == 9)
			select += firstColumnHeadings[i];
		else
			select += firstColumnHeadings[i] + ", ";
	}

	//used for productivity goal computations
	int productivityAMIndex = 0;
	int productivityPMIndex = 0;
	int productivityLaundryIndex = 0;

	//construct select query
	for (int i = 0; i < rVector.size(); ++i)
	{
		//adjust to create three different strings used in query
		withUnderscores = StringReplace(rVector[i], " ", "_", TReplaceFlags() << rfReplaceAll);
		hoursPaid = withUnderscores + "_Hours_Paid";
		standardHours = withUnderscores + "_Standard_Hours";
		percentPerformance = withUnderscores + "_Percent_Performance";

		//check the index for productivity goal
		if (SameText(standardHours, "AM_Room_Attendants_Standard_Hours"))
			productivityAMIndex = 8 + 3 * i + 2;
		else if (SameText(standardHours, "PM_Room_Attendants_Standard_Hours"))
			productivityPMIndex = 8 + 3 * i + 2;
		else if (SameText(standardHours, "Laundry_Attendant_Standard_Hours"))
			productivityLaundryIndex = 8 + 3 * i + 2;

		//append necessary strings to query
		select += (", " + hoursPaid + ", " + standardHours + ", " + percentPerformance);

		//populate column zero with role name headings
		columnIndex = 11 + i * 4;
		readGrid->Cells[0][columnIndex] = rVector[i];
	}

	//add more static items to column zero headings
	String nonRoleHeadings[3] = {"Overtime Premium Cost", "Total Labor Hours", "Total Labor Cost"};
	for (int i = 0; i < 3; ++i)
	{
		columnIndex += 4;
		readGrid->Cells[0][columnIndex] = nonRoleHeadings[i];
	}

	//add more static items to column zero headings
	columnIndex += 2;
	String productivityHeadings[6] = {"Hours Variance (Act. minus Std.)", "Cost Variance (Act. Minus Std.)", "Man Minutes per Room Cleaned", "Rooms Cleaned per AM GRA", "Rooms Cleaned per PM GRA", "Rooms per Laundry Attendant"};
	for (int i = 0; i < 6; ++i)
	{
		columnIndex++;
		readGrid->Cells[0][columnIndex] = productivityHeadings[i];
	}

	//add more items to select that will be required no matter what
	select += ", Overtime_Hours_Paid ,  Overtime_Standard_Hours , Overtime_Percent_Performance";
	select += ", Total_Labor_Hours_Hours_Paid ,  Total_Labor_Hours_Standard_Hours , Total_Labor_Hours_Percent_Performance";
	select += ", Total_Labor_Cost_Hours_Paid ,  Total_Labor_Cost_Standard_Hours , Total_Labor_Cost_Percent_Performance";

	//construct actual query
	Form3->SQLQuery2->SQL->Text = "SELECT " + select + " FROM baldwins_hotel_data." + readTable + " WHERE Date = '" + currentDate + "';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();

	//start filling column 1
	String firstColumnNoDayWeek[9] = {"Date", "Offset_Rooms_Occupied", "Occupancy_Percent", "AM_Rooms_Cleaned", "PM_Rooms_Cleaned", "Rooms_Sold", "Total_Rooms_Cleaned", "Guestroom_Carpets_Cleaned", "Documented_Inspections"};
	for (int i = 0; i < 9; ++i)
		readGrid->Cells[1][i] = StringReplace(firstColumnNoDayWeek[i], "_", " ", TReplaceFlags() << rfReplaceAll);

	//continue filling column 1 with headings
	String roleTypes[3] = {"Actual Hours", "Standard Hours", "% Performance"};
	columnIndex = 10;
	for (int i = 0; i < rVector.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			readGrid->Cells[1][columnIndex++] = roleTypes[j];
		}

		++columnIndex;
	}

	//continue filling column 1 with headers
	String overtimeHeaders[3] = {"Overtime Hours", "Overtime Cost", "Overtime Premium Cost"};
	for (int i = 0; i < 3; ++i)
	{
       	readGrid->Cells[1][columnIndex++] = overtimeHeaders[i];
	}
	++columnIndex;

	//continue filling column 1 with headers
	for (int i = 0; i < 3; ++i)
	{
		readGrid->Cells[1][columnIndex++] = roleTypes[i];
	}
	++columnIndex;

	//continue filling column 1 with headers
	for (int i = 0; i < 3; ++i)
	{
		readGrid->Cells[1][columnIndex++] = StringReplace(roleTypes[i], "Hours", "Cost", TReplaceFlags() << rfReplaceAll);
	}
	readGrid->Cells[1][columnIndex++] = "Productivity Goals";

	//get productivity index to be used later
    int productivityStartIndex = columnIndex;

	//create index indicating which column I am currently filling
	int indexOn = 2;

	//iterate through cursor until empty
	columnIndex = 0;
	String blank = "";

	//index indicating where in query cursor you currently are (i.e. which field)
	//just hardcoded for now but could be size of a return of another query
	int queryIndex = 9;

	//running calculations for productivity goals
	double manMinutesRoomsCleaned = 0;
	int roomsCleanedAM = 0;
	int roomsCleanedPM = 0;
	int roomsCleanedAMSingleInstance = 0;
	int roomsCleanedPMSingleInstance = 0;
	double productivityRoomsCleanedAM = 0;
	double productivityRoomsCleanedPM = 0;
	double productivityLaundry = 0;
	double productivityNonGoalAM = 0;
	double productivityNonGoalPM = 0;
	double productivityNonGoalLaundry = 0;
	int totalRoomsCleaned = 0;
	int totalRoomsCleanedSingleInstance = 0;
	double hoursVarianceOne = 0;
	double hoursVarianceTwo = 0;
	double costVarianceOne = 0;
	double costVarianceTwo = 0;
	double actualCostTotal = 0;
	double offsetRoomsOccupiedTotal = 0;

	while (!Form3->SQLQuery2->Eof)
	{
		//populate the header for this column with the day of the week
		readGrid->ColumnByIndex(indexOn)->Header = Form3->SQLQuery2->Fields->Fields[1]->AsString;

		//continue populating items
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[0]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[2]->AsString;
		offsetRoomsOccupiedTotal = Form3->SQLQuery2->Fields->Fields[2]->AsFloat;
		readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[2]->AsFloat / 310.0) + " %";

		//fill with rest of non role type stuff
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[3]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[4]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[5]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[6]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[7]->AsString;
		readGrid->Cells[indexOn][columnIndex++] = Form3->SQLQuery2->Fields->Fields[8]->AsString;

		//fill with role items
		++columnIndex;
		for (int i = 0; i < rVector.size(); ++i)
		{
			//need to round when appropriate and make it a percentage
			for (int j = 0; j < 3; ++j)
			{
				//if j == 2, these should be percents
				if (j == 2)
					readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat) + " %";
				else if (j == 1)
				{
					//compute productivity man hours
					manMinutesRoomsCleaned += Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					//compute productivity rooms cleaned AM divisor
					if (queryIndex == productivityAMIndex)
						productivityRoomsCleanedAM += Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (queryIndex == productivityPMIndex)
						productivityRoomsCleanedPM += Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (queryIndex == productivityLaundryIndex)
						productivityLaundry += Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					readGrid->Cells[indexOn][columnIndex++] = roundTwo(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
				}
				else
				{
					if (queryIndex == (productivityAMIndex - 1))
						productivityNonGoalAM = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (queryIndex == (productivityPMIndex - 1))
						productivityNonGoalPM = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (queryIndex == (productivityLaundryIndex - 1))
						productivityNonGoalLaundry = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					readGrid->Cells[indexOn][columnIndex++] = roundTwo(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
				}
			}

			//skip spaces when appropriate
			++columnIndex;
		}

		//fill with overtime, total labor hours, total labor cost
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (i == 0)
				{
					if (j == 0)
						readGrid->Cells[indexOn][columnIndex++] = roundTwo(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
					else
                        readGrid->Cells[indexOn][columnIndex++] = blank + "$" + nearestDollar(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
                }
				else if (i == 1)
				{
					//productivity computation
					if (j == 0)
						hoursVarianceOne = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (j == 1)
                        hoursVarianceTwo = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					if (j == 2)
						readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat) + " %";
					else
						readGrid->Cells[indexOn][columnIndex++] = nearestDollar(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat);
				}
				else
				{
					//productivity computation
					if (j == 0)
						costVarianceOne = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;
					else if (j == 1)
						costVarianceTwo = Form3->SQLQuery2->Fields->Fields[queryIndex]->AsFloat;

					if (j == 2)
						readGrid->Cells[indexOn][columnIndex++] = blank + makePercent(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat) + " %";
					else
						readGrid->Cells[indexOn][columnIndex++] = blank + "$" + commas(IntToStr(nearestDollar(Form3->SQLQuery2->Fields->Fields[queryIndex++]->AsFloat)));
				}

				if (SameText(readGrid->Cells[1][columnIndex-1], "Actual Cost"))
					actualCostTotal = toDouble(readGrid->Cells[indexOn][columnIndex-1]);
			}

			//skip spaces when appropriate
			++columnIndex;
		}

		//keep running total for productivity goal computation
		totalRoomsCleaned += Form3->SQLQuery2->Fields->Fields[6]->AsInteger;
		totalRoomsCleanedSingleInstance = Form3->SQLQuery2->Fields->Fields[6]->AsInteger;
		roomsCleanedAM += Form3->SQLQuery2->Fields->Fields[3]->AsInteger;
		roomsCleanedAMSingleInstance = Form3->SQLQuery2->Fields->Fields[3]->AsInteger;
		roomsCleanedPM += Form3->SQLQuery2->Fields->Fields[4]->AsInteger;
		roomsCleanedPMSingleInstance = Form3->SQLQuery2->Fields->Fields[4]->AsInteger;

		//fill in all productivity things for each iteration of cursor
		readGrid->Cells[indexOn][columnIndex++] = nearestDollar(hoursVarianceOne - hoursVarianceTwo);
		readGrid->Cells[indexOn][columnIndex++] = nearestDollar(costVarianceOne - costVarianceTwo);
		if (totalRoomsCleanedSingleInstance == 0)
			readGrid->Cells[indexOn][columnIndex++] = 0;
		else
			readGrid->Cells[indexOn][columnIndex++] = roundTwo((hoursVarianceOne * 60.0) / totalRoomsCleanedSingleInstance);
		productivityNonGoalAM /= 8.0;
		if (productivityNonGoalAM == 0)
			readGrid->Cells[indexOn][columnIndex++] = 0;
		else
			readGrid->Cells[indexOn][columnIndex++] = roundTwo(((double) roomsCleanedAMSingleInstance) / productivityNonGoalAM);
		productivityNonGoalPM /= 8.0;
		if (productivityNonGoalPM == 0)
			readGrid->Cells[indexOn][columnIndex++] = 0;
		else
			readGrid->Cells[indexOn][columnIndex++] = roundTwo(((double) roomsCleanedPMSingleInstance) / productivityNonGoalPM);
		productivityNonGoalLaundry /= 7.5;
		if (productivityNonGoalLaundry == 0)
			readGrid->Cells[indexOn][columnIndex++] = 0;
		else
			readGrid->Cells[indexOn][columnIndex++] = roundTwo(((double) totalRoomsCleanedSingleInstance) / productivityNonGoalLaundry);

		//advance cursor (for daily, should only be one iteration of cursor)
		++indexOn;
		Form3->SQLQuery2->Next();
	}

	//get what the productivity column is (probably ok to stay hardcoded)
	int productivityColumn = 1;

	//change first two productivity things to 0.00
	readGrid->Cells[productivityColumn][productivityStartIndex++] = "0.00";
	readGrid->Cells[productivityColumn][productivityStartIndex++] = "0.00";

	//compute productivity for man hours
	if (totalRoomsCleaned != 0)
		manMinutesRoomsCleaned = roundTwo((manMinutesRoomsCleaned * 60.0) / totalRoomsCleaned);
	else
		manMinutesRoomsCleaned = 0;
	readGrid->Cells[productivityColumn][productivityStartIndex++] = manMinutesRoomsCleaned;

	//compute productivity for roomsCleanedAM
	productivityRoomsCleanedAM /= 8.0;
	if (productivityRoomsCleanedAM == 0)
		productivityRoomsCleanedAM = 0;
	else
		productivityRoomsCleanedAM = roundTwo(((double) roomsCleanedAM) / productivityRoomsCleanedAM);
	readGrid->Cells[productivityColumn][productivityStartIndex++] = productivityRoomsCleanedAM;

	//compute productivity for roomsCleanedPM
	productivityRoomsCleanedPM /= 8.0;
	if (productivityRoomsCleanedPM == 0)
		productivityRoomsCleanedPM = 0;
	else
		productivityRoomsCleanedPM = roundTwo(((double) roomsCleanedPM) / productivityRoomsCleanedPM);
	readGrid->Cells[productivityColumn][productivityStartIndex++] = productivityRoomsCleanedPM;

	//compute productivity for laundryAttendantProductivity
	productivityLaundry /= 7.5;
	if (productivityLaundry == 0)
		productivityLaundry = 0;
	else
		productivityLaundry = roundTwo(((double) totalRoomsCleaned) / productivityLaundry);
	readGrid->Cells[productivityColumn][productivityStartIndex++] = productivityLaundry;

    //fill cells in top left with extra statistics
	readGrid->Cells[0][0] = "$12.96 Budget";
	if (actualCostTotal < 0)
		readGrid->Cells[0][2] = blank + "$" + roundTwo((actualCostTotal * -1) / offsetRoomsOccupiedTotal) + " Payroll cost to clean room";
	else
		readGrid->Cells[0][2] = blank + "$" + roundTwo(actualCostTotal / offsetRoomsOccupiedTotal) + " Payroll cost to clean room";
	if (toDouble(readGrid->Cells[0][2]) < 0)
		readGrid->Cells[0][1] = blank + "$" + roundTwo((toDouble(readGrid->Cells[0][2])*-1) - toDouble(readGrid->Cells[0][0])) + " Over/Under per rooom";
	else
		readGrid->Cells[0][1] = blank + "$" + roundTwo(toDouble(readGrid->Cells[0][2]) - toDouble(readGrid->Cells[0][0])) + " Over/Under per rooom";
	readGrid->Cells[0][3] = "$76,995 Accounting";
	if (toDouble(readGrid->Cells[0][3]) < 0)
		readGrid->Cells[0][4] = blank + "$" + commas(IntToStr(nearestDollar((toDouble(readGrid->Cells[0][3])*-1) - actualCostTotal))) + " Variance";
	else
		readGrid->Cells[0][4] = blank + "$" + commas(IntToStr(nearestDollar(toDouble(readGrid->Cells[0][3]) - actualCostTotal))) + " Variance";

  	readGrid->Visible = true;
}

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
	//set secondTimeArbitray initiall to false
    secondTimeArbitrary = false;

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
	//otherwise display advanced read form (need to populate role vector also)
	else
	{
		//simply call populateGrid function passing in today's week and running as week type

		//fill roleVector with all the roles currently possible
		Form3->SQLQuery2->SQL->Text = "SELECT Bare_Role_Name FROM baldwins_hotel_data.role_table WHERE hotelID = '"+currentHotelID+"';";
		Form3->SQLQuery2->Open();
		Form3->SQLQuery2->First();
		while (!Form3->SQLQuery2->Eof)
		{
			roleVector.push_back(Form3->SQLQuery2->Fields->Fields[0]->AsString);
			Form3->SQLQuery2->Next();
		}

        //get the date and change to a string
		TDateTime dayChosenStart = dayCalendar->Date;
		TDateTime dayChosenEnd = dayCalendar->Date;
		String dbDayChosenStart = "";
		String dbDayChosenEnd = "";

		//get the week selected by the day selected from the calendar
		while (DayOfWeek(dayChosenStart) != 7)
			dayChosenStart--;
		if (DayOfWeek(dayChosenEnd) == 7)
		{
			dayChosenEnd++;
			dayChosenEnd++;
		}
		else if (DayOfWeek(dayChosenEnd) == 1)
			dayChosenEnd++;
		while (DayOfWeek(dayChosenEnd) != 1)
			dayChosenEnd++;

		//convert to strings to pass into populateGrid function
		dbDayChosenStart = dayChosenStart.FormatString(L"yyyy-mm-dd");
		dbDayChosenEnd = dayChosenEnd.FormatString(L"yyyy-mm-dd");
		privateDayChosenStart = StrToDate(dayChosenStart).FormatString(L"mm/dd");
		privateDayChosenEnd = StrToDate(dayChosenEnd).FormatString(L"mm/dd");

		//call display function
		populateGrid(roleVector, 0, dbDayChosenStart, dbDayChosenEnd, "week");
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
	backImageButton2->Visible = false;
	filtersLabel->Visible = false;
	selectAllButton->Visible = false;
	dayRadio->Visible = false;
	weekRadio->Visible = false;
	arbitraryRadio->Visible = false;
	monthRadio->Visible = false;
	yearRadio->Visible = false;
	dayRadio->IsChecked = false;
	weekRadio->IsChecked = false;
	arbitraryRadio->IsChecked = false;
	monthRadio->IsChecked = false;
	yearRadio->IsChecked = false;
	selectAllButton->Text = "Select All";
	roleListBox->Items->Clear();
	roleVector.clear();
	monthPopupBox->ItemIndex = 0;
	yearPopupBox->ItemIndex = 0;
	readGrid->Visible = false;
	secondTimeArbitrary = false;
	dayCalendar->Visible = false;
	rangeEndCalendar->Visible = false;
	dayCalendar->Date = Now();
	rangeEndCalendar->Date = Now();

	//clear readGrid and set rowCount back to bare minimum (19)
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < readGrid->RowCount; ++j)
			readGrid->Cells[i][j] = "";
	readGrid->RowCount = 19;
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
			arbitraryRadio->Visible = true;
			monthRadio->Visible = true;
			yearRadio->Visible = true;

			break;
		//week filter
		case 1:
			//hide things from view filter
			dayRadio->Visible = false;
			weekRadio->Visible = false;
			arbitraryRadio->Visible = false;
			monthRadio->Visible = false;
			yearRadio->Visible = false;

			if (!secondTimeArbitrary)
			{
				//store things from view filter
				if (dayRadio->IsChecked)
					viewType = "Day";
				else if (weekRadio->IsChecked)
					viewType = "Week";
				else if (arbitraryRadio->IsChecked)
					viewType = "Arbitrary";
				else if (monthRadio->IsChecked)
					viewType = "Month";
				else
					viewType = "Year";
			}
			else
				viewType = "Arbitrary";

			//if arbitray date range, follows seperate pattern
			if (SameText(viewType, "Arbitrary"))
			{
				if (!secondTimeArbitrary)
				{
					//display filterLabel appropriately
					filtersLabel->Text = L"Choose a start date for the range:";
					secondTimeArbitrary = true;

					//show calendar and allow user to choose a start day for the range
					dayCalendar->Visible = true;
				}
				else
				{
					//increase basicState and display filterLabel appropriately
					basicState++;
					secondTimeArbitrary = false;
					filtersLabel->Text = L"Choose an end date for the range:";

					//show calendar and allow user to choose an end day for the range
					dayCalendar->Visible = false;
					rangeEndCalendar->Visible = true;
				}
			}
			//follows same pattern for all others
			else
			{
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
			rangeEndCalendar->Visible = false;
			monthPopupBox->Visible = false;
			yearPopupBox->Visible = false;

			//needed to compile
			String dbDayChosen = "";
			String dbDayChosenStart = "";
			String dbDayChosenEnd = "";

			//get whatever data was chosen by user (i.e. day, week, month or year)
			if (viewType == "Day")
			{
				//get the date selected from the calendar
				TDateTime dayChosen = dayCalendar->Date;
				dbDayChosen = StrToDate(dayChosen).FormatString(L"yyyy-mm-dd");

				//call display function
				populateGrid(roleVector, dbDayChosen);
			}
			else if (viewType == "Week")
			{
				//get the date and change to a string
				TDateTime dayChosenStart = dayCalendar->Date;
				TDateTime dayChosenEnd = dayCalendar->Date;

				//get the week selected by the day selected from the calendar
				while (DayOfWeek(dayChosenStart) != 7)
					dayChosenStart--;
				if (DayOfWeek(dayChosenEnd) == 7)
				{
					dayChosenEnd++;
					dayChosenEnd++;
				}
				else if (DayOfWeek(dayChosenEnd) == 1)
					dayChosenEnd++;
				while (DayOfWeek(dayChosenEnd) != 1)
					dayChosenEnd++;

				//convert to strings to pass into populateGrid function
				dbDayChosenStart = StrToDate(dayChosenStart).FormatString(L"yyyy-mm-dd");
				dbDayChosenEnd = StrToDate(dayChosenEnd).FormatString(L"yyyy-mm-dd");
				privateDayChosenStart = StrToDate(dayChosenStart).FormatString(L"mm/dd");
				privateDayChosenEnd = StrToDate(dayChosenEnd).FormatString(L"mm/dd");

				//call display function
				populateGrid(roleVector, 0, dbDayChosenStart, dbDayChosenEnd, "week");
			}
			else if (viewType == "Arbitrary")
			{
				//get the date and change to a string
				TDateTime dayChosenStart = dayCalendar->Date;
				TDateTime dayChosenEnd = rangeEndCalendar->Date;

				//convert to strings to pass into populateGrid function
				dbDayChosenStart = StrToDate(dayChosenStart).FormatString(L"yyyy-mm-dd");
				dbDayChosenEnd = StrToDate(dayChosenEnd).FormatString(L"yyyy-mm-dd");
				privateDayChosenStart = StrToDate(dayChosenStart).FormatString(L"mm/dd");
				privateDayChosenEnd = StrToDate(dayChosenEnd).FormatString(L"mm/dd");

				//call display function (should work same with week as with arbitrary)
				populateGrid(roleVector, 0, dbDayChosenStart, dbDayChosenEnd, "week");
            }
			else if (viewType == "Month")
			{
				//get the month selected from the drop down (value 1 through 12)
				int currentMonthIndex = monthPopupBox->ItemIndex + 1;

				//call display function
				populateGrid(roleVector, currentMonthIndex, "null", "null", "month");
			}
			else
			{
				//get the year selected from the drop down
				int currentYearIndex = yearPopupBox->ItemIndex;
				String currentYear = yearPopupBox->Items->operator [](currentYearIndex);

				//testing
				filtersLabel->Text = currentYear;
				filtersLabel->Visible = true;

				//call display function
				populateGrid(roleVector, (int) toDouble(currentYear), "null", "null", "year");
			}

			//hide things from testing
			filtersLabel->Visible = false;

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
			if (SameText(viewType, "Arbitrary"))
			{
				//special case, is for start date
				filtersLabel->Text = "View By:";
				selectAllButton->Visible = false;
				roleListBox->Visible = false;
				backImageButton2->Visible = true;
				dayCalendar->Visible = false;
				rangeEndCalendar->Visible = false;
				dayCalendar->WeekNumbers = false;
				monthPopupBox->Visible = false;
				yearPopupBox->Visible = false;

				//handle signature things for view filter
				dayRadio->Visible = true;
				weekRadio->Visible = true;
				arbitraryRadio->Visible = true;
				monthRadio->Visible = true;
				yearRadio->Visible = true;

				//special case set viewType to blank to avoid issues
				viewType = "";
				secondTimeArbitrary = false;
			}
			else
			{
				//hide things from view filter
				dayRadio->Visible = false;
				weekRadio->Visible = false;
				arbitraryRadio->Visible = false;
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
			}

			break;
		case 2:
			// view filter
			if (SameText(viewType, "Arbitrary") && !secondTimeArbitrary)
			{
				//must be where choosing end date
				secondTimeArbitrary = true;
				basicState--;
				filtersLabel->Text = L"Choose a start date for the range:";
				rangeEndCalendar->Visible = false;
				dayCalendar->Visible = true;
			}
			else
			{
				basicState--;
				filtersLabel->Text = "View By:";
				selectAllButton->Visible = false;
				roleListBox->Visible = false;
				backImageButton2->Visible = true;
				dayCalendar->Visible = false;
				rangeEndCalendar->Visible = false;
				dayCalendar->WeekNumbers = false;
				monthPopupBox->Visible = false;
				yearPopupBox->Visible = false;

				//handle signature things for view filter
				dayRadio->Visible = true;
				weekRadio->Visible = true;
				arbitraryRadio->Visible = true;
				monthRadio->Visible = true;
				yearRadio->Visible = true;
			}

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



