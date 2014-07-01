//////////////////////////////////////////////////////////////////////////
//  Class Name: CDuiFrame
// Description: A DuiWindow Frame.
//     Creator: Huang Jianxiong
//     Version: 2011.9.1 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////
#include "duistd.h"
#include "duiframe.h"


namespace SOUI
{

#define WM_NCMOUSEFIRST WM_NCMOUSEMOVE
#define WM_NCMOUSELAST  WM_NCMBUTTONDBLCLK


SwndContainerImpl::SwndContainerImpl(SWindow *pHost)
    :m_pHost(pHost)
    ,m_hCapture(NULL)
    ,m_hHover(NULL)
    ,m_bNcHover(FALSE)
    ,m_dropTarget(pHost)
    ,m_focusMgr(pHost)
{
}

LRESULT SwndContainerImpl::DoFrameEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    LRESULT lRet=0;
    m_pHost->AddRef();
    m_pHost->SetMsgHandled(TRUE);

    switch(uMsg)
    {
    case WM_MOUSEMOVE:
        OnFrameMouseMove((UINT)wParam,CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)));
        break;
    case WM_MOUSEHOVER:
        OnFrameMouseEvent(uMsg,wParam,lParam);
        break;
    case WM_MOUSELEAVE:
        OnFrameMouseLeave();
        break;
    case WM_SETCURSOR:
        lRet=OnFrameSetCursor(CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)));
        if(!lRet)
        {
            SetCursor(LoadCursor(NULL,IDC_ARROW));
        }
        break;
    case WM_KEYDOWN:
        OnFrameKeyDown((UINT)wParam,(UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16));
        break;
    case WM_SETFOCUS:
        OnActivate(WA_ACTIVE);
        break;
    case WM_KILLFOCUS:
        OnActivate(WA_INACTIVE);
        break;
    case WM_ACTIVATE:
        OnActivate(LOWORD(wParam));
        break;
    case WM_IME_STARTCOMPOSITION:
    case WM_IME_ENDCOMPOSITION:
    case WM_IME_COMPOSITION:
    case WM_IME_CHAR:
        OnFrameKeyEvent(uMsg,wParam,lParam);
        break;
    default:
        if(uMsg>=WM_KEYFIRST && uMsg<=WM_KEYLAST)
            OnFrameKeyEvent(uMsg,wParam,lParam);
        else if(uMsg>=WM_MOUSEFIRST && uMsg <= WM_MOUSELAST)
            OnFrameMouseEvent(uMsg,wParam,lParam);
        else
            m_pHost->SetMsgHandled(FALSE);
        break;
    }

    m_pHost->Release();
    return lRet;
}


BOOL SwndContainerImpl::OnReleaseSwndCapture()
{
    m_hCapture=NULL;
    return TRUE;
}

SWND SwndContainerImpl::OnSetSwndCapture(SWND hDuiWnd)
{
    SWindow *pWnd=DuiWindowMgr::GetWindow(hDuiWnd);
    ASSERT(pWnd);
    if(pWnd->IsDisabled(TRUE)) return 0;

    SWND hRet=m_hCapture;
    m_hCapture=hDuiWnd;
    return hRet;
}

void SwndContainerImpl::OnSetSwndFocus(SWND hDuiWnd)
{
    m_focusMgr.SetFocusedHwnd(hDuiWnd);
}

SWND SwndContainerImpl::OnGetSwndCapture()
{
    return m_hCapture;
}

SWND SwndContainerImpl::SwndGetFocus()
{
    return m_focusMgr.GetFocusedHwnd();
}

SWND SwndContainerImpl::SwndGetHover()
{
    return m_hHover;
}

