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
const long kwencryptFrame::ID_PANEL2 = wxNewId();
const long kwencryptFrame::ID_BUTTON2 = wxNewId();
const long kwencryptFrame::ID_BUTTON3 = wxNewId();
const long kwencryptFrame::ID_BUTTON1 = wxNewId();
const long kwencryptFrame::ID_STATICLINE1 = wxNewId();
const long kwencryptFrame::ID_BUTTON4 = wxNewId();
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
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, wxID_ANY, _("kw-encryptor"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLIP_CHILDREN, _T("wxID_ANY"));
    SetClientSize(wxSize(932,491));
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Panel2 = new wxPanel(Panel1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    ListBox1 = new wxListBox(Panel2, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_EXTENDED|wxLB_NEEDED_SB|wxLB_SORT, wxDefaultValidator, _T("ID_LISTBOX1"));
    ListBox1->SetToolTip(_("Drag and drop files here to add files"));
    BoxSizer2->Add(ListBox1, 1, wxEXPAND, 0);
    Panel2->SetSizer(BoxSizer2);
    BoxSizer2->Fit(Panel2);
    BoxSizer2->SetSizeHints(Panel2);
    BoxSizer1->Add(Panel2, 5, wxALL|wxEXPAND, 5);
    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    btnAdd = new wxButton(Panel1, ID_BUTTON2, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer3->Add(btnAdd, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 1);
    btnRemove = new wxButton(Panel1, ID_BUTTON3, _("Remove"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer3->Add(btnRemove, 0, wxALL|wxEXPAND, 1);
    btnRemoveAll = new wxButton(Panel1, ID_BUTTON1, _("Remove all"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer3->Add(btnRemoveAll, 0, wxALL|wxEXPAND, 1);
    StaticLine1 = new wxStaticLine(Panel1, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    BoxSizer3->Add(StaticLine1, 0, wxALL|wxEXPAND, 3);
    btnEncrypt = new wxButton(Panel1, ID_BUTTON4, _("Encrypt"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    btnEncrypt->Disable();
    BoxSizer3->Add(btnEncrypt, 0, wxALL|wxEXPAND, 1);
    BoxSizer1->Add(BoxSizer3, 1, wxTOP|wxBOTTOM|wxRIGHT|wxEXPAND, 5);
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
    FileDialog1 = new wxFileDialog(this, _("Select file"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE|wxFD_CHANGE_DIR, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));

    Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&kwencryptFrame::OnListBox1Select);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnAddClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnRemoveClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnRemoveAllClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&kwencryptFrame::OnbtnEncryptClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&kwencryptFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&kwencryptFrame::OnAbout);
    //*)


    SetDropTarget(new DnDialogFile(this)); // �ϷŶԻ���
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

// ���ѡ���ļ�
void kwencryptFrame::OnbtnRemoveClick(wxCommandEvent& event)
{
    wxArrayInt selects;
    int n = ListBox1->GetSelections(selects);
    for (int i = n - 1; i >= 0; i--) {
        ListBox1->Delete(selects[i]);
    }
}

// ����ļ�
void kwencryptFrame::OnbtnAddClick(wxCommandEvent& event)
{
    if (FileDialog1->ShowModal() == wxID_CANCEL) return;
    wxArrayString filepaths;
    FileDialog1->GetPaths(filepaths);



    wxMessageOutputStderr out = wxMessageOutputStderr(stdout);
    wxArrayString unExistsFiles;
    for (size_t i = 0; i < filepaths.GetCount(); i++) {
        wxFileName file_to_check = filepaths[i];
        file_to_check.Normalize(wxPATH_NORM_ALL); // ������ݷ�ʽ����ȡ����·�� https://forums.wxwidgets.org/viewtopic.php?t=13529
        if (wxFileName::FileExists(file_to_check.GetFullPath())) {
            ListBox1->Append(file_to_check.GetFullPath());
            out.Printf("%s\n", file_to_check.GetFullPath());
        } else {
            unExistsFiles.Add(file_to_check.GetFullPath());
        }
    }

    // ����Ƿ������Ч�Ŀ�ݷ�ʽ���������ӽ��
    if (!unExistsFiles.IsEmpty()) {
        StatusBar1->SetLabel(wxString("") << filepaths.GetCount() - unExistsFiles.GetCount()
                                    << " file(s) successfully added, "
                                    << unExistsFiles.GetCount() << " invalid has been excluded!");
    } else {
        StatusBar1->SetLabel(wxString("") << filepaths.GetCount() - unExistsFiles.GetCount()
                                    << " file(s) successfully added!");
    }

    // �����ظ���
    if (!ListBox1->IsEmpty()) {
        for (size_t i = ListBox1->GetCount() - 1; i >= 1; i--) {
            if (ListBox1->GetString(i) == ListBox1->GetString(i - 1)) ListBox1->Delete(i);
        }
    }
}

// ��������ļ�
void kwencryptFrame::OnbtnRemoveAllClick(wxCommandEvent& event)
{
    ListBox1->Clear();
}

// �����ļ�
void kwencryptFrame::OnbtnEncryptClick(wxCommandEvent& event)
{
}
