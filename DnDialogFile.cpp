#include "DnDialogFile.h"
#include <wx/listbox.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

// 拖放文件触发该函数
bool DnDialogFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filepaths)
{
    wxListBox *box = frame->ListBox1;
    wxMessageOutputStderr out = wxMessageOutputStderr(stdout);
    //wxArrayString unExistsFiles;
    for (size_t i = 0; i < filepaths.GetCount(); i++) {
        wxFileName file_to_check = filepaths[i];
        box->Append(file_to_check.GetFullPath());
        if (!file_to_check.FileExists()) {
            out.Printf("[Log] Folder: %s\n", file_to_check.GetFullPath());
        } else {
            out.Printf("[Log] File: %s\n", file_to_check.GetFullPath());
        }

    }

    // 消除重复项
    if (!box->IsEmpty()) {
        for (size_t i = box->GetCount() - 1; i >= 1; i--) {
            if (box->GetString(i) == box->GetString(i - 1)) box->Delete(i);
        }
    }


    return true;
}

