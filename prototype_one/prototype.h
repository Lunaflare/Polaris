//---------------------------------------------------------------------------

#ifndef prototypeH
#define prototypeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>
#include <FMX.ExtCtrls.hpp>
#include <Data.DB.hpp>
#include <Data.DBXMySQL.hpp>
#include <Data.SqlExpr.hpp>
#include <Data.FMTBcd.hpp>
#include <FMX.Effects.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *usernameText;
	TLabel *passwordText;
	TEdit *usernameEdit;
	TEdit *passwordEdit;
	TButton *loginSubmitButton;
	TButton *loginregisterButton;
	TSQLConnection *SQLConnection1;
	TSQLQuery *SQLQuery1;
	TGlowEffect *usernameGlowEffect;
	TGlowEffect *passwordGlowEffect;
	TLabel *errorLabel;
	void __fastcall loginSubmitButtonClick(TObject *Sender);
	void __fastcall loginregisterButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
