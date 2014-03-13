//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
#include <FMX.Controls.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Types.hpp>
#include <System.Classes.hpp>
#include <FMX.ExtCtrls.hpp>
#include <FMX.Menus.hpp>
#include <FMX.Grid.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Ani.hpp>
//---------------------------------------------------------------------------
#include <vector>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>

using namespace std;
//---------------------------------------------------------------------------
class TReadThread : public TThread
{
protected:
	virtual void __fastcall Execute();
public:
	__fastcall TReadThread();
};

class TForm4 : public TForm
{
__published:	// IDE-managed Components
	TStyleBook *StyleBook2;
	TImage *homeImageButton4;
	TLabel *homeLabel4;
	TImage *nextImageButton2;
	TLabel *filtersLabel;
	TImage *backImageButton2;
	TListBox *roleListBox;
	TRadioButton *dayRadio;
	TRadioButton *weekRadio;
	TRadioButton *monthRadio;
	TRadioButton *yearRadio;
	TButton *selectAllButton;
	TCalendar *dayCalendar;
	TPopupBox *monthPopupBox;
	TPopupBox *yearPopupBox;
	TStringGrid *readGrid;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn2;
	TStringColumn *StringColumn3;
	TStringColumn *StringColumn4;
	TStringColumn *StringColumn5;
	TStringColumn *StringColumn6;
	TStringColumn *StringColumn7;
	TStringColumn *StringColumn8;
	TStringColumn *StringColumn9;
	TStringColumn *StringColumn10;
	TRadioButton *arbitraryRadio;
	TCalendar *rangeEndCalendar;
	TGroupBox *radioGroupBox;
	TTabControl *rangeTabContainer;
	TTabItem *dayCalendarTab;
	TTabItem *rangeEndCalendarTab;
	TCalendar *filterStartCalendar;
	TCalendar *filterEndCalendar;
	TImage *updateImageButton;
	TAniIndicator *readLoadingIndicator;
	TFloatAnimation *gridFadeIn;
	TFloatAnimation *gridFadeOut;
	TPanel *rolePanel;
	TLabel *filterLabel;
	TImage *filterSeperatorImage;
	TFloatAnimation *FloatAnimation1;
	TFloatAnimation *FloatAnimation2;
	TFloatAnimation *FloatAnimation3;
	TFloatAnimation *FloatAnimation4;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall homeImageButton4Click(TObject *Sender);
	void __fastcall nextImageButton2Click(TObject *Sender);
	void __fastcall backImageButton2Click(TObject *Sender);
	void __fastcall selectAllButtonClick(TObject *Sender);
	void __fastcall roleListBoxChangeCheck(TObject *Sender);
	void __fastcall dayRadioClick(TObject *Sender);
	void __fastcall weekRadioClick(TObject *Sender);
	void __fastcall arbitraryRadioClick(TObject *Sender);
	void __fastcall monthRadioClick(TObject *Sender);
	void __fastcall yearRadioClick(TObject *Sender);
	void __fastcall updateImageButtonClick(TObject *Sender);
	//was private
	public:	// User declarations
	String inputTable;
	String readTable;
	String laborTable;
	int basicState; //0=roles,1=view,2=week,3=display
	vector<String> roleVector;
	String viewType;
	String privateDayChosenStart;
	String privateDayChosenEnd;
	bool secondTimeArbitrary;
	int selectAllButtonX;
	int selectAllButtonY;
	int RoleListBoxX;
	int RoleListBoxY;
	int radioGroupBoxX;
	int radioGroupBoxY;
	int monthYearSelected;
	int dayCalendarX;
	int dayCalendarY;
	int monthPopupBoxX;
	int monthPopupBoxY;
	int yearPopupBoxX;
	int yearPopupBoxY;
	bool homeAlreadyPressed;
public:		// User declarations
	__fastcall TForm4(TComponent* Owner);
	void __fastcall populateGrid(vector<String>, String);
	void __fastcall populateGrid(vector<String>, int, String, String, String);
	float __fastcall makePercent(float);
	float __fastcall roundTwo(float value);
	int __fastcall nearestDollar(float value);
	string __fastcall IntToStr(long int n);
	String __fastcall commas(string num);
	double __fastcall toDouble(String);
	void __fastcall displayFilters(String, int);
	void __fastcall TForm4::ThreadTerminated(TObject *Sender);
	void __fastcall chooseButtonCode();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
