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
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall homeImageButton4Click(TObject *Sender);
	void __fastcall nextImageButton2Click(TObject *Sender);
	void __fastcall backImageButton2Click(TObject *Sender);
	void __fastcall selectAllButtonClick(TObject *Sender);
	void __fastcall roleListBoxChangeCheck(TObject *Sender);
private:	// User declarations
	String inputTable;
	String readTable;
	String laborTable;
	int basicState; //0=roles,1=view,2=week,3=display
	vector<String> roleVector;
	String viewType;
public:		// User declarations
	__fastcall TForm4(TComponent* Owner);
	void __fastcall populateGrid(vector<String>, String);
	void __fastcall populateGrid(vector<String>, int);
	float __fastcall makePercent(float);
	float __fastcall roundTwo(float value);
	int __fastcall nearestDollar(float value);
	string __fastcall IntToStr(long int n);
	String __fastcall commas(string num);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
