#include "stdafx.h"
#include "FlashReader.h"
#include "FlashReaderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	TIMER_SWOW_PAGE_COUNT	500

IMPLEMENT_DYNCREATE(CFlashReaderView, CFormView)

BEGIN_MESSAGE_MAP(CFlashReaderView, CFormView)
	ON_WM_SIZE()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_ONCE, IDC_RADIO_REPAIR, &CFlashReaderView::OnChangeMode)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

CFlashReaderView::CFlashReaderView()
: CFormView(CFlashReaderView::IDD)
{
	m_nTimerID = 0;
	m_bFirstUpdate = TRUE;
}

CFlashReaderView::~CFlashReaderView()
{
}

#ifdef _DEBUG
void CFlashReaderView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFlashReaderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFlashReaderDoc* CFlashReaderView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlashReaderDoc)));
	return (CFlashReaderDoc*)m_pDocument;
}
#endif //_DEBUG

void CFlashReaderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_STEP, m_progressStep);
}

BOOL CFlashReaderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying the CREATESTRUCT cs
	return CFormView::PreCreateWindow(cs);
}

BOOL CFlashReaderView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	if (!CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext)) return FALSE;
	CImageList	ilPageState;
	CBitmap		bmp;
	CRect		rectTemp;
	CRect		rectWnd;

	GetWindowRect(rectTemp);
	GetDlgItem(IDC_CURRENT_FLASH)->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	m_dlgFlashInfo.Create(this, rectTemp.TopLeft());
	m_dlgFlashInfo.ShowFlashInfo(NULL);
	
	m_ilState.Create(16, 16, ILC_COLORDDB|ILC_MASK, 0, 1);
	bmp.LoadBitmap(IDB_STATES);
	m_ilState.Add(&bmp, RGB(255, 0, 255));
	bmp.DeleteObject();
	rectWnd.SetRectEmpty();
	::ImageList_GetIconSize(m_ilState, (int*)&rectWnd.right, (int*)&rectWnd.bottom);
	for (BYTE nMode = 0; nMode <= ModeCount; nMode++)
	{
		GetDlgItem(IDC_LABLE_CHECK + nMode)->GetWindowRect(rectTemp);
		ScreenToClient(rectTemp);
		rectWnd.MoveToXY(rectTemp.left - rectWnd.Width() - 7, rectTemp.top - 1);
		m_pModeState[nMode] = new CStatic;
		m_pModeState[nMode]->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_ICON, rectWnd, this);
		for (BYTE nCS = 0; nCS < MAX_CS; nCS++)
		{
			GetDlgItem(IDC_LABLE_CS0 + nCS)->GetWindowRect(rectTemp);
			ScreenToClient(rectTemp);
			rectWnd.MoveToX(rectTemp.left + (rectTemp.Width() - rectWnd.Width()) / 2);
			m_pSubTaskState[nMode][nCS] = new CStatic;
			m_pSubTaskState[nMode][nCS]->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_ICON, rectWnd, this);
		}
	}

	ilPageState.Create(8, 8, ILC_COLORDDB|ILC_MASK, 0, 1);
	bmp.LoadBitmap(IDB_PAGE_STATE);
	ilPageState.Add(&bmp, RGB(255, 0, 255));
	bmp.DeleteObject();
	rectWnd.SetRectEmpty();
	::ImageList_GetIconSize(ilPageState, (int*)&rectWnd.right, (int*)&rectWnd.bottom);
	for (BYTE nPageState = PageStateNo; nPageState < PageStateCount; nPageState++)
	{
		GetDlgItem(IDC_STATE_NO + nPageState)->GetWindowRect(rectTemp);
		ScreenToClient(rectTemp);
		rectWnd.MoveToXY(rectTemp.right + 7, rectTemp.top + 2);
		m_pPageState[nPageState] = new CStatic;
		m_pPageState[nPageState]->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_ICON, rectWnd, this);
		m_pPageState[nPageState]->SetIcon(ilPageState.ExtractIcon(nPageState));
	}
	return TRUE;
}

void CFlashReaderView::OnDestroy()
{
	for (BYTE nPageState = PageStateNo; nPageState < PageStateCount; nPageState++)
	{
		delete m_pPageState[nPageState];
	}
	for (BYTE nMode = PageStateNo; nMode <= ModeCount; nMode++)
	{
		delete m_pModeState[nMode];
		for (BYTE nCS = 0; nCS < MAX_CS; nCS++)
		{
			delete m_pSubTaskState[nMode][nCS];
		}
	}
	CFormView::OnDestroy();
}

void CFlashReaderView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (!cx || !cy) return;
	CWnd* pFrameLog = GetDlgItem(IDC_FRAME_LOG);
	CWnd* pReLog = GetDlgItem(IDC_RE_LOG);
	if (!pFrameLog) return;
	if (!pReLog) return;
	CRect	rectClient;
	CRect	rectTemp;
	GetClientRect(rectClient);
	pFrameLog->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	rectTemp.bottom = rectClient.bottom - 9;
	pFrameLog->MoveWindow(rectTemp);
	pReLog->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	rectTemp.bottom = rectClient.bottom - 14;
	pReLog->MoveWindow(rectTemp);
}

