//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mainfrm.h"
#include "progressfrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_progress *frm_progress;
//---------------------------------------------------------------------------
__fastcall Tfrm_progress::Tfrm_progress(TComponent* Owner) : TForm(Owner) { }
//---------------------------------------------------------------------------
void __fastcall Tfrm_progress::b_progress_cancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_progress::on_show(TObject *Sender)
{
	CNTW->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_progress::FormHide(TObject *Sender)
{
	CNTW->it_start = CNTW->it_end;//завершаем разбор текста
	CNTW->Enabled = true;
	Position = poMainFormCenter;  //центрируем форму перед сл. ее запуском
}
//---------------------------------------------------------------------------
