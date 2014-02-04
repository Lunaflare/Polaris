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
	TStyleBook *StyleBook2;
	TButton *recoverPasswordButton;
	TImage *logoImage;
	TLabel *rememberLabel;
	TSwitch *Switch1;
	TLabel *copyrightLabel;
	TAniIndicator *loadIndicator;
	TBrushObject *BrushObject1;
	void __fastcall loginSubmitButtonClick(TObject *Sender);
	void __fastcall loginregisterButtonClick(TObject *Sender);
	void __fastcall recoverPasswordButtonClick(TObject *Sender);
	void __fastcall passwordEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	String username;
	String password;
	String inputLevel;
	String readLevel;
	String accessLevel;
	String userID;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	String getUsername() { return username; }
	String getPassword() { return password; }
	String getInputLevel() { return inputLevel; }
	String getReadLevel() { return readLevel; }
	void setReadLevel(String r) { readLevel = r; }
	String getAccessLevel() { return accessLevel; }
    void setInputLevel(String i) { inputLevel = i; }
	String getUserID() { return userID; }
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
