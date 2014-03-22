//---------------------------------------------------------------------------

#ifndef Unit6H
#define Unit6H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Types.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Menus.hpp>
#include <string>
#include <vector>
//---------------------------------------------------------------------------
class TForm6 : public TForm
{
__published:	// IDE-managed Components
	TButton *saveChangesButton;
	TImage *homeImageButton6;
	TImage *backImageButton;
	TLabel *roleNameLabel;
	TEdit *roleNameEdit;
	TLabel *roleWagesLabel;
	TEdit *roleWagesEdit;
	TLabel *roleReferenceLabel;
	TPopupBox *referencePopupBox;
	TStyleBook *StyleBook2;
	TImage *errorLabel;
	TLabel *copyrightLabel;
	TImage *swapImage;
	TImage *homeOverImage;
	TImage *backOverImage;
	TImage *homeShadow;
	TImage *backShadow;
	TFloatAnimation *FloatAnimation1;
	TFloatAnimation *FloatAnimation2;
	TFloatAnimation *FloatAnimation3;
	TFloatAnimation *FloatAnimation4;
	TBrushObject *bg_gradient;
	TImage *addRoleLabel;
	TImage *checkInputImage;
	TCalloutPanel *inputCallout;
	TFloatAnimation *FloatAnimation5;
	TLabel *inputCalloutLabel;
	TCalloutPanel *wagesCallout;
	TLabel *Label1;
	TFloatAnimation *FloatAnimation6;
	TLabel *dollarSignLabel;
	TCalloutPanel *selectCallout;
	TLabel *selectLabel;
	void __fastcall backImageButtonClick(TObject *Sender);
	void __fastcall saveChangesButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall homeImageButton6Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall homeImageButton6MouseEnter(TObject *Sender);
	void __fastcall backImageButtonMouseEnter(TObject *Sender);
	void __fastcall backImageButtonMouseLeave(TObject *Sender);
	void __fastcall homeImageButton6MouseLeave(TObject *Sender);
private:	// User declarations
	String inputTable;
	String readTable;
	String laborTable;
	String currentHotelID;
public:		// User declarations
	__fastcall TForm6(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm6 *Form6;
//---------------------------------------------------------------------------
#endif
