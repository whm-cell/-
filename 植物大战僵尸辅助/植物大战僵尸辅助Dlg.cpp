
// 植物大战僵尸辅助Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "植物大战僵尸辅助.h"
#include "植物大战僵尸辅助Dlg.h"
#include "afxdialogex.h"
#include <iostream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 斜线代表 下边的代码属于上边的  宏
#define log(fmt, ...) \
CString str; \
str.Format(CString(fmt), __VA_ARGS__);\
AfxMessageBox(str);

// 加static 表示 只允许当前类可以访问
static C植物大战僵尸辅助Dlg* g_glg;

static HANDLE g_processHandle;


// 将某个值写入植物大战僵尸内存（后面的可变参数是地址链，要以-1结尾）
void WriteMemory(void* value, DWORD valueSize, ...) {
	if (value == NULL || valueSize == 0 || g_processHandle == NULL) return;

	DWORD tempValue = 0;

	va_list addresses;
	va_start(addresses, valueSize);
	DWORD offset = 0;
	DWORD lastAddress = 0;
	while ((offset = va_arg(addresses, DWORD)) != -1) {
		lastAddress = tempValue + offset;
		::ReadProcessMemory(g_processHandle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
	}
	va_end(addresses);

	::WriteProcessMemory(g_processHandle, (LPVOID)lastAddress, value, valueSize, NULL);
}

void WriteMemory(void* value, DWORD valueSize, DWORD address) {
	WriteMemory(value, valueSize, address, -1);
}





/*
	用来监控游戏的线程！
*/
DWORD monitorFunc(LPVOID lpThreadParameter) {

	while (1) {
		// 这里判断植物大战僵尸是否打开
		// 使用windows函数判断游戏是否打开 FindWindow
		// 如何找应用的  名称

		// gameHANDLE 为游戏植物大战僵尸（窗口）的句柄
		HWND game窗口Handle = FindWindow(CString("MainWindow"), CString("植物大战僵尸中文版"));
		
		if (game窗口Handle == NULL) {
			g_glg->m_bnSun.SetCheck(FALSE);
			g_glg->m_bnKill.SetCheck(FALSE);
			g_glg->m_bnSun.EnableWindow(FALSE);
			g_glg->m_bnKill.EnableWindow(FALSE);

			// 清空  g_processhandle 保证 elseif 执行的唯一性
			g_processHandle = NULL;

		}else if(g_processHandle == NULL) { // 保证全局函数  进程句柄为空的时候，才会浸入！
			g_glg->m_bnSun.EnableWindow(TRUE);
			g_glg->m_bnKill.EnableWindow(TRUE);

			// 获取植物大战僵尸的进程ID
			DWORD pid;

			// PID 先定义了4个字节 ， 然后把地址传递进下边的方法内 ，然后最后会给 pid的内存地址赋值
			GetWindowThreadProcessId(game窗口Handle, &pid);

			// 获取进行ID 
			//  PROCESS_ALL_ACCESS 权限 
			//  获取职务大战僵尸的句柄！
			g_processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

		}
		// 睡眠一秒钟
		Sleep(1000);
	
	}

	// NULL == 0
	return NULL;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C植物大战僵尸辅助Dlg 对话框



C植物大战僵尸辅助Dlg::C植物大战僵尸辅助Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/*
	
	主动 - （将页面的按钮事件ID 和 自定义的CButton绑定在一起！）

*/
void C植物大战僵尸辅助Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SUN, m_bnSun);

	DDX_Control(pDX, IDC_kill, m_bnKill);
}

