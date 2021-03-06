#ifndef __ADDINNATIVE_H__
#define __ADDINNATIVE_H__

#include "ComponentBase.h"
#include "AddInDefBase.h"
#include "IMemoryManager.h"
#include "OOTest.h"
///////////////////////////////////////////////////////////////////////////////
// class CAddInNativeOO
class CAddInNativeOO : public IComponentBase, public OOTest
{
public:
    enum Props
    {
        ePropLast      // Always last
    };

    enum Methods
    {
        eMethGetContext = 0,
        eMethInvoke,
        eMethCreateStruct,
        eMethCreateSequence,
        eMethSetSequenceValue,
        eMethGetSequenceValue,
        eMethSetProperty,
        eMethGetProperty,
        eMethGetValueType,
        eMethCleanup,
        eMethGetLog,
        eMethShowValue,
        eMethLast      // Always last
    };
    
    enum ErrorCodes
    {
	eNoError = 0,
	eGetContextException,
	eErrLast
    };

    CAddInNativeOO(void);
    virtual ~CAddInNativeOO();
    // IInitDoneBase
    virtual bool ADDIN_API Init(void*);
    virtual bool ADDIN_API setMemManager(void* mem);
    virtual long ADDIN_API GetInfo();
    virtual void ADDIN_API Done();
    // ILanguageExtenderBase
    virtual bool ADDIN_API RegisterExtensionAs(WCHAR_T**);
    virtual long ADDIN_API GetNProps();
    virtual long ADDIN_API FindProp(const WCHAR_T* wsPropName);
    virtual const WCHAR_T* ADDIN_API GetPropName(long lPropNum, long lPropAlias);
    virtual bool ADDIN_API GetPropVal(const long lPropNum, tVariant* pvarPropVal);
    virtual bool ADDIN_API SetPropVal(const long lPropNum, tVariant* varPropVal);
    virtual bool ADDIN_API IsPropReadable(const long lPropNum);
    virtual bool ADDIN_API IsPropWritable(const long lPropNum);
    virtual long ADDIN_API GetNMethods();
    virtual long ADDIN_API FindMethod(const WCHAR_T* wsMethodName);
    virtual const WCHAR_T* ADDIN_API GetMethodName(const long lMethodNum, 
                            const long lMethodAlias);
    virtual long ADDIN_API GetNParams(const long lMethodNum);
    virtual bool ADDIN_API GetParamDefValue(const long lMethodNum, const long lParamNum,
                            tVariant *pvarParamDefValue);   
    virtual bool ADDIN_API HasRetVal(const long lMethodNum);
    virtual bool ADDIN_API CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray);
    virtual bool ADDIN_API CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray);
    // LocaleBase
    virtual void ADDIN_API SetLocale(const WCHAR_T* loc);
protected:
    bool wchar2variant(tVariant *var, const wstring &src);
    bool variant2wchar(wstring &dst, const tVariant *var);
	bool w2v(tVariant *var, const wstring &src);
	bool v2w(wstring &var, const tVariant *src);
	bool simpleAny2Variant(tVariant *var, const Any &src);
private:
    long findName(const wchar_t* names[], const wchar_t* name, const uint32_t size) const;
    void addError(uint32_t wcode, const wchar_t* source, 
                    const wchar_t* descriptor, long code);
    // Attributes
    IAddInDefBase      *m_iConnect;
    IMemoryManager     *m_iMemory;

    bool                m_boolEnabled;
    uint32_t            m_uiTimer;
};

#endif //__ADDINNATIVE_H__
