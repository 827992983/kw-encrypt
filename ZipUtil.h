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
    参数
    files：源文件和源目录的绝对路径，这些路径由用户拖拽或用对话框选取，目录路径不包含其子文件、子目录的路径
    outputPath：压缩文件的输出绝对路径
    **/
    static int writeToZip(wxArrayString files, wxString outputPath) {
        // 删除原输出文件
        if (wxFileExists(outputPath)) wxRemoveFile(outputPath);
        // 初始化 wxZipOutputStream
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
        // 遍历源文件
        for (size_t i = 0; i < files.GetCount(); i++) {
            wxFileName fname(files[i]);
            if (!fname.Exists()) {
                out.Close();
                zip.Close();
                return ERROR_READ_FILE_FAILED;
            }
            if (fname.FileExists()) {
                // 文件直接写入到压缩文件中
                wxFileInputStream in(fname.GetFullPath());
                if (!in.IsOk()) {
                    out.Close();
                    zip.Close();
                    return ERROR_READ_FILE_FAILED;
                }
                zip.PutNextEntry(fname.GetFullName());
                zip.Write(in);
            } else {
//                // 目录则删除父路径后写入，比如 D:\\Folder1\\Folder2\\ 就把 D:\\Folder1\\ 作为前缀删掉
                wxArrayString folder;
                wxString prePath = fname.GetPath(); // 要删除的前缀路径
                wxString parentPath = fname.GetFullPath();
                parentPath.Replace(prePath, ""); // 删除前缀
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
                        path.Replace(prePath, ""); // 删除前缀
                        zip.PutNextEntry(path);
                        zip.Write(in);
                    } else {
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // 删除前缀
                        zip.PutNextDirEntry(path);
                    }
                }
            }
        }
        if (!zip.Close()) return ERROR_FILE_CLOSE_FAILED;
        return SUCCESSFUL;
    }
};


