/*
 * OOTest.h
 *
 *  Created on: 25.07.2010
 *      Author: om
 */

#ifndef OOTEST_H_
#define OOTEST_H_

#include <string>
#include <fstream>
#include "warpcontainer.h"

#include <com/sun/star/lang/XSingleServiceFactory.hdl>
#include <com/sun/star/beans/XIntrospection.hpp>

using namespace std;

class OOTest
	: public Warp
{
public:
	OOTest();
	virtual ~OOTest();
	bool getContext(wstring &result);
	bool callMethod(const wstring &ref, const wstring &meth, const wstring &arg, wstring &result);
	bool dumpMethodNames(const wstring &ref, vector<wstring> &methods);
	bool dumpAttributeNames(const wstring &ref, vector<wstring> &attributes);
	bool createStruct(const wstring &id, wstring &result);
	bool createPropertyValue(wstring &result);
	bool createSequence(wstring &result, const size_t size);
	bool setSequenceValue(const wstring &sequence, const size_t elem, const wstring &data);
	bool getSequenceValue(const wstring &sequence, const size_t elem, wstring &data);
	bool setProperty(const wstring &property, const wstring &name, const wstring &value);
	bool getProperty(const wstring &property, const wstring &name, wstring &value);
	bool getValueType(const wstring &any, wstring &type);
	static OUString w2o(const wstring &src);
	static wstring o2w(const OUString &src);
	wofstream log;
private:
    Reference< XComponentContext > __context;
    Reference< XMultiComponentFactory > __service;
    Reference< XSingleServiceFactory > __factory;
    Reference< XIntrospection > __introspection;
};

#endif /* OOTEST_H_ */
