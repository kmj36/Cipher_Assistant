
// Cipher_AssistantDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Cipher_Assistant.h"
#include "Cipher_AssistantDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CCipherAssistantDlg 대화 상자



CCipherAssistantDlg::CCipherAssistantDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIPHER_ASSISTANT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCipherAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, plaintext);
	DDX_Control(pDX, IDC_EDIT8, MD5textbox);
	DDX_Control(pDX, IDC_EDIT7, SHA256textbox);
	DDX_Control(pDX, IDC_EDIT9, AESkeytextbox);
	DDX_Control(pDX, IDC_EDIT6, AEStextbox);
	DDX_Control(pDX, IDC_EDIT4, Decrypttextbox);
	DDX_Control(pDX, IDC_EDIT5, Publictextbox);
	DDX_Control(pDX, IDC_EDIT3, RSAtextbox);
	DDX_Control(pDX, IDC_EDIT2, Base64textbox);
	DDX_Control(pDX, IDC_PROGRESS1, progress);
	DDX_Control(pDX, IDC_BUTTON1, progressstatic);
}

BEGIN_MESSAGE_MAP(CCipherAssistantDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCipherAssistantDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCipherAssistantDlg 메시지 처리기

BOOL CCipherAssistantDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	ShowWindow(SW_MINIMIZE);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(_T("Cipher_Assistant"));
	ShowWindow(TRUE);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCipherAssistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCipherAssistantDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCipherAssistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCipherAssistantDlg::OnBnClickedButton1()
{
	//초기화
	short min = 0, max = 180;
	int job = 0;
	progress.SetRange(min, max);
	progress.SetPos(job);
	SetDlgItemText(IDC_STATIC7, _T("progress.."));
	Decrypttextbox.SetWindowTextW(_T(""));
	Publictextbox.SetWindowTextW(_T(""));
	AESkeytextbox.SetWindowTextW(_T(""));

	//작업
	CString plain = _T("");
	plaintext.GetWindowTextW(plain);

	job = 30;
	progress.SetPos(job);

	CString MD5entext = MD5(plain);
	MD5textbox.SetWindowTextW(MD5entext);
	
	job = 60;
	progress.SetPos(job);
	
	CString SHA2entext = SHA2(plain);
	SHA256textbox.SetWindowTextW(SHA2entext);
	
	job = 90;
	progress.SetPos(job);

	AEStextbox.SetWindowTextW(_T("It is bug fixing.."));
	/*
	if (plain.GetLength() % 16 == 0 && plain.GetLength() != 0)
	{
		CString AESkey;
		CString AESentext = AES(plain, AESkey); // 에러 수정대기 중
		AEStextbox.SetWindowTextW(AESentext);
		AESkeytextbox.SetWindowTextW(AESkey);
	}
	else if (plain.GetLength() == 0)
		AEStextbox.SetWindowTextW(_T("No text"));
	else
		AEStextbox.SetWindowTextW(_T("Plain text is not a multiple of 16."));
	*/
	job = 120;
	progress.SetPos(job);

	if (plain.GetLength() == 0)
		Base64textbox.SetWindowTextW(_T("No text"));
	else
	{
		CString Base64entext = Base64(plain);
		Base64textbox.SetWindowTextW(Base64entext);
	}

	job = 150;
	progress.SetPos(job);

	if (plain.GetLength() == 0)
		RSAtextbox.SetWindowTextW(_T("No text"));
	else
	{
		CString decryptkey, publickey;
		CString RSAentext = RSA(plain, decryptkey, publickey);
		RSAtextbox.SetWindowTextW(RSAentext);
		Decrypttextbox.SetWindowTextW(decryptkey);
		Publictextbox.SetWindowTextW(publickey);
	}

	job = 180;
	progress.SetPos(job);
	SetDlgItemText(IDC_STATIC7, _T("progress"));
}