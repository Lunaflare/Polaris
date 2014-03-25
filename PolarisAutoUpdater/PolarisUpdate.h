//---------------------------------------------------------------------------

#ifndef PolarisUpdateH
#define PolarisUpdateH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
class TupdateThread : public TThread
{
protected:
	virtual void __fastcall Execute();
public:
	__fastcall TupdateThread();
};

class TForm5 : public TForm
{
__published:	// IDE-managed Components
	TBrushObject *splash;
	TProgressBar *progressBar;
	TLabel *statusLabel;
	TStyleBook *StyleBook1;
	TTimer *testTimer;
	TTimer *exitTimer;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall exitTimerTimer(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
private:	// User declarations
public:		// User declarations
	void __fastcall checkFiles();
	__fastcall TForm5(TComponent* Owner);
	void __fastcall TForm5::ThreadTerminated(TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
