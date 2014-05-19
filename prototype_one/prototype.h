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
#include <string>
//---------------------------------------------------------------------------
using namespace std;

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
	void __fastcall Show();
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	String username;
	String password;
	String inputLevel;
	String readLevel;
	String accessLevel;
	String userID;
	String hotelID;
	int weekStartDay;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);

	String getUsername() { return username; }
	void setUsername(String un) { username = un; }

	String getPassword() { return password; }
	void setPassword(String p) { password = p; }

	String getInputLevel() { return inputLevel; }
	void setInputLevel(String i) { inputLevel = i; }

	String getReadLevel() { return readLevel; }
	void setReadLevel(String r) { readLevel = r; }

	String getAccessLevel() { return accessLevel; }
	void setAccessLevel(String a) { accessLevel = a; }

	String getUserID() { return userID; }
	void setUserID(String u) { userID = u; }

	String getHotelID() { return hotelID; }
	void setHotelID(String h) { hotelID = h; }

	void setWeekStartDay(int w) { weekStartDay = w; }
	int getWeekStartDay() { return weekStartDay; }
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
