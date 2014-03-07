//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
#include <Data.DB.hpp>
#include <Data.DBXMySQL.hpp>
#include <Data.FMTBcd.hpp>
#include <Data.SqlExpr.hpp>
#include <Datasnap.DBClient.hpp>
#include <Datasnap.Provider.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Effects.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Types.hpp>
#include <System.Classes.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Menus.hpp>
#include <FMX.ExtCtrls.hpp>
#include <FMX.Ani.hpp>
//---------------------------------------------------------------------------
#include <iostream>
#include <map>
#include <vector>
#include <ctype.h>
#include <string>
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Effects.hpp>
#include <Data.DB.hpp>
#include <Data.DBXMySQL.hpp>
#include <Data.FMTBcd.hpp>
#include <Data.SqlExpr.hpp>
#include <Datasnap.DBClient.hpp>
#include <Datasnap.Provider.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Layouts.hpp>

using namespace std;

struct inputValues
{
	map<int,vector<String> > valueMap;
	int size;
	int currentIndex;
};

class calculateInfo
{
	public:
		calculateInfo(String r, String q, String b, String s, int c, double a, double l)
		{
			Role_Name = r;
			roleStandardHours = q;
			Bare_Role_Name = b;
			Standard_Hours_Reference = s;
			contextValue = c;
			actualValue = a;
			laborValue = l;

			if (actualValue != 0)
				percentPerformance = laborValue / actualValue;
			else
				percentPerformance = 0;
		}

		String Role_Name;
		String roleStandardHours;
		String Bare_Role_Name;
		String Standard_Hours_Reference;
		int contextValue;
		double actualValue;
		double laborValue;
		double percentPerformance;
};

//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TStyleBook *StyleBook2;
	TLabel *dbFieldLabel;
	TEdit *dbFieldEdit;
	TGlowEffect *usernameGlowEffect;
	TImage *backImageButton;
	TImage *nextImageButton;
	TSQLConnection *SQLConnection2;
	TSQLQuery *SQLQuery2;
	TSQLTable *SQLTable1;
	TDataSetProvider *DataSetProvider1;
	TClientDataSet *ClientDataSet1;
	TDataSource *DataSource1;
	TStringGrid *displayGrid;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn2;
	TImage *submitButton;
	TImage *chooseDateImageButton;
	TSQLQuery *SQLQuery3;
	TBrushObject *bg_gradient;
	TImage *datePopupBoxLabel;
	TLabel *copyrightLabel;
	TCalendar *calendar;
	TImage *homeLabelImage;
	TImage *Image2;
	TImage *homeImageButton5;
	TFloatAnimation *floatAnimation1;
	TFloatAnimation *opacityAnimation1;
	TImage *home_over;
	TImage *home_swapper;
	TFloatAnimation *FloatAnimation2;
	TImage *inputLabelImage;
	TFloatAnimation *FloatAnimation3;
	TFloatAnimation *FloatAnimation4;
	TImage *nextLabelImage;
	TImage *Image3;
	TImage *nextOver;
	TImage *backOver;
	TImage *Image5;
	TFloatAnimation *FloatAnimation5;
	TFloatAnimation *FloatAnimation6;
	TImage *Image4;
	TImage *submitOver;
	TFloatAnimation *FloatAnimation7;
	TFloatAnimation *FloatAnimation8;
	TImage *Image1;
	TImage *submitLabelImage;
	TImage *useArrowsImage;
	TFloatAnimation *FloatAnimation9;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall homeImageButton3Click(TObject *Sender);
	void __fastcall nextImageButtonClick(TObject *Sender);
	void __fastcall backImageButtonClick(TObject *Sender);
	void __fastcall submitButtonClick(TObject *Sender);
	void __fastcall dbFieldEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift);
	void __fastcall chooseDateImageButtonClick(TObject *Sender);
	void __fastcall calendarChange(TObject *Sender);
	void __fastcall homeImageButton5MouseEnter(TObject *Sender);
	void __fastcall homeImageButton5MouseLeave(TObject *Sender);
	void __fastcall homeImageButton5Click(TObject *Sender);
	void __fastcall nextImageButtonMouseEnter(TObject *Sender);
	void __fastcall nextImageButtonMouseLeave(TObject *Sender);
	void __fastcall backImageButtonMouseEnter(TObject *Sender);
	void __fastcall backImageButtonMouseLeave(TObject *Sender);
	void __fastcall submitButtonMouseEnter(TObject *Sender);
	void __fastcall submitButtonMouseLeave(TObject *Sender);
	void __fastcall StringColumn2KeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall displayGridClick(TObject *Sender);
	void __fastcall displayGridKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
private:	// User declarations
	inputValues inputObject;
	String inputTable;
	String readTable;
	String laborTable;
	double overtimePerHour;
	String dateChosen;
	bool alreadyThere;
	TDateTime pureDate;
	String selectedDate;
public:		// User declarations
	__fastcall TForm3(TComponent* Owner);

};

//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