void SwndContainerImpl::OnFrameMouseMove(UINT uFlag,CPoint pt)
{
    SWindow *pCapture=DuiWindowMgr::GetWindow(m_hCapture);
    if(pCapture)
    {
        CRect rc;
        pCapture->GetRect(&rc);
        SWindow * pHover=rc.PtInRect(pt)?pCapture:NULL;
        SWND hHover=pHover?pHover->GetSwnd():NULL;
        if(hHover!=m_hHover)
        {
            SWindow *pOldHover=DuiWindowMgr::GetWindow(m_hHover);
            m_hHover=hHover;
            if(pOldHover) pOldHover->DuiSendMessage(m_bNcHover?WM_NCMOUSELEAVE:WM_MOUSELEAVE);
            if(pHover)    pHover->DuiSendMessage(m_bNcHover?WM_NCMOUSEHOVER:WM_MOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
        }
        pCapture->DuiSendMessage(m_bNcHover?WM_NCMOUSEMOVE:WM_MOUSEMOVE,uFlag,MAKELPARAM(pt.x,pt.y));
    }
    else
    {
        SWND hHover=m_pHost->HswndFromPoint(pt,FALSE);
        SWindow * pHover=DuiWindowMgr::GetWindow(hHover);
        if(m_hHover!=hHover)
        {
            SWindow *pOldHover=DuiWindowMgr::GetWindow(m_hHover);
            m_hHover=hHover;
            if(pOldHover)
            {
                if(m_bNcHover) pOldHover->DuiSendMessage(WM_NCMOUSELEAVE);
                pOldHover->DuiSendMessage(WM_MOUSELEAVE);
            }
            if(pHover && !pHover->IsDisabled(TRUE))
            {
                m_bNcHover=pHover->OnNcHitTest(pt);
                if(m_bNcHover) pHover->DuiSendMessage(WM_NCMOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
                pHover->DuiSendMessage(WM_MOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
            }
        }
        else if(pHover && !pHover->IsDisabled(TRUE))
        {
            BOOL bNcHover=pHover->OnNcHitTest(pt);
            if(bNcHover!=m_bNcHover)
            {
                m_bNcHover=bNcHover;
                if(m_bNcHover)
                {
                    pHover->DuiSendMessage(WM_MOUSELEAVE);
                    pHover->DuiSendMessage(WM_NCMOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
                }
                else
                {
                    pHover->DuiSendMessage(WM_NCMOUSELEAVE);
                    pHover->DuiSendMessage(WM_MOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
                }
            }
        }
        if(pHover && !pHover->IsDisabled(TRUE))
            pHover->DuiSendMessage(m_bNcHover?WM_NCMOUSEMOVE:WM_MOUSEMOVE,uFlag,MAKELPARAM(pt.x,pt.y));
    }
}

void SwndContainerImpl::OnFrameMouseLeave()
{
    SWindow *pCapture=DuiWindowMgr::GetWindow(m_hCapture);
    if(pCapture)
    {
        pCapture->DuiSendMessage(WM_MOUSELEAVE);
    }
    else if(m_hHover)
    {
        SWindow *pHover=DuiWindowMgr::GetWindow(m_hHover);
        if(pHover && !pHover->IsDisabled(TRUE))
            pHover->DuiSendMessage(m_bNcHover?WM_NCMOUSELEAVE:WM_MOUSELEAVE);
    }
    m_hHover=NULL;
}


BOOL SwndContainerImpl::OnFrameSetCursor(const CPoint &pt)
{
    SWindow *pCapture=DuiWindowMgr::GetWindow(m_hCapture);
    if(pCapture) return pCapture->OnSetCursor(pt);
    else
    {
        SWindow *pHover=DuiWindowMgr::GetWindow(m_hHover);
        if(pHover && !pHover->IsDisabled(TRUE)) return pHover->OnSetCursor(pt);
    }
    return FALSE;
}

void SwndContainerImpl::OnFrameMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    SWindow *pCapture=DuiWindowMgr::GetWindow(m_hCapture);
    if(pCapture)
    {
        if(m_bNcHover && uMsg!=WM_MOUSEWHEEL) uMsg += WM_NCMOUSEFIRST - WM_MOUSEFIRST;//转换成NC对应的消息
        BOOL bMsgHandled = FALSE;
        pCapture->DuiSendMessage(uMsg,wParam,lParam,&bMsgHandled);
        m_pHost->SetMsgHandled(bMsgHandled);
    }
    else
    {
        m_hHover=m_pHost->HswndFromPoint(CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)),FALSE);
        SWindow *pHover=DuiWindowMgr::GetWindow(m_hHover);
        if(pHover  && !pHover->IsDisabled(TRUE))
        {
            BOOL bMsgHandled = FALSE;
            if(m_bNcHover && uMsg!=WM_MOUSEWHEEL) uMsg += WM_NCMOUSEFIRST - WM_MOUSEFIRST;//转换成NC对应的消息
            pHover->DuiSendMessage(uMsg,wParam,lParam,&bMsgHandled);
            m_pHost->SetMsgHandled(bMsgHandled);
        }else
        {
            m_pHost->SetMsgHandled(FALSE);
        }
    }
}

void SwndContainerImpl::OnFrameKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    SWindow *pFocus=DuiWindowMgr::GetWindow(m_focusMgr.GetFocusedHwnd());
    if(pFocus)
    {
        BOOL bMsgHandled = FALSE;
        pFocus->DuiSendMessage(uMsg,wParam,lParam,&bMsgHandled);
        m_pHost->SetMsgHandled(bMsgHandled);
    }else
    {
        m_pHost->SetMsgHandled(FALSE);
    }
}

void SwndContainerImpl::OnFrameKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if(m_focusMgr.OnKeyDown(nChar)) return; //首先处理焦点切换

    SWindow *pFocus=DuiWindowMgr::GetWindow(m_focusMgr.GetFocusedHwnd());
    if(pFocus)
    {
        BOOL bMsgHandled=FALSE;
        pFocus->DuiSendMessage(WM_KEYDOWN,nChar,MAKELPARAM(nRepCnt,nFlags),&bMsgHandled);
        m_pHost->SetMsgHandled(bMsgHandled);
    }else
    {
        m_pHost->SetMsgHandled(FALSE);
    }
}