void CFlashReaderView::StringToLog(CString strLog)
{
	CRichEditCtrl*	reLog = (CRichEditCtrl*)GetDlgItem(IDC_RE_LOG);
	long			nRELen = reLog->GetTextLength();
	long			nREMaxLen = reLog->GetLimitText();
	long			nStrLen = strLog.GetLength();

	if (!nREMaxLen)
	{
		nREMaxLen = 0x7FFF;
		reLog->LimitText(nREMaxLen);
	}
	while ((nRELen + nStrLen) >= nREMaxLen)
	{	//wrap buffer
		reLog->SetSel(0, reLog->LineLength(0)+1);
		reLog->ReplaceSel(_T(""));
		nRELen = reLog->GetTextLength();
	}
	reLog->SetSel(nRELen, nRELen);
	reLog->ReplaceSel(strLog);
}

void CFlashReaderView::ClearLog()
{
	CRichEditCtrl*	reLog = (CRichEditCtrl*)GetDlgItem(IDC_RE_LOG);
	reLog->SetSel(0, reLog->GetTextLength());
	reLog->ReplaceSel(_T(""));
}

void CFlashReaderView::OnChangeMode(UINT nID)
{
	GetDocument()->SetNewTaskMode((BYTE)(nID - IDC_RADIO_ONCE));
	ShowStateTask(TRUE);
}

void CFlashReaderView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	if (m_bFirstUpdate)
	{
		CFrameWnd* pParent = GetParentFrame();
		pParent->RecalcLayout();
		ResizeParentToFit(FALSE);
		m_bFirstUpdate = FALSE;
	}
	GetDlgItem(IDC_RE_LOG)->SetFocus();
}

void CFlashReaderView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	WORD	nType = LOWORD(lHint);
	if (nType == UpdateInitial)
	{
		InitialUpdate();
	}
	else if (nType == UpdateCheck)
	{
		ShowStateCheck(TRUE);
	}
	else if (nType == UpdateStepStart)
	{
		m_nStepStartPage = GetDocument()->m_nCurrentCS * m_nPagePerCS;
		m_progressStep.SetPos(0);
		EnableModeButtons(FALSE);
		ShowStateTask(TRUE);
		StartTimer();
	}
	else if (nType == UpdateStepProgress)
	{
		m_progressStep.SetPos((int)(*(ULONG*)pHint + HIWORD(lHint) - m_nStepStartPage));
	}
	else if (nType == UpdateStepEnd)
	{
		StopTimer();
		m_progressStep.SetPos(0);
		EnableModeButtons(TRUE);
		ShowStateTask(TRUE);
		ShowPageStateCount();
	}
	else if (nType == UpdateLog)
	{
		StringToLog(*(CString*)pHint);
	}
	else if (nType == UpdateLogClear)
	{
		ClearLog();
	}
}

void CFlashReaderView::InitialUpdate()
{
	CFlashReaderDoc*	pDoc = GetDocument();
	CFlashRecord*		pRecord = &pDoc->m_Flash;
	BOOL				bRecordEnable = pRecord->IsEnable();

	m_dlgFlashInfo.ShowFlashInfo(bRecordEnable ? pRecord : NULL);
	CheckRadioButton(IDC_RADIO_ONCE, IDC_RADIO_REPAIR, IDC_RADIO_ONCE + pDoc->m_nTaskMode);
	EnableModeButtons(bRecordEnable);
	m_nPagePerCS = bRecordEnable ? pRecord->GetPagePerCS() : 0;
	m_progressStep.SetRange32(0, (int)m_nPagePerCS);
	ShowStateCheck(bRecordEnable);
	ShowStateTask(bRecordEnable);
	ShowPageStateCount();
}

void CFlashReaderView::SetModeState(INT nMode, INT nState)
{
	m_pModeState[nMode]->SetIcon(m_ilState.ExtractIcon(nState));
}

void CFlashReaderView::SetSubTaskState(INT nMode, INT nCS, INT nState)
{
	m_pSubTaskState[nMode][nCS]->SetIcon(m_ilState.ExtractIcon(nState));
}

void CFlashReaderView::ShowStateCheck(BOOL bRecordEnable)
{
	ULONGLONG	nSignature = GetDocument()->m_nSignature;
	ULONGLONG	nMask = GetDocument()->m_nMaskID;
	BYTE		nMaskCS = GetDocument()->m_nMaskCS;
	BYTE		nCountCS = GetDocument()->m_Flash.m_nCountCS;
	CString		strID;
	for (BYTE nCS = 0; nCS < MAX_CS; nCS++)
	{
		SetSubTaskState(0, nCS, (bRecordEnable && (nCS < nCountCS)) ? (nMaskCS & BIT(nCS) ? StateComplete : StateFail) : StateDisable);
	}
	SetModeState(0, bRecordEnable ? (nMaskCS ? StateComplete : StateFail) : StateDisable);
	if (nSignature) strID.Format(_T("%s, ID %s"), GetDocument()->m_Flash.GetStringVendor(nSignature), CFlashRecord::GetStringSignature(nSignature, nMask));
	SetDlgItemText(IDC_ID_AND_VENDOR, strID);
}

