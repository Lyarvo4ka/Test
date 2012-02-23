#include "stdafx.h"
#include "NANDReader.h"
#include "NANDReaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()



CNANDReaderDlg::CNANDReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNANDReaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNANDReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNANDReaderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CNANDReaderDlg message handlers

BOOL CNANDReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	BOOL ret = FALSE;
	if (reader.Open(_T("\\\\.\\PhysicalDrive0")))
	{
		DWORD rw = 0;
		BYTE buff[1024];
		memset(buff, 0xFF, sizeof(buff));

		ret = reader.PowerOff(buff, &rw);

		//
		// Set pawer param
		//
		// #define MAKE_POWER_PARAM(i, c, o, w)	(((i) & 0x0F) | (((c) & 0x0F) << 4) | (((o) ? 1 : 0) << 8) | (((w) ? 1 : 0) << 9))
		// m_pReader->PowerOn(MAKE_POWER_PARAM(flash.m_nPowerIO, flash.m_nPowerCore, flash.m_nONFI, flash.m_nBusWidth), bufDummy, &nReadedBytes);
		ret = reader.PowerOn(MAKE_POWER_PARAM(PowerIO_3_3, ModeFirstRead, Support_No, Bus_08), buff, &rw);

		ret = reader.GetFlashID(buff, &rw);

		// nCommandParam = MAKE_COMMAND_PARAM(flash.m_nNumFullAdr, flash.m_nNumPageAdr, flash.m_nNumCmd);
		// nSpeedParam = MAKE_SPEED_PARAM(flash.m_nSpeedAsyn, flash.m_nSpeedSyn, flash.m_nCmdType, flash.m_nReadType);
		// m_nPowerParam = MAKE_POWER_PARAM(flash.m_nPowerIO, flash.m_nPowerCore, flash.m_nONFI, flash.m_nBusWidth);
		// m_pReader->SetFlashParameters(flash.m_nPagePerBlock, m_nPageSize, m_nCountPage, nCommandParam, nSpeedParam, bufDummy, &nReadedBytes);

		ret = reader.PowerOff(buff, &rw);

		WORD nCommandParam = MAKE_COMMAND_PARAM(5, 3, 2);
		WORD nSpeedParam = MAKE_SPEED_PARAM(SpeedAsyn_25, SpeedAsyn_25, ReadPage, ReadOnce);
		ret = reader.SetFlashParameters(128, 4224, 4096, nCommandParam, nSpeedParam, buff, &rw);
		ret = reader.SetReadParameters(MAKE_READ_PARAM(1, ModeFirstRead),  buff, &rw);

		ret = reader.PowerOn(MAKE_POWER_PARAM(PowerIO_3_3, ModeFirstRead, Support_No, Bus_08), buff, &rw);

		BYTE *page = new BYTE[32*4224];
		memset(page, 0xBB, sizeof(32*4224));

		ret = reader.ReadPage(900, 4, page, &rw);


		ret = reader.PowerOff(buff, &rw);
		int x = 0;
	}
	else
	{
		DWORD err = ::GetLastError();
		int x = 0;
	}


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNANDReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNANDReaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNANDReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

