//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
#include <FMX.Ani.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Effects.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Types.hpp>
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
#include <string>
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Effects.hpp>
#include <Data.DB.hpp>
#include <Data.DBXMySQL.hpp>
#include <Data.FMTBcd.hpp>
#include <Data.SqlExpr.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Ani.hpp>
//---------------------------------------------------------------------------

using namespace std;
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
	TButton *saveChangesButton;
	TButton *editRoleButton;
	TImage *homeImageButton5;
	TLabel *confirmPasswordLabel;
	TEdit *confirmPasswordEdit;
	TGlowEffect *GlowEffect1;
	TBrushObject *BrushObject1;
	TImage *settingsLabel;
	TFloatAnimation *floatAnimation1;
	TFloatAnimation *opacityAnimation1;
	TImage *Image1;
	TImage *home_swapper;
	TImage *home_over;
	TLabel *adminLabel;
	TImage *sepImage;
	TImage *sep2Image;
	TGroupBox *changePasswordGroup;
	TImage *homeLabelImage;
	TLabel *copyrightLabel;
	TButton *addRoleButton;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall homeImageButton5Click(TObject *Sender);
	void __fastcall saveChangesButtonClick(TObject *Sender);
	void __fastcall homeImageButton5MouseEnter(TObject *Sender);
	void __fastcall homeImageButton5MouseLeave(TObject *Sender);
	void __fastcall addRoleButtonClick(TObject *Sender);
	void __fastcall editRoleButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm5(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
