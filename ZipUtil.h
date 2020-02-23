#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>

class ZipUtil {
public:
    static const int SUCCESSFUL = 0;
    static const int ERROR_WRITE_FILE_FAILED = 1;
    static const int ERROR_READ_FILE_FAILED = 2;
    static const int ERROR_FILE_CLOSE_FAILED = 3;

    /**
    ����
    files��Դ�ļ���ԴĿ¼�ľ���·������Щ·�����û���ק���öԻ���ѡȡ��Ŀ¼·�������������ļ�����Ŀ¼��·��
    outputPath��ѹ���ļ����������·��
    **/
    static int writeToZip(wxArrayString files, wxString outputPath) {
        // ɾ��ԭ����ļ�
        if (wxFileExists(outputPath)) wxRemoveFile(outputPath);
        // ��ʼ�� wxZipOutputStream
        wxFFileOutputStream out(wxFileName(outputPath).GetFullPath());
        if (!out.IsOk()) {
            out.Close();
            return ERROR_WRITE_FILE_FAILED;
        }
        wxZipOutputStream zip( out );
        if (!zip.IsOk()) {
            out.Close();
            zip.Close();
            return ERROR_WRITE_FILE_FAILED;
        }
        // ����Դ�ļ�
        for (size_t i = 0; i < files.GetCount(); i++) {
            wxFileName fname(files[i]);
            if (!fname.Exists()) {
                out.Close();
                zip.Close();
                return ERROR_READ_FILE_FAILED;
            }
            if (fname.FileExists()) {
                // �ļ�ֱ��д�뵽ѹ���ļ���
                wxFileInputStream in(fname.GetFullPath());
                if (!in.IsOk()) {
                    out.Close();
                    zip.Close();
                    return ERROR_READ_FILE_FAILED;
                }
                zip.PutNextEntry(fname.GetFullName());
                zip.Write(in);
            } else {
//                // Ŀ¼��ɾ����·����д�룬���� D:\\Folder1\\Folder2\\ �Ͱ� D:\\Folder1\\ ��Ϊǰ׺ɾ��
                wxArrayString folder;
                wxString prePath = fname.GetPath(); // Ҫɾ����ǰ׺·��
                wxString parentPath = fname.GetFullPath();
                parentPath.Replace(prePath, ""); // ɾ��ǰ׺
                zip.PutNextDirEntry(parentPath);
                wxDirTraverserImplement traverser(folder);
                wxDir dir(fname.GetFullPath());
                dir.Traverse(traverser);

                for (size_t j = 0; j < folder.GetCount(); j++) {
                    wxFileName fname2(folder[j]);
                    if (fname2.FileExists()) {
                        wxFileInputStream in(fname2.GetFullPath());
                        if (!in.IsOk()) {
                            out.Close();
                            zip.Close();
                            return ERROR_READ_FILE_FAILED;
                        }
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // ɾ��ǰ׺
                        zip.PutNextEntry(path);
                        zip.Write(in);
                    } else {
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // ɾ��ǰ׺
                        zip.PutNextDirEntry(path);
                    }
                }
            }
        }
        if (!zip.Close()) return ERROR_FILE_CLOSE_FAILED;
        return SUCCESSFUL;
    }
};


