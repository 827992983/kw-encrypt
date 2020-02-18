#include "DnDialogFile.h"
#include <wx/listbox.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

// �Ϸ��ļ������ú���
bool DnDialogFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filepaths)
{
    wxListBox *box = frame->ListBox1;
    wxMessageOutputStderr out = wxMessageOutputStderr(stdout);
    wxArrayString unExistsFiles;
    for (size_t i = 0; i < filepaths.GetCount(); i++) {
        wxFileName file_to_check = filepaths[i];
        file_to_check.Normalize(wxPATH_NORM_ALL); // ������ݷ�ʽ����ȡ����·�� https://forums.wxwidgets.org/viewtopic.php?t=13529
        if (wxFileName::FileExists(file_to_check.GetFullPath())) {
            box->Append(file_to_check.GetFullPath());
            out.Printf("%s\n", file_to_check.GetFullPath());
        } else {
            unExistsFiles.Add(file_to_check.GetFullPath());
        }
    }

    // ����Ƿ������Ч�Ŀ�ݷ�ʽ���������ӽ��
    if (!unExistsFiles.IsEmpty()) {
        frame->StatusBar1->SetLabel(wxString("") << filepaths.GetCount() - unExistsFiles.GetCount()
                                    << " file(s) successfully added, "
                                    << unExistsFiles.GetCount() << " invalid has been excluded!");
    } else {
        frame->StatusBar1->SetLabel(wxString("") << filepaths.GetCount() - unExistsFiles.GetCount()
                                    << " file(s) successfully added!");
    }

    // �����ظ���
    if (!box->IsEmpty()) {
        for (size_t i = box->GetCount() - 1; i >= 1; i--) {
            if (box->GetString(i) == box->GetString(i - 1)) box->Delete(i);
        }
    }


    return true;
}

