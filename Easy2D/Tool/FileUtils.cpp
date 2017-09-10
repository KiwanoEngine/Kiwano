#include "..\Easy2d.h"
#include "..\EasyX\easyx.h"
#include <algorithm>


tstring FileUtils::getFileExtension(const tstring & filePath)
{
	tstring fileExtension;
	// 找到文件名中的最后一个 '.' 的位置
	size_t pos = filePath.find_last_of('.');
	// 判断 pos 是否是个有效位置
	if (pos != tstring::npos)
	{
		// 截取扩展名
		fileExtension = filePath.substr(pos, filePath.length());
		// 转换为小写字母
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
	}

	return fileExtension;
}

bool FileUtils::getSaveFilePath(tstring& path, LPCTSTR title, LPCTSTR defExt)
{
	// 弹出保存对话框
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };				// 用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);				// 结构体大小
	ofn.hwndOwner = GetHWnd();							// 拥有着窗口句柄，NULL 表示对话框是非模态的
	ofn.lpstrFilter = _T("所有文件\0*.*\0\0");			// 设置过滤
	ofn.nFilterIndex = 1;								// 过滤器索引
	ofn.lpstrFile = strFilename;						// 接收返回的文件路径和文件名
	ofn.nMaxFile = sizeof(strFilename);					// 缓冲区长度
	ofn.lpstrInitialDir = NULL;							// 初始目录为默认
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;// 目录必须存在，覆盖文件前发出警告
	ofn.lpstrTitle = title;								// 使用系统默认标题留空即可
	ofn.lpstrDefExt = defExt;							// 默认追加的扩展名

	if (GetSaveFileName(&ofn))
	{
		path = strFilename;
		return true;
	}
	return false;
}
