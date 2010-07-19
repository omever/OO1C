
#include "stdafx.h"


#ifdef __linux__
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#endif

#include <stdio.h>
#include <wchar.h>
#include "addinnative.h"
#include <string>

using namespace std;

#define TIME_LEN 34

#define BASE_ERRNO     7

static wchar_t *g_PropNames[] = {};
static wchar_t *g_MethodNames[] = {L"GetContext", L"InvokeMethod"};

static wchar_t *g_PropNamesRu[] = {};
static wchar_t *g_MethodNamesRu[] = {L"ПолучитьКонтекст", L"ВызватьМетод"};

static const wchar_t g_kClassNames[] = L"CAddInNativeOO"; //"|OtherClass1|OtherClass2";
static IAddInDefBase *pAsyncEvent = NULL;

uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len = 0);
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len = 0);
uint32_t getLenShortWcharStr(const WCHAR_T* Source);

//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
    if(!*pInterface)
    {
        *pInterface= new CAddInNativeOO;
        return (long)*pInterface;
    }
    return 0;
}
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pIntf)
{
   if(!*pIntf)
      return -1;

   delete *pIntf;
   *pIntf = 0;
   return 0;
}
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
    static WCHAR_T* names = 0;
    if (!names)
        ::convToShortWchar(&names, g_kClassNames);
    return names;
}
//---------------------------------------------------------------------------//
#ifndef __linux__
VOID CALLBACK MyTimerProc(
        HWND hwnd, // handle of window for timer messages
        UINT uMsg, // WM_TIMER message
        UINT idEvent, // timer identifier
        DWORD dwTime // current system time
);
#else
static void MyTimerProc(int sig);
#endif //__linux__

