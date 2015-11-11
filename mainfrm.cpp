//---------------------------------------------------------------------------

#include <vcl.h>
#include <iostream>
#include <time.h>
#include <windows.h>
#include <Clipbrd.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "mainfrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCNTW *CNTW;
//---------------------------------------------------------------------------
__fastcall TCNTW::TCNTW(TComponent* Owner) : TForm(Owner){}
//---------------------------------------------------------------------------
void __fastcall TCNTW::exit_click(TObject *Sender) {Close(); }
//---------------------------------------------------------------------------
void __fastcall TCNTW::sg_all_clean(TStringGrid *tsg) { for (int i = 1; i < tsg->RowCount; i++) tsg->Rows[i]->Clear(); sg_set_title(tsg); tsg->RowCount = 2; }
//---------------------------------------------------------------------------
void __fastcall TCNTW::sg_set_max_row(TStringGrid *tsg, int index=0) { for (int i = 1; 1; i++) { if (tsg->Cells[index][i].IsEmpty()) { tsg->RowCount = i; return; } } }
//---------------------------------------------------------------------------
void __fastcall TCNTW::sg_add_new_row(TStringGrid *tsg, UnicodeString val_1, UnicodeString val_2, UnicodeString val_3, UnicodeString val_4) { for (int i = tsg->RowCount + 1; 1; i--) { if (!tsg->Cells[0][i - 1].IsEmpty()) { tsg->Cells[0][i] = val_1; tsg->Cells[1][i] = val_2; tsg->Cells[2][i] = val_3; tsg->Cells[3][i] = val_4; tsg->RowCount += (tsg->RowCount > i) ? 0 : 1;return; } } }
//---------------------------------------------------------------------------
void __fastcall TCNTW::sg_set_title(TStringGrid *tsg) { tsg->Cells[0][0] = " №"; tsg->Cells[1][0] = " Слово"; tsg->Cells[2][0] = " Частота"; tsg->Cells[3][0] = " В словаре"; }
//---------------------------------------------------------------------------
void __fastcall TCNTW::app_update() { static DWORD t1 = 0; DWORD t2 = GetTickCount(); /*sb->Panels->Items[0]->Text = ++cnt;*/ frm_progress->progress->Position = str.length() - (it_end - it_start); if (t2 > t1) { t1 = t2 + 300; Application->ProcessMessages(); if (Application->Terminated) exit(0); } }
//---------------------------------------------------------------------------
void __fastcall TCNTW::pogress_init(int end) { frm_progress->progress->Position = 0; frm_progress->progress->Min = 0; frm_progress->progress->Max = end; }
//---------------------------------------------------------------------------
void __fastcall TCNTW::add_db(UnicodeString value) { exec_db("UPDATE WORDS SET CNT = CNT + 1 WHERE NAME = " + QuotedStr(value)); if (!sqlite3_changes(db)) exec_db("INSERT INTO WORDS(NAME, CNT, DIC) VALUES(" + QuotedStr(value) + ", 1, 0)"); }
//---------------------------------------------------------------------------
UnicodeString & __fastcall TCNTW::replace(UnicodeString & msg) { msg = StringReplace(msg, "’", "'", TReplaceFlags() << rfReplaceAll << rfIgnoreCase); msg = StringReplace(msg, "`", "'", TReplaceFlags() << rfReplaceAll << rfIgnoreCase); return msg = msg.LowerCase(); }
//---------------------------------------------------------------------------
void __fastcall TCNTW::reginit(UnicodeString ustr, UnicodeString ufrm) { frm = AnsiString(ufrm).c_str();	str = AnsiString(ustr).c_str();	it_start = str.begin();	it_end = str.end(); }
//---------------------------------------------------------------------------
bool __fastcall TCNTW::regexpr(UnicodeString & ares) { if (boost::regex_search(it_start, it_end, res, frm)) { ares = res[0].str().c_str(); it_start = res[1].second; return 1; } return 0; }
//---------------------------------------------------------------------------
bool __fastcall TCNTW::is_regx(UnicodeString lstr, UnicodeString lfrm, UnicodeString & lres) { boost::smatch l_res; boost::regex l_frm = boost::regex(AnsiString(lfrm).c_str()); std::string l_str = AnsiString(lstr).c_str(); std::string::const_iterator l_it_start = l_str.begin(); std::string::const_iterator l_it_end = l_str.end(); if (boost::regex_search(l_it_start, l_it_end, l_res, l_frm)) { lres = l_res[0].str().c_str(); return 1; } return 0; }
//---------------------------------------------------------------------------
void __fastcall TCNTW::new_db(const char *filename)
{
	char *error_message = 0;
	AnsiString sql_req;

	if (sqlite3_open(filename, &db) == SQLITE_OK) {
		sql_req = "CREATE TABLE WORDS                                           \
				   (                                                            \
					   ID       INTEGER CONSTRAINT id PRIMARY KEY AUTOINCREMENT,\
					   NAME 	STRING,                                         \
					   CNT      INTEGER DEFAULT NULL,                           \
					   DIC      INTEGER DEFAULT NULL                            \
				   );";
		if (sqlite3_exec(db, sql_req.c_str(), 0, 0, &error_message) > SQLITE_ERROR) ShowMessage(error_message);
	} else {
		MessageDlg("Не удалось создать журнал!\n"
				   "Возможно на диске не достаточно места\n",
				   mtWarning,
				   TMsgDlgButtons() << mbOK, 0);
		DeleteFile(filename);
		exit(0);
	}
	if (error_message) sqlite3_free(error_message);
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::exec_db(UnicodeString request)
{
	char *error_message = 0;
	if (db) {
		if (sqlite3_exec(db, AnsiString(request).c_str(), NULL, 0, &error_message) != SQLITE_OK) {
			ShowMessage(error_message);
			exit(1);
		}
	} else {
		ShowMessage("Неудалось открыть базу данных!");
		exit(1);
	}

	if (error_message) sqlite3_free(error_message);
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::load_db()
{
	sqlite3_stmt *stmt;
	char *error_message = 0;
	AnsiString sql_req;

	cnt = 1;

	if (db) {
		sql_req = "SELECT ID, NAME, CNT, DIC FROM WORDS ORDER BY CNT;";
		int rc = sqlite3_prepare(db, sql_req.c_str(), sql_req.Length(), &stmt, 0);
		if (rc != SQLITE_OK) {
			ShowMessage(error_message);
			exit(1);
		} else {
			//отображаем слова из БД
			sg_all_clean(sg);
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				sg_add_new_row(sg,(char *)(sqlite3_column_text(stmt, 0)),
								  (char *)(sqlite3_column_text(stmt, 1)),
								  (char *)(sqlite3_column_text(stmt, 2)),
								  (char *)(sqlite3_column_text(stmt, 3)));

				sb->Panels->Items[0]->Text = cnt++;
				app_update();//периодически обновляем внешний вид программы
			}
		}
	} else {
		ShowMessage("Неудалось открыть базу данных!");
		exit(1);
	}

	if (error_message) sqlite3_free(error_message);
	sqlite3_finalize(stmt);
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::process(UnicodeString filename)
{
	TStringList * sl  = new TStringList();
	cnt = 0;
	int t = clock();
	sl->LoadFromFile(filename);
	UnicodeString res_1;

	AnsiString reg_format = "";//выражение для поиска
	if (n_rus->Checked && n_eng->Checked) {
		reg_format = "([А-Яа-яЁё]+)|([A-Za-z'’`]+)";
	} else if (n_rus->Checked) {
		reg_format = "[А-Яа-яЁё]+";
	} else if (n_eng->Checked) {
		reg_format = "[A-Za-z'’`]+";
	}
	if (reg_format.Length()) {
		reginit(sl->Text, "[А-Яа-яЁёA-Za-z'’`]{2,}");
		pogress_init(str.length());
		frm_progress->Show();
		exec_db("BEGIN;");
		while(regexpr(res_1)) {
            UnicodeString res_2;
			if (is_regx(res_1, reg_format, res_2)) {
				if (res_1 == res_2) {
					add_db(replace(res_1));
				}
			}
			app_update();//периодически обновляем внешний вид программы
		}
		exec_db("COMMIT;");
		frm_progress->Close();
	}
	load_db();
	if (sl)  delete sl;
	sb->Panels->Items[0]->Text += " words / " + String(clock()-t) + " ms";
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::load_click(TObject *Sender)
{
	AnsiString filename, path_to_file;
	cnt = 0;
	if (dlgo->Execute()) {
		if (dlgo->FileName.Length() >= 3) {
			try {
				do {
					filename = ExtractFileName(dlgo->FileName) + "_" + AnsiString(++cnt) + ".dat";
					path_to_file = ExtractFilePath(Application->ExeName) + UnicodeString("temp\\");
					if(!DirectoryExists(UnicodeString(path_to_file))) {
						MkDir(UnicodeString(path_to_file));
					}
				} while (FileExists(UnicodeString(path_to_file + filename)));
				new_db((path_to_file + filename).c_str());
				//запускаем обработку файла
				process(dlgo->FileName);
			} catch (...) {
			    CNTW->Enabled = true; //на всякий "пожарный" случай!
				ShowMessage("Ошибка создания временного файла!");
			}
		} else {
			ShowMessage("Не верно указанно имя файла!");
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::save_as_text(UnicodeString filename)
{
	TStringList * sl  = new TStringList();
	for (int i = 1; i < sg->RowCount; i++) {
		sl->Add(sg->Cells[2][i] + "\t\t\t" + sg->Cells[1][i]);
	}
	sl->SaveToFile(filename);
	if (sl) delete sl;
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::save_as_click(TObject *Sender)
{
	if (dlgs->Execute()) {
		if (dlgs->FileName.Length() >= 3) {
			save_as_text(dlgs->FileName);
		} else {
			ShowMessage("Не верно указанно имя файла!");
		}
	}
}
//---------------------------------------------------------------------------
int t_size = 0;
void __fastcall TCNTW::frm_resize(TObject *Sender)
{
	double scale;
	int const MIN_WIDTH = 20;
	//вычисляем коэфициент масштабирования
	if (t_size) scale = double(sg->Width)/double(t_size); else scale = 1.0;
	//сохраняем новую ширину таблицы
	t_size = sg->Width;
	//масштабируем колонки таблицы
	for (int i = 0; i < sg->ColCount; i++) sg->ColWidths[i] *= scale;
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::frm_show(TObject *Sender)
{
    sg_all_clean(sg);
}
//---------------------------------------------------------------------------
int col = 0, row = 0, state = 0;
void __fastcall TCNTW::sg_mouse_up(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int const MIN_WIDTH = 20;
	int const DELTAX    = 4;
	int t_col, t_row, t_dx;
	//ограничиваем минимальную ширину колонки
	for (int i = 0; i < sg->ColCount; i++) if (sg->ColWidths[i] < MIN_WIDTH) sg->ColWidths[i] = MIN_WIDTH;
	//обновляем текущее положение курсора (со смещением для заголовка)
	sg->MouseToCell((Y <= sg->DefaultRowHeight) ? X-DELTAX : X, Y, col, row);
	state = (col >= 0) ? 1 : 0;
	//sb->Panels->Items[0]->Text = String(col) + " - " + String(row);
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::sg_dbl_click(TObject *Sender)
{
	TStringGrid * tsg   = (TStringGrid *)Sender;
	int max             = 0;
	int const DEFBORDER = 6;

    //обрабатываем автомасштабирование только когда курсор на краю поля, как в Excel
	HCURSOR curs    = GetCursor();
	HCURSOR syscurs = (HCURSOR)GetClassLong((HWND)tsg->Handle, GCL_HCURSOR);

	if (!GetLastError() && curs != syscurs) {
		//обходим колонку в поисках самого "широкого" слова
		for (int i = 0, temp; state && (i < tsg->RowCount); i++) {
			temp = tsg->Canvas->TextWidth(sg->Cells[col][i]) + DEFBORDER;
			if (temp > max) max = temp;
		}
		if (max > 0) tsg->ColWidths[col] = max;
	}
	state = 0;
}
//---------------------------------------------------------------------------
UnicodeString TCNTW::print_version_string_info(const TCHAR *fileName)
{
	UnicodeString version_info = "";

	PLONG infoBuffer;  // буфер, куда будем читать информацию
	DWORD infoSize;    // и его размер

	struct LANGANDCODEPAGE { // структура для получения кодовых страниц по языкам трансляции ресурсов файла
		WORD wLanguage;
		WORD wCodePage;
	} *pLangCodePage;

	// имена параметров, значения которых будем запрашивать
	const TCHAR *paramNames[] = {
			_T("FileDescription"),
			_T("CompanyName"),
			_T("FileVersion"),
			_T("InternalName"),
			_T("LegalCopyright"),
			_T("LegalTradeMarks"),
			_T("OriginalFilename"),
			_T("ProductName"),
			_T("ProductVersion"),
			_T("Comments"),
			_T("Author")
	};

	TCHAR paramNameBuf[256]; // здесь формируем имя параметра
	TCHAR *paramValue;       // здесь будет указатель на значение параметра, который нам вернет функция VerQueryValue
	UINT paramSz;            // здесь будет длина значения параметра, который нам вернет функция VerQueryValue

	// получаем размер информации о версии файла
	infoSize = GetFileVersionInfoSize(fileName, NULL);
	if ( infoSize > 0 )
	{
		// выделяем память
		infoBuffer = (PLONG) malloc(infoSize);

        // получаем информацию
        if ( 0 != GetFileVersionInfo(fileName, NULL, infoSize, infoBuffer) )
        {
            // информация находится блоками в виде "\StringFileInfo\кодовая_страница\имя_параметра
            // т.к. мы не знаем заранее сколько и какие локализации (кодовая_страница) ресурсов имеются в файле,
            // то будем перебирать их все

            UINT cpSz;
            // получаем список кодовых страниц
            if ( VerQueryValue(infoBuffer,                      // наш буфер, содержащий информацию
                               _T("\\VarFileInfo\\Translation"),// запрашиваем имеющиеся трансляции
                               (LPVOID*) &pLangCodePage,        // сюда функция вернет нам указатель на начало интересующих нас данных
                               &cpSz) )                         // а сюда - размер этих данных
            {
				// перебираем все кодовые страницы
				for (int cpIdx = 0; cpIdx < (int)(cpSz/sizeof(struct LANGANDCODEPAGE)); cpIdx++ )
				{
					// перебираем имена параметров
					for (int paramIdx = 0; paramIdx < int(sizeof(paramNames)/sizeof(char*)); paramIdx++)
					{
						// формируем имя параметра ( \StringFileInfo\кодовая_страница\имя_параметра )
						swprintf(	paramNameBuf, _T("\\StringFileInfo\\%04x%04x\\%s"),
									pLangCodePage[cpIdx].wLanguage,  // ну, или можно сделать фильтр для
									pLangCodePage[cpIdx].wCodePage,  // какой-то отдельной кодовой страницы
									paramNames[paramIdx]);

						// получаем значение параметра
						if ( VerQueryValue(infoBuffer, paramNameBuf, (LPVOID*)&paramValue, &paramSz)) {
							if (UnicodeString(paramNames[paramIdx]) == "FileVersion") {
								version_info = UnicodeString(paramValue);
							}
						}
					}
				}
			}
		}
		free(infoBuffer);
	}
	return version_info;
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::help_click(TObject *Sender)
{
	UnicodeString version_value, msg;

	version_value = print_version_string_info(Application->ExeName.c_str());

	msg  = "Peretykin Sergey\n";
	msg += "piratkin@tut.by\n";
	msg += "in 2015 © Minsk";

	if (version_value != "") {
		msg += "\nver. " + version_value;
	}

	ShowMessage(msg);
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::popup_coppy_click(TObject *Sender)
{
	UnicodeString Buffer = "";
	for (int i = sg->Selection.Top; i <= sg->Selection.Bottom; i++) {
		Buffer+= sg->Cells[2][i] + "\t" + sg->Cells[1][i] + "\n";
	}
	Clipboard()->SetTextBuf(Buffer.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TCNTW::on_popup(TObject *Sender)
{
	a_coppy->Enabled  = (sg->RowCount >= 2 && !(sg->Rows[1]->CommaText == ",,,")) && (sg->Selection.Top <= sg->Selection.Bottom);
	a_delete->Enabled = a_coppy->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TCNTW::popup_delete_click(TObject *Sender)
{
	int end    = sg->RowCount - sg->Selection.Bottom;
	int count  = sg->Selection.Bottom - sg->Selection.Top + 1;

	for (int i = sg->Selection.Top; i <= end; i++) {
		sg->Rows[i]->CommaText = sg->Rows[count + i]->CommaText;
	}

	if (2 > (sg->RowCount - count)) sg_all_clean(sg); else sg->RowCount -= count;
}
//---------------------------------------------------------------------------



