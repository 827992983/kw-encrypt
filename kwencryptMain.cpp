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
const long kwencryptFrame::ID_BUTTON2 = wxNewId();
const long kwencryptFrame::ID_BUTTON5 = wxNewId();
const long kwencryptFrame::ID_BUTTON3 = wxNewId();
const long kwencryptFrame::ID_BUTTON1 = wxNewId();
const long kwencryptFrame::ID_BUTTON4 = wxNewId();
const long kwencryptFrame::ID_LISTBOX2 = wxNewId();
const long kwencryptFrame::ID_BUTTON6 = wxNewId();
const long kwencryptFrame::ID_BUTTON7 = wxNewId();
const long kwencryptFrame::ID_BUTTON9 = wxNewId();
const long kwencryptFrame::ID_PANEL2 = wxNewId();
const long kwencryptFrame::ID_PANEL1 = wxNewId();
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
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer4;
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxStaticBoxSizer* StaticBoxSizer2;

    Create(parent, wxID_ANY, _("kw-encryptor"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLIP_CHILDREN, _T("wxID_ANY"));
    SetClientSize(wxSize(800,492));
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Panel2 = new wxPanel(Panel1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, Panel2, _("Drag the files you want to encrypt to the box below"));
    ListBox1 = new wxListBox(Panel2, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_EXTENDED|wxLB_NEEDED_SB|wxLB_SORT, wxDefaultValidator, _T("ID_LISTBOX1"));
    StaticBoxSizer1->Add(ListBox1, 5, wxALL|wxEXPAND, 1);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    btnAddFile = new wxButton(Panel2, ID_BUTTON2, _("Choose File"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer4->Add(btnAddFile, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 1);
    btnAddFolder = new wxButton(Panel2, ID_BUTTON5, _("Choose Folder"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    BoxSizer4->Add(btnAddFolder, 0, wxALL|wxEXPAND, 1);
    btnRemove = new wxButton(Panel2, ID_BUTTON3, _("Remove"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer4->Add(btnRemove, 0, wxALL|wxEXPAND, 1);
    btnRemoveAll = new wxButton(Panel2, ID_BUTTON1, _("Remove All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer4->Add(btnRemoveAll, 0, wxALL|wxEXPAND, 1);
    BoxSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnEncrypt = new wxButton(Panel2, ID_BUTTON4, _("Encrypt"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    btnEncrypt->Disable();
    BoxSizer4->Add(btnEncrypt, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
    StaticBoxSizer1->Add(BoxSizer4, 1, wxALL|wxEXPAND, 0);
    BoxSizer2->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, Panel2, _("Drag a file you want to decrypt to the box below"));
    ListBox2 = new wxListBox(Panel2, ID_LISTBOX2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX2"));
    StaticBoxSizer2->Add(ListBox2, 5, wxALL|wxEXPAND, 1);
    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    Button1 = new wxButton(Panel2, ID_BUTTON6, _("Choose .kwe File"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    BoxSizer3->Add(Button1, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 1);
    Button2 = new wxButton(Panel2, ID_BUTTON7, _("Remove"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    BoxSizer3->Add(Button2, 0, wxALL|wxEXPAND, 1);
    BoxSizer3->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button4 = new wxButton(Panel2, ID_BUTTON9, _("Decrypt"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
    BoxSizer3->Add(Button4, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
    StaticBoxSizer2->Add(BoxSizer3, 1, wxALL|wxEXPAND, 0);
    BoxSizer2->Add(StaticBoxSizer2, 1, wxALL|wxEXPAND, 5);
    Panel2->SetSizer(BoxSizer2);
    BoxSizer2->Fit(Panel2);
    BoxSizer2->SetSizeHints(Panel2);
    BoxSizer1->Add(Panel2, 5, wxALL|wxEXPAND, 5);
    Panel1->SetSizer(BoxSizer1);
    BoxSizer1->Fit(Panel1);
    BoxSizer1->SetSizeHints(Panel1);
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
    FileDialog1 = new wxFileDialog(this, _("Select file"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
    DirDialog1 = new wxDirDialog(this, _("Select directory"), wxEmptyString, wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxDirDialog"));

    Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&kwencryptFrame::OnListBox1Select);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnAddFileClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnAddFolderClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnRemoveClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnRemoveAllClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnEncryptClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&kwencryptFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&kwencryptFrame::OnAbout);
    //*)


    ListBox1->SetDropTarget(new DnDialogFile(this)); // 拖放对话框
    printf("[Log] Window initialization successful!\n");
}

kwencryptFrame::~kwencryptFrame()
{
    //(*Destroy(kwencryptFrame)
    //*)
    printf("[Log] Window closed!\n");

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

// 添加单个文件
void kwencryptFrame::OnbtnAddFileClick(wxCommandEvent& event)
{
    if (FileDialog1->ShowModal() == wxID_CANCEL) return;
    wxString filepath = FileDialog1->GetPath();
    ListBox1->Append(filepath);
    // 消除重复项
    if (!ListBox1->IsEmpty()) {
        for (size_t i = ListBox1->GetCount() - 1; i >= 1; i--) {
            if (ListBox1->GetString(i) == ListBox1->GetString(i - 1)) ListBox1->Delete(i);
        }
    }
}

// 添加单个目录
void kwencryptFrame::OnbtnAddFolderClick(wxCommandEvent& event)
{
    if (DirDialog1->ShowModal() == wxID_CANCEL) return;
    wxString folderPath = DirDialog1->GetPath();
    ListBox1->Append(folderPath);
    // 消除重复项
    if (!ListBox1->IsEmpty()) {
        for (size_t i = ListBox1->GetCount() - 1; i >= 1; i--) {
            if (ListBox1->GetString(i) == ListBox1->GetString(i - 1)) ListBox1->Delete(i);
        }
    }
}

// 清除所有文件
void kwencryptFrame::OnbtnRemoveAllClick(wxCommandEvent& event)
{
    ListBox1->Clear();
}

// 加密文件
void kwencryptFrame::OnbtnEncryptClick(wxCommandEvent& event)
{
}


