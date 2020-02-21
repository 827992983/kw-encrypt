/***************************************************************
 * Name:      kwencryptMain.h
 * Purpose:   Defines Application Frame
 * Author:    kw ()
 * Created:   2020-02-16
 * Copyright: kw ()
 * License:
 **************************************************************/

#ifndef KWENCRYPTMAIN_H
#define KWENCRYPTMAIN_H


//(*Headers(kwencryptFrame)
#include <wx/button.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/progdlg.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/textdlg.h>
//*)
#include "FileItemMenu.h"

class kwencryptFrame: public wxFrame
{
    friend class DnDialogFile;
    public:

        kwencryptFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~kwencryptFrame();

    private:

        //(*Handlers(kwencryptFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnListBox1Select(wxCommandEvent& event);
        void OnbtnRemoveClick(wxCommandEvent& event);
        void OnbtnAddFileClick(wxCommandEvent& event);
        void OnbtnRemoveAllOriginFilesClick(wxCommandEvent& event);
        void OnbtnRemoveClick1(wxCommandEvent& event);
        void OnbtnEncryptClick(wxCommandEvent& event);
        void OnbtnAddFolderClick(wxCommandEvent& event);
        void OnButton3Click(wxCommandEvent& event);
        void OnlistOriginFilesItemRClick(wxListEvent& event);
        void OnlistOriginFilesItemSelect(wxListEvent& event);
        void OnlistOriginFilesItemDeselect(wxListEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        //*)
        void OnMenuItemCopyOriginalFileSelected(wxCommandEvent& event);
        void OnMenuItemRemoveOriginalFileSelected(wxCommandEvent& event);
        void OnMenuItemOpenOriginalFileInExplorerSelected(wxCommandEvent& event);

        //(*Identifiers(kwencryptFrame)
        static const long ID_LISTCTRL1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON5;
        static const long ID_BUTTON3;
        static const long ID_BUTTON1;
        static const long ID_BUTTON4;
        static const long ID_BUTTON6;
        static const long ID_PANEL2;
        static const long ID_PANEL1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long ID_PASSWORDENTRYDIALOG1;
        static const long ID_PROGRESSDIALOG1;
        //*)
        static const long ID_MENU_ORIGINAL_FILE_COPY;
        static const long ID_MENU_ORIGINAL_FILE_REMOVE;
        static const long ID_MENU_ORIGINAL_FILE_OPEN_IN_FILE_EXPLORER;


        //(*Declarations(kwencryptFrame)
        wxButton* btnAddFile;
        wxButton* btnAddFolder;
        wxButton* btnDecrypt;
        wxButton* btnEncrypt;
        wxButton* btnRemove;
        wxButton* btnRemoveAllOriginFiles;
        wxDirDialog* DirDialog1;
        wxFileDialog* FileDialog1;
        wxFileDialog* FileDialog2;
        wxListCtrl* listOriginFiles;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxPasswordEntryDialog* PasswordEntryDialog1;
        wxProgressDialog* ProgressDialog1;
        wxStatusBar* StatusBar1;
        //*)
        FileItemMenu *fileItemMenu;
        DECLARE_EVENT_TABLE()
};


#endif // KWENCRYPTMAIN_H