void CFlashReaderView::ShowStateTask(BOOL bRecordEnable)
{
	BYTE*	pSubTaskState = GetDocument()->m_nSubTaskState;
	BYTE	nFlashCS = GetDocument()->m_Flash.m_nCountCS;
	BYTE	nSubTaskState;
	BYTE	nItemState;
	BYTE	nCompleteCount;
	BYTE	nWaitCount;

	for (BYTE nMode = ModeFirstRead; nMode < ModeCount; nMode++)
	{
		nSubTaskState = StateDisable;
		nCompleteCount = 0;
		nWaitCount = 0;
		for (BYTE nCS = 0; nCS < MAX_CS; nCS++)
		{
			nItemState = pSubTaskState[nMode * MAX_CS + nCS];
			SetSubTaskState(nMode+1, nCS, nItemState);
			if (nItemState == StateWait) nWaitCount++;
			else if (nItemState == StateWork) nSubTaskState = StateWork;
			else if (nItemState == StateComplete) nCompleteCount++;
		}
		if (nCompleteCount == nFlashCS) nSubTaskState = StateComplete;
		else if (nWaitCount && (nSubTaskState != StateWork)) nSubTaskState = StateWait;
		SetModeState(nMode+1, nSubTaskState);
	}
}

void CFlashReaderView::ShowPageStateCount()
{
	ULONGLONG	arrPageStateCount[PageStateCount];

	GetDocument()->m_FlashMap.GetArrStateCount(arrPageStateCount);
	for (BYTE nPageState = PageStateNo; nPageState < PageStateCount; nPageState++)
	{
		CWnd* pWnd = GetDlgItem(IDC_STATE_NO + nPageState);
		SetDlgItemInt(IDC_STATE_NO + nPageState, (UINT)arrPageStateCount[nPageState], FALSE);
	}
}

void CFlashReaderView::EnableModeButtons(BOOL bEnable)
{
	for (BYTE nMode = ModeFirstRead; nMode < ModeCount; nMode++)
	{
		GetDlgItem(IDC_RADIO_ONCE + nMode)->EnableWindow(bEnable);
	}
}

void CFlashReaderView::StartTimer()
{
	while (!m_nTimerID) m_nTimerID = SetTimer(++m_nTimerID, TIMER_SWOW_PAGE_COUNT, 0);
}

void CFlashReaderView::StopTimer()
{
	if (m_nTimerID)
	{
		KillTimer(m_nTimerID);
		m_nTimerID = 0;
	}
}

void CFlashReaderView::OnTimer(UINT_PTR nIDEvent)
{
	if (m_nTimerID == nIDEvent)
	{
		ShowPageStateCount();
	}
	CFormView::OnTimer(nIDEvent);
}

void CFlashReaderView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (!GetDocument()->IsRunning() && GetDocument()->m_Flash.IsEnable())
	{
		for (BYTE nPageState = PageStateOnce; nPageState < PageStateCount; nPageState++)
		{
			if (GetDlgItem(IDC_STATE_NO + nPageState) == pWnd)
			{
				CMenu		menuChangeState;
				CMenu		menuPopup;
				CMenu*		pSubMenu;
				CString		strMenuTitle;
				CString		strItemText;
				UINT_PTR	nRet;
				ULONGLONG	arrPageStateCount[PageStateCount];

				GetDocument()->m_FlashMap.GetArrStateCount(arrPageStateCount);
				if (arrPageStateCount[nPageState])
				{
					menuPopup.LoadMenu(IDR_MENU_NEW_STATE);
					pSubMenu = menuPopup.GetSubMenu(0);
					pSubMenu->GetMenuString(nPageState, strItemText, MF_BYPOSITION);
					strMenuTitle.Format(_T("Change all pages with state %s to"), strItemText);
					for (UINT nPos = pSubMenu->GetMenuItemCount() - 1; nPos >= nPageState; nPos--)
					{
						pSubMenu->RemoveMenu(nPos, MF_BYPOSITION);
					}
					menuChangeState.CreatePopupMenu();
					menuChangeState.AppendMenu(MF_STRING|MF_POPUP, (UINT_PTR)pSubMenu->m_hMenu, strMenuTitle);
					nRet = menuChangeState.TrackPopupMenu(TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_RETURNCMD|TPM_NONOTIFY, point.x, point.y, this, CRect(0,0,100,100));
					if (nRet)
					{
						BYTE nStateFrom = nPageState;
						BYTE nStateTo = (BYTE)(nRet - ID_NEW_NOTREADED);
						GetDocument()->ChangePageState(nPageState, (BYTE)(nRet - ID_NEW_NOTREADED));
					}
				}
				return;
			}
		}
	}
}
