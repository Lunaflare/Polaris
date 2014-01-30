//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Effects.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// IDE-managed Components
	TStyleBook *StyleBook2;
	TLabel *readModeLabel;
	TLabel *InputModeLabel;
	TLabel *passwordChangeLabel;
	TRadioButton *inputDefaultRadio;
	TRadioButton *inputAdvancedRadio;
	TRadioButton *readDefaultRadio;
	TRadioButton *readAdvancedRadio;
	TEdit *passwordChangeEdit;
	TGlowEffect *passwordGlowEffect;
	TButton *changePasswordButton;
	TButton *editDLRButton;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TForm5(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif