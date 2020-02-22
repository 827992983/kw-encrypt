#ifndef WXDIRTRAVERSERIMPLEMENT_H
#define WXDIRTRAVERSERIMPLEMENT_H

#include <wx/dir.h>

class wxDirTraverserImplement : public wxDirTraverser
{
public:
    wxDirTraverserImplement(wxArrayString& files) : m_files(files) { }
    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        m_files.Add(filename);
        return wxDIR_CONTINUE;
    }
    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        m_files.Add(dirname);
        return wxDIR_CONTINUE;
    }
private:
    wxArrayString& m_files;
};

#endif // WXDIRTRAVERSERIMPLEMENT_H
