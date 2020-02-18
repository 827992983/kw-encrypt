#include "DnDialogFile.h"
#include <wx/listbox.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

// 拖放文件触发该函数
bool DnDialogFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filepaths)
{
    wxListBox *box = frame->ListBox1;
    wxMessageOutputStderr out = wxMessageOutputStderr(stdout);
    wxArrayString unExistsFiles;
    for (size_t i = 0; i < filepaths.GetCount(); i++) {
        wxFileName file_to_check = filepaths[i];
        file_to_check.Normalize(wxPATH_NORM_ALL); // 解析快捷方式，获取完整路径 https://forums.wxwidgets.org/viewtopic.php?t=13529
        if (wxFileName::FileExists(file_to_check.GetFullPath())) {
            box->Append(file_to_check.GetFullPath());
            out.Printf("%s\n", file_to_check.GetFullPath());
        } else {
            unExistsFiles.Add(file_to_check.GetFullPath());
        }
    }

    // 检查是否存在无效的快捷方式，并输出添加结果
    if (!unExistsFiles.IsEmpty()) {
        frame->StatusBar1->SetLabel(wxString("") << filepaths.GetCount() - unExistsFiles.GetCount()
                                    << " file(s) successfully added, "
                                    << unExistsFiles.GetCount() << " invalid has been excluded!");
    } else {
        frame->StatusBar1->SetLabel(wxString("") << filepaths.GetCount() - unExistsFiles.GetCount()
                                    << " file(s) successfully added!");
    }

    // 消除重复项
    if (!box->IsEmpty()) {
        for (size_t i = box->GetCount() - 1; i >= 1; i--) {
            if (box->GetString(i) == box->GetString(i - 1)) box->Delete(i);
        }
    }


    return true;
}