BOOL SwndContainerImpl::RegisterDragDrop( SWND hDuiWnd,IDropTarget *pDropTarget )
{
    return m_dropTarget.RegisterDragDrop(hDuiWnd,pDropTarget);
}

BOOL SwndContainerImpl::RevokeDragDrop( SWND hDuiWnd )
{
    return m_dropTarget.RevokeDragDrop(hDuiWnd);
}

void SwndContainerImpl::OnActivate( UINT nState )
{
    if(nState==WA_INACTIVE)
    {
        m_focusMgr.StoreFocusedView();
    }else if(nState==WA_ACTIVE)
    {
        m_focusMgr.RestoreFocusedView();
    }
}

BOOL SwndContainerImpl::RegisterTimelineHandler( ITimelineHandler *pHandler )
{
    POSITION pos=m_lstTimelineHandler.Find(pHandler);
    if(pos) return FALSE;
    m_lstTimelineHandler.AddTail(pHandler);
    IObjRef *pRef=dynamic_cast<IObjRef*>(pHandler);
    if(pRef) pRef->AddRef();
    return TRUE;
}

BOOL SwndContainerImpl::UnregisterTimelineHandler( ITimelineHandler *pHandler )
{
    POSITION pos=m_lstTimelineHandler.Find(pHandler);
    if(!pos) return FALSE;
    m_lstTimelineHandler.RemoveAt(pos);
    IObjRef *pRef=dynamic_cast<IObjRef*>(pHandler);
    if(pRef) pRef->Release();
    return TRUE;
}

void SwndContainerImpl::OnNextFrame()
{
    POSITION pos=m_lstTimelineHandler.GetHeadPosition();
    while(pos)
    {
        ITimelineHandler * pHandler=m_lstTimelineHandler.GetNext(pos);
        pHandler->OnNextFrame();
    }
}
}//namespace SOUI
