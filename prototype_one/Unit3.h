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
//---------------------------------------------------------------------------
#include <iostream>
#include <map>
#include <vector>
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

//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TStyleBook *StyleBook2;
	TImage *homeImageButton3;
	TLabel *homeLabel3;
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
	TPopupBox *datePopupBox;
	TLabel *datePopupBoxLabel;
	TImage *chooseDateImageButton;
	TSQLQuery *SQLQuery3;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall homeImageButton3Click(TObject *Sender);
	void __fastcall nextImageButtonClick(TObject *Sender);
	void __fastcall backImageButtonClick(TObject *Sender);
	void __fastcall submitButtonClick(TObject *Sender);
	void __fastcall dbFieldEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift);
	void __fastcall datePopupBoxChange(TObject *Sender);
	void __fastcall chooseDateImageButtonClick(TObject *Sender);
private:	// User declarations
	inputValues inputObject;
	String inputTable;
	String readTable;
	String dateChosen;
	bool alreadyThere;
public:		// User declarations
	__fastcall TForm3(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
