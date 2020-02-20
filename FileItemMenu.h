#ifndef FILEITEMMENU_H
#define FILEITEMMENU_H

#include <wx/menu.h>

class FileItemMenu : public wxMenu
{
    public:
        FileItemMenu();
        int itemIndex; // 文件列表下标
};

#endif // FILEITEMMENU_H
