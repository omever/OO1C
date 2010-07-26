
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

static const wchar_t *g_PropNames[] = {};
static const wchar_t *g_MethodNames[] = {L"GetContext",
		L"InvokeMethod",
		L"CreateStruct",
		L"CreateSequence",
		L"SetSequenceValue",
		L"GetSequenceValue",
		L"SetProperty",
		L"GetProperty",
		L"GetValueType"
	};

static const wchar_t *g_PropNamesRu[] = {};
static const wchar_t *g_MethodNamesRu[] = {L"ПолучитьКонтекст",
		L"ВызватьМетод",
		L"СоздатьСтруктуру",
		L"СоздатьПоследовательность",
		L"УстановитьЗначениеЭлементаПоследовательности",
		L"ПолучитьЗначениеЭлементаПоследовательности",
		L"УстановитьЗначениеСвойства",
		L"ПолучитьЗначениеСвойства",
		L"ПолучитьТипРезультата"
	};

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
    const wchar_t *wsExtension = L"AddInOO";
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

    const wchar_t *wsCurrentName = NULL;
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

    const wchar_t *wsCurrentName = NULL;
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
	case eMethCreateStruct:
	case eMethCreateSequence:
	case eMethSetSequenceValue:
	case eMethGetSequenceValue:
	case eMethSetProperty:
	case eMethGetProperty:
	case eMethGetValueType:
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
	case eMethCreateStruct:
	case eMethCreateSequence:
	case eMethSetSequenceValue:
	case eMethGetSequenceValue:
	case eMethSetProperty:
	case eMethGetProperty:
	case eMethGetValueType:
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
    wstring result;
    switch(lMethodNum)
    {
    	case eMethGetContext:
    		ret = this->getContext(result);
    		wchar2variant(pvarRetValue, result);
    		break;
    	case eMethInvoke:
    		if(lSizeArray != 3) {
    			addError(ADDIN_E_VERY_IMPORTANT, L"OOConverter", L"Неверное количество параметров", eGetContextException);
    			return false;
    		}
    		wstring obj;
    		wstring meth;
    		wstring args;
    		wstring retval;
    		if(!variant2wchar(obj, paParams[0])) {
    			addError(ADDIN_E_VERY_IMPORTANT, L"OOConverter", L"Некорректные параметы (объект)", eGetContextException);
    			return false;
    		}
    		if(!variant2wchar(meth, paParams[1])) {
    			addError(ADDIN_E_VERY_IMPORTANT, L"OOConverter", L"Некорректные параметы (метод)", eGetContextException);
    			return false;
    		}
    		if(!variant2wchar(meth, paParams[2])) {
    			addError(ADDIN_E_VERY_IMPORTANT, L"OOConverter", L"Некорректные параметы (аргументы)", eGetContextException);
    			return false;
    		}
    		ret = this->callMethod(obj, meth, args, retval);
    		if(ret) {
    			ret = wchar2variant(pvarRetValue, retval);
    		}

    		break;
    	case eMethCreateStruct:
    		break;
    	case eMethCreateSequence:
    		break;
    	case eMethSetSequenceValue:
    		break;
    	case eMethGetSequenceValue:
    		break;
    	case eMethSetProperty:
    		break;
    	case eMethGetProperty:
    		break;
    	case eMethGetValueType:
    		break;
    	default:
    		break;
    }

    return ret; 
}
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

bool CAddInNativeOO::wchar2variant(tVariant *var, const wstring &src)
{
	if(m_iMemory->AllocMemory((void**)&(var->pwstrVal), src.length() * sizeof(WCHAR_T))) {
		convToShortWchar(&var->pwstrVal, src.c_str(), src.length());
		var->wstrLen = src.length();
		TV_VT(var) = VTYPE_PWSTR;
	}
}

bool CAddInNativeOO::variant2wchar(wstring &dst, const tVariant *var)
{
	dst.clear();
	switch(TV_VT(var)) {
	case VTYPE_PWSTR:
	case VTYPE_BLOB:
		if(v2w(dst, var) == false) {
			addError(ADDIN_E_VERY_IMPORTANT, L"OOConverter", L"Невозможно конвертировать значение", eGetContextException);
			return false;
		}
	case VTYPE_BOOL:
		dst = warp(new Any(TV_BOOL(var)));
		break;
	case VTYPE_DATE:
		dst = warp(new Any(TV_DATE(var)));
		break;
	case VTYPE_EMPTY:
		dst = warp(new Any);
		break;
	case VTYPE_ERROR:
		return false;
	case VTYPE_I1:
		dst = warp(new Any((sal_Int8)TV_I1(var)));
		break;
	case VTYPE_I2:
		dst = warp(new Any((sal_Int16)TV_I2(var)));
		break;
	case VTYPE_I4:
		dst = warp(new Any((sal_Int32)TV_I4(var)));
		break;
	case VTYPE_I8:
		dst = warp(new Any((sal_Int64)TV_I8(var)));
		break;
	case VTYPE_INT:
		dst = warp(new Any((sal_Int32)TV_INT(var)));
		break;
	case VTYPE_NULL:
	case VTYPE_PSTR:
	case VTYPE_R4:
		dst = warp(new Any(TV_R4(var)));
		break;
	case VTYPE_R8:
		dst = warp(new Any(TV_R8(var)));
		break;
	case VTYPE_STR_BLOB:
	case VTYPE_UI1:
		dst = warp(new Any((sal_uInt8)TV_UI1(var)));
		break;
	case VTYPE_UI2:
		dst = warp(new Any((sal_uInt32)TV_UI2(var)));
		break;
	case VTYPE_UI4:
		dst = warp(new Any((sal_uInt32)TV_UI4(var)));
		break;
	case VTYPE_UI8:
		dst = warp(new Any((sal_uInt64)TV_UI8(var)));
		break;
	case VTYPE_UINT:
		dst = warp(new Any((sal_uInt32)TV_UINT(var)));
		break;
	case VTYPE_VARIANT:
		break;

	}
}

bool CAddInNativeOO::w2v(tVariant *var, const wstring &src)
{

}

bool CAddInNativeOO::v2w(wstring &var, const tVariant *src)
{
	if(TV_VT(src) == VTYPE_PWSTR) {
		WCHAR_T * ptr = src->pwstrVal;
		for(int i = 0; i < src->wstrLen ; ++i)
			var.push_back(static_cast<wchar_t>(src->pwstrVal[i]));

		return true;
	}

	return false;
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
long CAddInNativeOO::findName(const wchar_t* names[], const wchar_t* name,
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
