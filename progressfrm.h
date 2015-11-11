//---------------------------------------------------------------------------

#ifndef progressfrmH
#define progressfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class Tfrm_progress : public TForm
{
__published:	// IDE-managed Components
	TLabel *text_message;
	TProgressBar *progress;
	TButton *b_progress_cancel;
	void __fastcall b_progress_cancelClick(TObject *Sender);
	void __fastcall on_show(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Tfrm_progress(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_progress *frm_progress;
//---------------------------------------------------------------------------
#endif
