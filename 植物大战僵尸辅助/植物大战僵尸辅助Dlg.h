
// 植物大战僵尸辅助Dlg.h: 头文件
//

#pragma once


// C植物大战僵尸辅助Dlg 对话框
class C植物大战僵尸辅助Dlg : public CDialogEx
{
// 构造
public:
	C植物大战僵尸辅助Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	friend DWORD monitorFunc(LPVOID lpThreadParameter);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	


	// 在头文件里进行声明自己自定义的函数！
	//  afx_msg 代表 标注该声明的实现 是事件处理函数！！
	afx_msg void C植物大战僵尸辅助Dlg::onBtnClickedCourse();
// public:
	afx_msg void OnBnClickedkill();
//public:
	afx_msg void OnBnClickedSun();
private:
	// 无限阳光
	CButton m_bnSun;


	// 保存线程的句柄（用全局变量，不会回收）
	HANDLE m_monitorThread;

};
