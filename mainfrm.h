//---------------------------------------------------------------------------

#ifndef mainfrmH
#define mainfrmH
//---------------------------------------------------------------------------
#include "progressfrm.h"
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
#include "sqlite3\sqlite3.h"
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <ActnPopup.hpp>
#include <PlatformDefaultStyleActnCtrls.hpp>
#include <string>
#include <regex>
//---------------------------------------------------------------------------
class TCNTW : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *mm;
	TMenuItem *m_file;
	TMenuItem *n_exit;
	TStatusBar *sb;
	TMenuItem *m_potions;
	TMenuItem *m_help;
	TMenuItem *n_load;
	TOpenDialog *dlgo;
	TStringGrid *sg;
	TMenuItem *n_eng;
	TMenuItem *n_rus;
	TMenuItem *N1;
	TMenuItem *n_save_as;
	TSaveDialog *dlgs;
	TPopupActionBar *popup;
	TMenuItem *a_coppy;
	TMenuItem *a_delete;
	void __fastcall load_click(TObject *Sender);
	void __fastcall exit_click(TObject *Sender);
	void __fastcall frm_resize(TObject *Sender);
	void __fastcall frm_show(TObject *Sender);
	void __fastcall sg_mouse_up(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall sg_dbl_click(TObject *Sender);
	void __fastcall help_click(TObject *Sender);
	void __fastcall save_as_click(TObject *Sender);
	void __fastcall popup_coppy_click(TObject *Sender);
	void __fastcall on_popup(TObject *Sender);
	void __fastcall popup_delete_click(TObject *Sender);
private:	// User declarations
	sqlite3 *db;
	int cnt;
	boost::smatch               res;//результат поиска
	boost::regex                frm;
	std::string                 str;
public:		// User declarations
	std::string::const_iterator it_start;
	std::string::const_iterator it_end;
	__fastcall TCNTW(TComponent* Owner);
	void __fastcall new_db(const char *filename);
	void __fastcall add_db(UnicodeString value);
	void __fastcall exec_db(UnicodeString request);
	void __fastcall load_db();
	void __fastcall save_as_text(UnicodeString filename);
	void __fastcall app_update();
	void __fastcall process(UnicodeString filename);
	void __fastcall sg_all_clean(TStringGrid *tsg);
	void __fastcall sg_set_title(TStringGrid *tsg);
	void __fastcall sg_set_max_row(TStringGrid *tsg, int index);
	void __fastcall sg_add_new_row(TStringGrid *tsg, UnicodeString val_1, UnicodeString val_2, UnicodeString val_3, UnicodeString val_4);
	UnicodeString print_version_string_info(const TCHAR *fileName);
	UnicodeString & __fastcall replace(UnicodeString & msg);
	bool __fastcall regexpr(UnicodeString & ares);
	void __fastcall reginit(UnicodeString ustr, UnicodeString ufrm);
	bool __fastcall is_regx(UnicodeString lstr, UnicodeString lfrm, UnicodeString & lres);
	void __fastcall pogress_init(int end);
};
//---------------------------------------------------------------------------
extern PACKAGE TCNTW *CNTW;
//---------------------------------------------------------------------------
#endif
