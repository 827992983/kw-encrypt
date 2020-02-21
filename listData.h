/**
存列表数据，对列表的操作也通过这个文件定义的API，如添加元素、清空列表。
**/
#ifndef LISTDATA_H
#define LISTDATA_H


#include <vector>
#include <wx/string.h>
#include <wx/filename.h>
#include <wx/listctrl.h>
#include <algorithm>
#include <wx/icon.h>
#include <wx/imaglist.h>

class FileItem {
public:
    FileItem(wxFileName n, wxString s, wxString t) : fileName(n), fileSize(s), fileType(t) {}
    wxFileName fileName;
    wxString fileSize;
    wxString fileType;
    bool selected = false;
};

extern std::vector<FileItem> fileItems;


// 对list排序并去重，然后填充listctrl
void overwriteListCtrl(wxListCtrl *listCtrl, std::vector<FileItem> &files);

#endif // LISTDATA_H
