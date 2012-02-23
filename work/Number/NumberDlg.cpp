#include "stdafx.h"
#include "Number.h"
#include "NumberDlg.h"
#include "DlgProxy.h"
#include "odbcinst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CNumberDlg, CDialog);

CNumberDlg::CNumberDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumberDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CNumberDlg::~CNumberDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNumberDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN_SRC_BTN, &CNumberDlg::OnBnClickedOpenSrcBtn)
	ON_BN_CLICKED(IDC_START_BUTTON, &CNumberDlg::OnBnClickedStartButton)
END_MESSAGE_MAP()


// CNumberDlg message handlers

BOOL CNumberDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CEdit *src_edit = (CEdit *)GetDlgItem(IDC_SRC_EDIT);
	if (src_edit)
		src_edit->SetWindowText(_T("D:\\QT_Projects\\Prokopenko.xls"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNumberDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CNumberDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CNumberDlg::OnClose()
{
	if (CanExit())
		CDialog::OnClose();
}

void CNumberDlg::OnOK()
{
	if (CanExit())
		CDialog::OnOK();
}

void CNumberDlg::OnCancel()
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CNumberDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


void CNumberDlg::OnBnClickedOpenSrcBtn()
{
	CButton *btn = (CButton *)GetDlgItem(IDC_OPEN_SRC_BTN);
	CEdit *src_edit = (CEdit *)GetDlgItem(IDC_SRC_EDIT);
	if (!btn || !src_edit) return;

	CFileDialog file_dlg(TRUE);
	file_dlg.m_pOFN->lpstrFilter = _T("Excel files\0*.xls\0All Files\0*.*\0\0");

	if (file_dlg.DoModal() == IDOK)
		src_edit->SetWindowText(file_dlg.m_pOFN->lpstrFile);
}

// Получаем имя Excel-ODBC драйвера 
CString GetExcelDriver()
{
	char *buff = NULL;
	char *driver_descr;
	WORD bytes_ret = 0;
	WORD buff_size = 1000;
	CString driver_name = _T("");

	for (;;)
	{
		buff = new TCHAR[buff_size];
		if (::SQLGetInstalledDrivers(buff, buff_size, &bytes_ret))
		{
			if (bytes_ret >= buff_size)
			{
				buff_size *= 2*buff_size;
				delete buff;
			}
			else
				break;
		}
		else
			return driver_name;
	}

	size_t len ;
	driver_descr = buff;
	while (len = strlen(driver_descr))
	{
		CString str = driver_descr;
		if (str.Find("Excel") != -1)
		{
			driver_name = str;
			break;
		}
		driver_descr += (len + 1);
	}

	delete buff;
	return driver_name;
}

void CNumberDlg::OnBnClickedStartButton()
{
	CEdit *src_edit = (CEdit *)GetDlgItem(IDC_SRC_EDIT);
	if (!src_edit) return;

	CString file_name = _T("");
	src_edit->GetWindowText(file_name);
	if (file_name == _T(""))
	{
		MessageBox(_T("Source file is not selected"), NULL, MB_OK|MB_ICONHAND);
		return;
	}

	CString odbc_driverName = GetExcelDriver();
	if (odbc_driverName == _T(""))
	{
		MessageBox(_T("Excel driver not installed"), NULL, MB_OK|MB_ICONHAND);
		return;
	}

	CString DSN_string;
	DSN_string.Format(_T("DBQ=%s;Driver=%s;DriverId=790"), file_name, odbc_driverName);

	CDatabase db;
	try 
	{
		if (db.OpenEx(DSN_string))
		//if (db.Open(NULL))
		{
			CRecordset rec_set(&db);
			if (rec_set.Open(CRecordset::dynaset, _T("SELECT * FROM [Лист1$]"), CRecordset::none))
			{
				short nFields = rec_set.GetODBCFieldCount();
				for(short i = 0; i < nFields; ++i)
				{
					CODBCFieldInfo field_info;
					CDBVariant var;
					rec_set.GetODBCFieldInfo(i, field_info);
					//do {
					//	
					//} while ()

					
					int x = 0;
				} 

				rec_set.Close();
			}			
			db.Close();
		}
	}
	catch(CDBException e)
	{
		AfxMessageBox(e.m_strStateNativeOrigin);
	}
	catch (...)
	{
		AfxMessageBox(_T("DSN error"));
	}

	MessageBeep(MB_OK);
}