BEGIN_MESSAGE_MAP(C植物大战僵尸辅助Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// 下边传递 ID 和 函数名
	// &C植物大战僵尸辅助Dlg 前面可以写 &符号，取出函数地址！！！（这里的&可以省略）
	ON_BN_CLICKED(IDC_COURSE, &C植物大战僵尸辅助Dlg::onBtnClickedCourse)
	ON_BN_CLICKED(IDC_kill, &C植物大战僵尸辅助Dlg::OnBnClickedkill)
	ON_BN_CLICKED(IDC_SUN, &C植物大战僵尸辅助Dlg::OnBnClickedSun)
END_MESSAGE_MAP()


// C植物大战僵尸辅助Dlg 消息处理程序

BOOL C植物大战僵尸辅助Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 在这里可以添加  多线程代码 （监控游戏的打开或者关闭！！）
	/*
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
		_In_ SIZE_T dwStackSize,
		_In_ LPTHREAD_START_ROUTINE lpStartAddress,
		_In_opt_ __drv_aliasesMem LPVOID lpParameter,
		_In_ DWORD dwCreationFlags,
		_Out_opt_ LPDWORD lpThreadId
	*/

	m_monitorThread = CreateThread(NULL, NULL, monitorFunc, NULL, NULL, NULL);

	// 这里保存下 对象， 让 全局函数可以访问到全局变量
	g_glg = this;


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C植物大战僵尸辅助Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C植物大战僵尸辅助Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C植物大战僵尸辅助Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 写一个函数刦处理想做的事情 (实现 -  声明在对应的头文件)
void C植物大战僵尸辅助Dlg::onBtnClickedCourse() {
	// 这里可以使用cout  但是必须引入  iostream
	// 这里无法看到打印的原因 ， 是因为 cout只适用于 “命令行项目”
	// cout << "C植物大战僵尸辅助Dlg::onBtnClickedCours" << endl;



	// 打开外部url：
	/*
	_In_opt_ HWND hwnd, 
	_In_opt_ LPCWSTR lpOperation, 
	_In_ LPCWSTR lpFile, 
	_In_opt_ LPCWSTR lpParameters,
    _In_opt_ LPCWSTR lpDirectory, 
	_In_ INT nShowCmd
	*/
	// 两个冒号是全局作用域
	::ShellExecute(NULL,
		CString("open"),
		CString("https://blog.csdn.net/lq724445056/article/details/123655259"),
		NULL, NULL, SW_NORMAL);


	// int age = 19;

	//log("大家好  my age si %d\n",10);

	// 方式 1 
	// int age = 10;
	// TRACE(" 年龄是 %d\n", age);

	// 方式2 
	// AfxMessageBox(CString("age is 十大：  "+age));

	// 打印带有替换符的！(这个可以自动识别中文字体)
	/*CString str;
	str.Format(CString("age is aa啊  %d"), age);

	AfxMessageBox(str);*/

	
	// 方式3 
	// CString str;
	// str.Format(CString("age is aa啊  %d"), age);
	// demo1
	//MessageBox(str);

	// demo2 (可以出现  确认取消的二次确认狂)
	// MessageBox(str, CString("警告！！"), MB_YESNOCANCEL | MB_ICONWARNING);
}

void C植物大战僵尸辅助Dlg::OnBnClickedkill()
{
	BOOL s = m_bnKill.GetCheck();

	if (s) {
		// true的时候  就需要秒杀僵尸了！
		// 写进程内存，写进程句柄。 
		// 需要拿到植物大战僵尸的句柄

		// 需要写入的字节
		BYTE data[] = {0xFF,0x90 ,0x90 };


		// 0x00531319 为开始写内存的起始 内存地址（这个操作是将只读的16进制字节写入（覆盖）到从0x00531319开始的后三个字节内）

		WriteMemory(&data, sizeof(data), 0x00531310);

	}
	else {
		// false 不需要秒杀僵尸
			// 需要写入的字节
		BYTE data[] = { 0x7c,0x24 ,0x20 };


		// 
		WriteMemory(&data, sizeof(data), 0x00531310);
	}








	// TODO: 在此添加控件通知处理程序代码

	// log("ajs");

	// auto  bool 
	// 获取事件信息  
	// 方式1
	/*BOOL t = IsDlgButtonChecked(IDC_kill);

	if (t) {
		log("勾选了 ");
	}
	else {
		log("没有勾选");
	}*/

	// 方式2 (获取对话框里的所有的东西！！！)
	// GetDlgItem是返回 对话框的地址！！（需要用指针接收）


	/*CButton * c = (CButton *)GetDlgItem(IDC_kill);
	if (c ->GetCheck()) {
		log("勾选了 ");
	}
	else {
		log("没有勾选");
	}*/


	// 

}


void C植物大战僵尸辅助Dlg::OnBnClickedSun()
{
	// TODO: 在此添加控件通知处理程序代码


	/*BOOL s = m_bnSun.GetCheck();

	if (s) {
		log("勾选了 ");
	}
	else {
		log("没有勾选");
	}*/
}
