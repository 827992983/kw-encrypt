/***************************************************************
 * Name:      kwencryptMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    kw ()
 * Created:   2020-02-16
 * Copyright: kw ()
 * License:
 **************************************************************/

#include "kwencryptMain.h"
#include <wx/msgdlg.h>
#include "DnDialogFile.h"
#include <wx/filename.h>
//(*InternalHeaders(kwencryptFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(kwencryptFrame)
const long kwencryptFrame::ID_LISTBOX1 = wxNewId();
const long kwencryptFrame::ID_BUTTON1 = wxNewId();
const long kwencryptFrame::ID_BUTTON2 = wxNewId();
const long kwencryptFrame::ID_BUTTON3 = wxNewId();
const long kwencryptFrame::idMenuQuit = wxNewId();
const long kwencryptFrame::idMenuAbout = wxNewId();
const long kwencryptFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(kwencryptFrame,wxFrame)
    //(*EventTable(kwencryptFrame)
    //*)
END_EVENT_TABLE()

kwencryptFrame::kwencryptFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(kwencryptFrame)
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, wxID_ANY, _("kw-encryptor"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLIP_CHILDREN, _T("wxID_ANY"));
    SetClientSize(wxSize(720,500));
    ListBox1 = new wxListBox(this, ID_LISTBOX1, wxPoint(8,8), wxSize(580,400), 0, 0, wxLB_EXTENDED|wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT, wxDefaultValidator, _T("ID_LISTBOX1"));
    btnRemoveAll = new wxButton(this, ID_BUTTON1, _("Remove all"), wxPoint(600,88), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    btnAdd = new wxButton(this, ID_BUTTON2, _("Add"), wxPoint(600,8), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    btnRemove = new wxButton(this, ID_BUTTON3, _("Remove"), wxPoint(600,48), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    FileDialog1 = new wxFileDialog(this, _("Select file"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE|wxFD_CHANGE_DIR, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));

    Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&kwencryptFrame::OnListBox1Select);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnRemoveAllClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnAddClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnRemoveClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&kwencryptFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&kwencryptFrame::OnAbout);
    //*)


    SetDropTarget(new DnDialogFile(this));
    printf("frame initial success!\n");

}

kwencryptFrame::~kwencryptFrame()
{
    //(*Destroy(kwencryptFrame)
    //*)
    printf("frame closed!\n");
    //system("pause");
}

void kwencryptFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void kwencryptFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to kw-encryptor"));
}


void kwencryptFrame::OnClose(wxCloseEvent& event)
{
}

void kwencryptFrame::OnListBox1Select(wxCommandEvent& event)
{
}

// 清除选中文件
void kwencryptFrame::OnbtnRemoveClick(wxCommandEvent& event)
{
    wxArrayInt selects;
    int n = ListBox1->GetSelections(selects);
    for (int i = n - 1; i >= 0; i--) {
        ListBox1->Delete(selects[i]);
    }
}

// 添加文件
void kwencryptFrame::OnbtnAddClick(wxCommandEvent& event)
{
    if (FileDialog1->ShowModal() == wxID_CANCEL) return;
    wxArrayString filepaths;
    FileDialog1->GetPaths(filepaths);



    wxMessageOutputStderr out = wxMessageOutputStderr(stdout);
    wxArrayString unExistsFiles;
    for (size_t i = 0; i < filepaths.GetCount(); i++) {
        wxFileName file_to_check = filepaths[i];
        file_to_check.Normalize(wxPATH_NORM_ALL); // 解析快捷方式，获取完整路径 https://forums.wxwidgets.org/viewtopic.php?t=13529
        if (wxFileName::FileExists(file_to_check.GetFullPath())) {
            ListBox1->Append(file_to_check.GetFullPath());
            out.Printf("%s\n", file_to_check.GetFullPath());
        } else {
            unExistsFiles.Add(file_to_check.GetFullPath());
        }
    }

    // 检查是否存在无效的快捷方式，并输出添加结果
    if (!unExistsFiles.IsEmpty()) {
        StatusBar1->SetLabel(wxString("") << filepaths.GetCount() - unExistsFiles.GetCount()
                                    << " file(s) successfully added, "
                                    << unExistsFiles.GetCount() << " invalid has been excluded!");
    } else {
        StatusBar1->SetLabel(wxString("") << filepaths.GetCount() - unExistsFiles.GetCount()
                                    << " file(s) successfully added!");
    }

    // 消除重复项
    for (size_t i = ListBox1->GetCount() - 1; i >= 1; i--) {
        if (ListBox1->GetString(i) == ListBox1->GetString(i - 1)) ListBox1->Delete(i);
    }
}

// 清除所有文件
void kwencryptFrame::OnbtnRemoveAllClick(wxCommandEvent& event)
{
    ListBox1->Clear();
}
