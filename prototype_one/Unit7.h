//---------------------------------------------------------------------------

#ifndef Unit7H
#define Unit7H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Menus.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
class TForm7 : public TForm
{
__published:	// IDE-managed Components
	TLabel *chooseRoleLabel;
	TLabel *roleNameLabel;
	TLabel *roleWagesLabel;
	TEdit *roleNameEdit;
	TEdit *roleWagesEdit;
	TPopupBox *rolePopupBox;
	TImage *homeImageButton7;
	TImage *backImageButton;
	TButton *saveChangesButton;
	TImage *nextImageButton;
	TLabel *errorLabel;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall homeImageButton7Click(TObject *Sender);
	void __fastcall backImageButtonClick(TObject *Sender);
	void __fastcall nextImageButtonClick(TObject *Sender);
	void __fastcall saveChangesButtonClick(TObject *Sender);
private:	// User declarations
	int state;
	String inputTable;
	String readTable;
	String laborTable;
	String currentHotelID;
	String roleChosen;
	double roleWages;
public:		// User declarations
	__fastcall TForm7(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm7 *Form7;
//---------------------------------------------------------------------------
#endif