// CAddInNativeOO
//---------------------------------------------------------------------------//
CAddInNativeOO::CAddInNativeOO()
{
    m_iMemory = 0;
    m_iConnect = 0;
}
//---------------------------------------------------------------------------//
CAddInNativeOO::~CAddInNativeOO()
{
}
//---------------------------------------------------------------------------//
bool CAddInNativeOO::Init(void* pConnection)
{ 
    m_iConnect = (IAddInDefBase*)pConnection;
    return m_iConnect != NULL;
}
//---------------------------------------------------------------------------//
long CAddInNativeOO::GetInfo()
{ 
    // Component should put supported component technology version 
    // This component supports 2.0 version
    return 2000; 
}
//---------------------------------------------------------------------------//
void CAddInNativeOO::Done()
{
}
/////////////////////////////////////////////////////////////////////////////
// ILanguageExtenderBase
//---------------------------------------------------------------------------//
bool CAddInNativeOO::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{ 
    wchar_t *wsExtension = L"AddInOO";
    int iActualSize = ::wcslen(wsExtension) + 1;
    WCHAR_T* dest = 0;

    if (m_iMemory)
    {
        if(m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
            ::convToShortWchar(wsExtensionName, wsExtension, iActualSize);
        return true;
    }

    return false; 
}
//---------------------------------------------------------------------------//
long CAddInNativeOO::GetNProps()
{ 
    // You may delete next lines and add your own implementation code here
    return ePropLast;
}
//---------------------------------------------------------------------------//
long CAddInNativeOO::FindProp(const WCHAR_T* wsPropName)
{ 
    long plPropNum = -1;
    wchar_t* propName = 0;

    ::convFromShortWchar(&propName, wsPropName);
    plPropNum = findName(g_PropNames, propName, ePropLast);

    if (plPropNum == -1)
        plPropNum = findName(g_PropNamesRu, propName, ePropLast);

    delete[] propName;

    return plPropNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNativeOO::GetPropName(long lPropNum, long lPropAlias)
{ 
    if (lPropNum >= ePropLast)
        return NULL;

    wchar_t *wsCurrentName = NULL;
    WCHAR_T *wsPropName = NULL;
    int iActualSize = 0;

    switch(lPropAlias)
    {
    case 0: // First language
        wsCurrentName = g_PropNames[lPropNum];
        break;
    case 1: // Second language
        wsCurrentName = g_PropNamesRu[lPropNum];
        break;
    default:
        return 0;
    }
    
    iActualSize = wcslen(wsCurrentName)+1;

    if (m_iMemory && wsCurrentName)
    {
        if (m_iMemory->AllocMemory((void**)&wsPropName, iActualSize * sizeof(WCHAR_T)))
            ::convToShortWchar(&wsPropName, wsCurrentName, iActualSize);
    }

    return wsPropName;
}
//---------------------------------------------------------------------------//
bool CAddInNativeOO::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{ 
    switch(lPropNum)
    {
    default:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------//
bool CAddInNativeOO::SetPropVal(const long lPropNum, tVariant *varPropVal)
{ 
    switch(lPropNum)
    { 
    default:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------//
bool CAddInNativeOO::IsPropReadable(const long lPropNum)
{ 
    switch(lPropNum)
    { 
    default:
        return false;
    }

    return false;
}
//---------------------------------------------------------------------------//
bool CAddInNativeOO::IsPropWritable(const long lPropNum)
{
    switch(lPropNum)
    { 
    default:
        return false;
    }

    return false;
}
//---------------------------------------------------------------------------//
long CAddInNativeOO::GetNMethods()
{ 
    return eMethLast;
}
//---------------------------------------------------------------------------//
long CAddInNativeOO::FindMethod(const WCHAR_T* wsMethodName)
{ 
    long plMethodNum = -1;
    wchar_t* name = 0;

    ::convFromShortWchar(&name, wsMethodName);

    plMethodNum = findName(g_MethodNames, name, eMethLast);

    if (plMethodNum == -1)
        plMethodNum = findName(g_MethodNamesRu, name, eMethLast);

    return plMethodNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNativeOO::GetMethodName(const long lMethodNum, const long lMethodAlias)
{ 
    if (lMethodNum >= eMethLast)
        return NULL;

    wchar_t *wsCurrentName = NULL;
    WCHAR_T *wsMethodName = NULL;
    int iActualSize = 0;

    switch(lMethodAlias)
    {
    case 0: // First language
        wsCurrentName = g_MethodNames[lMethodNum];
        break;
    case 1: // Second language
        wsCurrentName = g_MethodNamesRu[lMethodNum];
        break;
    default: 
        return 0;
    }

    iActualSize = wcslen(wsCurrentName)+1;

    if (m_iMemory && wsCurrentName)
    {
        if(m_iMemory->AllocMemory((void**)&wsMethodName, iActualSize * sizeof(WCHAR_T)))
            ::convToShortWchar(&wsMethodName, wsCurrentName, iActualSize);
    }

    return wsMethodName;
}
//---------------------------------------------------------------------------//
long CAddInNativeOO::GetNParams(const long lMethodNum)
{ 
    switch(lMethodNum)
    { 
    case eMethGetContext:
        return 0;
    case eMethInvoke:
        return 2;
    default:
        return 0;
    }
    
    return 0;
}
//---------------------------------------------------------------------------//
bool CAddInNativeOO::GetParamDefValue(const long lMethodNum, const long lParamNum,
                          tVariant *pvarParamDefValue)
{ 
    TV_VT(pvarParamDefValue)= VTYPE_EMPTY;

    switch(lMethodNum)
    { 
    case eMethGetContext:
    case eMethInvoke:
        // There are no parameter values by default 
        break;
    default:
        return false;
    }

    return false;
} 
//---------------------------------------------------------------------------//
bool CAddInNativeOO::HasRetVal(const long lMethodNum)
{ 
    switch(lMethodNum)
    { 
    case eMethGetContext:
    case eMethInvoke:
        return true;
    default:
        return false;
    }

    return false;
}
//---------------------------------------------------------------------------//
bool CAddInNativeOO::CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray)
{ 
    switch(lMethodNum)
    { 
    default:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------//
bool CAddInNativeOO::CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{ 
    bool ret = false;
    FILE *file = 0;
    char *name = 0;
    int size = 0;
    char *mbstr = 0;
    wchar_t* wsTmp = 0;

    switch(lMethodNum)
    {
    case eMethGetContext:
	try {
	    __context = ::cppu::bootstrap();
	    __service = __context->getServiceManager();
	}
	catch (Exception &e) {
        wchar_t* wsMsgBuf;
        OString o = OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US );
        name = o.pData->buffer;
        size = mbstowcs(0, name, 0) + 1;
        wsMsgBuf = new wchar_t[size];
        memset(wsMsgBuf, 0, size * sizeof(wchar_t));
        size = mbstowcs(wsMsgBuf, name, size);

        addError(ADDIN_E_VERY_IMPORTANT, L"AddInNativeOO", wsMsgBuf, eGetContextException);
        delete[] wsMsgBuf;
        return false;
	}
	
    Any *ctx = new Any(__context);

    wstring res = warp(ctx);
    wchar_t 
	break;
    }
    return ret; 
}
//---------------------------------------------------------------------------//
// This code will work only on the client!
#ifndef __linux__
VOID CALLBACK MyTimerProc(
  HWND hwnd,    // handle of window for timer messages
  UINT uMsg,    // WM_TIMER message
  UINT idEvent, // timer identifier
  DWORD dwTime  // current system time
)
{
    if (!pAsyncEvent)
        return;

    wchar_t *who = L"ComponentNative", *what = L"Timer";

    wchar_t *wstime = new wchar_t[TIME_LEN];
    if (wstime)
    {
        wmemset(wstime, 0, TIME_LEN);
        ::_ultow(dwTime, wstime, 10);
        pAsyncEvent->ExternalEvent(who, what, wstime);
        delete[] wstime;
    }
}
#else
void MyTimerProc(int sig)
{
    if (pAsyncEvent)
        return;

    WCHAR_T *who = 0, *what = 0, *wdata = 0;
    wchar_t *data = 0;
    time_t dwTime = time(NULL);

    data = new wchar_t[TIME_LEN];
    
    if (data)
    {
        wmemset(data, 0, TIME_LEN);
        swprintf(data, TIME_LEN, L"%ul", dwTime);
        ::convToShortWchar(&who, L"ComponentNative");
        ::convToShortWchar(&what, L"Timer");
        ::convToShortWchar(&wdata, data);

        pAsyncEvent->ExternalEvent(who, what, wdata);

        delete[] who;
        delete[] what;
        delete[] wdata;
        delete[] data;
    }
}
#endif
//---------------------------------------------------------------------------//
void CAddInNativeOO::SetLocale(const WCHAR_T* loc)
{
#ifndef __linux__
    _wsetlocale(LC_ALL, loc);
#else
    //We convert in char* char_locale
    //also we establish locale
    //setlocale(LC_ALL, char_locale);
#endif
}
/////////////////////////////////////////////////////////////////////////////
// LocaleBase
//---------------------------------------------------------------------------//
bool CAddInNativeOO::setMemManager(void* mem)
{
    m_iMemory = (IMemoryManager*)mem;
    return m_iMemory != 0;
}
//---------------------------------------------------------------------------//
void CAddInNativeOO::addError(uint32_t wcode, const wchar_t* source, 
                        const wchar_t* descriptor, long code)
{
    if (m_iConnect)
    {
        WCHAR_T *err = 0;
        WCHAR_T *descr = 0;
        
        ::convToShortWchar(&err, source);
        ::convToShortWchar(&descr, descriptor);

        m_iConnect->AddError(wcode, err, descr, code);
        delete[] err;
        delete[] descr;
    }
}
//---------------------------------------------------------------------------//
long CAddInNativeOO::findName(wchar_t* names[], const wchar_t* name, 
                         const uint32_t size) const
{
    long ret = -1;
    for (uint32_t i = 0; i < size; i++)
    {
        if (!wcscmp(names[i], name))
        {
            ret = i;
            break;
        }
    }
    return ret;
}
//---------------------------------------------------------------------------//
uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len)
{
    if (!len)
        len = ::wcslen(Source)+1;

    if (!*Dest)
        *Dest = new WCHAR_T[len];

    WCHAR_T* tmpShort = *Dest;
    wchar_t* tmpWChar = (wchar_t*) Source;
    uint32_t res = 0;

    ::memset(*Dest, 0, len*sizeof(WCHAR_T));
    do
    {
        *tmpShort++ = (WCHAR_T)*tmpWChar++;
        ++res;
    }
    while (len-- && *tmpWChar);

    return res;
}
//---------------------------------------------------------------------------//
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len)
{
    if (!len)
        len = getLenShortWcharStr(Source)+1;

    if (!*Dest)
        *Dest = new wchar_t[len];

    wchar_t* tmpWChar = *Dest;
    WCHAR_T* tmpShort = (WCHAR_T*)Source;
    uint32_t res = 0;

    ::memset(*Dest, 0, len*sizeof(wchar_t));
    do
    {
        *tmpWChar++ = (wchar_t)*tmpShort++;
        ++res;
    }
    while (len-- && *tmpShort);

    return res;
}
//---------------------------------------------------------------------------//
uint32_t getLenShortWcharStr(const WCHAR_T* Source)
{
    uint32_t res = 0;
    WCHAR_T *tmpShort = (WCHAR_T*)Source;

    while (*tmpShort++)
        ++res;

    return res;
}
//---------------------------------------------------------------------------//
