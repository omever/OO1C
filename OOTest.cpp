/*
 * OOTest.cpp
 *
 *  Created on: 25.07.2010
 *      Author: om
 */

#include <iostream>
#include <fstream>
#include <com/sun/star/script/XInvocation2.hpp>
#include <com/sun/star/lang/XSingleServiceFactory.hpp>
#include <com/sun/star/reflection/ParamMode.hpp>
#include <com/sun/star/beans/PropertyConcept.hpp>
#include "OOTest.h"

using namespace std;
using namespace com::sun::star::script;
using namespace com::sun::star::lang;
using namespace com::sun::star::reflection;
using namespace com::sun::star::beans;

OOTest::OOTest()
	: Warp()
{
	clearLogBuffer();
	__cmd_counter = 0;
}

OOTest::~OOTest() {
	// TODO Auto-generated destructor stub
}

bool OOTest::getContext(wstring &result)
{
	try {
		__context = ::cppu::bootstrap();
		__service = __context->getServiceManager();
		__factory = Reference<XSingleServiceFactory> (
				__service->createInstanceWithContext(
						OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.script.Invocation" ) ),
						__context),
				UNO_QUERY);
		__introspection = Reference<XIntrospection> (
				__service->createInstanceWithContext(
						OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.beans.Introspection" ) ),
						__context),
				UNO_QUERY);
	}
	catch (Exception &e) {
		log << L"Exception caught " << o2w(e.Message.pData) << endl;
		return false;
	}

	Any * a = new Any(__context);
	result = warp(a);
	return true;
}

bool OOTest::callMethod(const wstring &ref, const wstring &meth, const wstring &arg, wstring &result)
{
	Any *a = unwarp(ref);
	if(a == NULL)
		return false;

	Sequence<Any> parg(1);
	parg[0] = *a;

	if(__cmd_counter++ > 1000) {
		log << L"Ограничение 1000 команд!" << endl;
		return false;
	}

	try {
		if(!__factory.is()) {
			log << L"Error quering factory" << endl;
			return false;
		}

		Reference<XInvocation2> inv (__factory->createInstanceWithArguments(parg), UNO_QUERY);
		if(!inv.is()) {
			log << L"Error quering invocation data " << ref << endl;
			return false;
		}

		InvocationInfo info = inv->getInfoForName(w2o(meth), true);

		log << o2w(info.aName) << endl;
		log << o2w(info.aType.getTypeName()) << endl;
		log << L"Executing method " << meth << endl;

		size_t out_count = 0;

		for(int i = 0; i < info.aParamModes.getLength() ; ++i)
			if(info.aParamModes[i] != ParamMode_IN)
				++out_count;

		Sequence<Any> inParam(info.aParamModes.getLength());
		Sequence<Any> aoutParam(out_count);
		Sequence<short> outParam(out_count);

		out_count = 0;
		for(int i = 0; i < info.aParamModes.getLength() ; ++i)
			if(info.aParamModes[i] != ParamMode_IN)
				outParam[out_count++] = i;

		Any * pars = unwarp(arg);
		Sequence<Any> inParams;

		
		if(pars != NULL) {
			log << L"Params type: " << o2w(pars->getValueTypeName()) << endl;
			*pars >>= inParams;
			log << L"      count: " << inParams.getLength() << endl;
			if(inParam.getLength() < inParams.getLength())
			    inParam.realloc(inParams.getLength());
			    
			for(int i = 0; i < inParams.getLength(); ++i) {
			    inParam[i] = inParams[i];
			    log << L"Param: " << o2w(inParam[i].getValueTypeName()) << endl;
			}
		}

		Any *res = new Any;
		*res = inv->invoke(w2o(meth), inParams, outParam, aoutParam);
		result = warp(res);
		return true;
	}
	catch(IllegalArgumentException &e) {
		log << L"Illegal argument exception" << endl;
		log << o2w(e.Message) << endl;
	}
	catch(CannotConvertException &e) {
		log << L"Can not convert arguments exception" << endl;
		log << o2w(e.Message) << endl;
	}
	catch(InvocationTargetException &e) {
		log << L"Invocation target exception" << endl;
		log << o2w(e.Message) << endl;
	}
	catch(Exception &e) {
		log << L"Exception caught: " << o2w(e.Message) << endl;
	}
	return true;
}

bool OOTest::dumpMethodNames(const wstring &ref, vector<wstring> &methods)
{
	Any *a = unwarp(ref);
	if(a == NULL)
		return false;

	Sequence<Any> parg(1);
	parg[0] = *a;

	log << "Quering for " << o2w(a->getValueTypeName()) << endl;
	try {
		if(!__factory.is()) {
			log << "Error quering factory" << endl;
			return false;
		}
		Reference<XInvocation2> inv (__factory->createInstanceWithArguments(parg), UNO_QUERY);
		if(!inv.is()) {
			log << "Error querying Invocation for info" << endl;
			log << "Falling back to Introspection" << endl;
			if(!__introspection.is()) {
				log << "Error quering introspection" << endl;
				return false;
			}
			Reference<XIntrospectionAccess> inacc (__introspection->inspect(*a), UNO_QUERY);
			if(!inacc.is()) {
				log << "Error quering introspection info" << endl;
				return false;
			}
			Sequence<Property> plist = inacc->getProperties(PropertyConcept::ALL);
			int len = plist.getLength();
			for(int i = 0 ; i < len ; ++i)
				methods.push_back(o2w(plist[i].Name));

			return true;
		}
		Sequence<OUString> list = inv->getMemberNames();
		int len = list.getLength();
		for(int i = 0; i < len ; ++i)
			methods.push_back(o2w(list[i]));
	}
	catch(IllegalArgumentException &e) {
		log << "Illegal argument exception" << endl;
	}
	catch(CannotConvertException &e) {
		log << "Can not convert arguments exception" << endl;
	}
	catch(InvocationTargetException &e) {
		log << "Invocation target exception" << endl;
	}
	catch(Exception &e) {
		OString o = OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US );
		log << "Exception caught: " << o.pData->buffer << endl;
	}
	return true;

}

bool OOTest::dumpAttributeNames(const wstring &ref, vector<wstring> &attributes)
{
	Any *a = unwarp(ref);
	if(a == NULL)
		return false;

	Sequence<Any> parg(1);
	parg[0] = *a;

	try {
		if(!__factory.is()) {
			log << "Error quering factory" << endl;
			return false;
		}
		Reference<XInvocation2> inv (__factory->createInstanceWithArguments(parg), UNO_QUERY);
	}
	catch(IllegalArgumentException &e) {
		log << "Illegal argument exception" << endl;
	}
	catch(CannotConvertException &e) {
		log << "Can not convert arguments exception" << endl;
	}
	catch(InvocationTargetException &e) {
		log << "Invocation target exception" << endl;
	}
	catch(Exception &e) {
		OString o = OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US );
		log << "Exception caught: " << o.pData->buffer << endl;
	}
	return true;
}

bool OOTest::createStruct(const wstring &id, wstring &result)
{
	Reference<XIdlReflection> iref (__service->createInstanceWithContext(w2o(L"com.sun.star.reflection.CoreReflection"), __context), UNO_QUERY);
	if(! iref.is()) {
		log << "Unable to request reflection" << endl;
		return false;
	}

	Reference<XIdlClass> cls = iref->forName(w2o(id));
	Any *obj = new Any;

	cls->createObject(*obj);
	result = warp(obj);
	return true;
}

bool OOTest::createPropertyValue(wstring & result)
{
	Reference<PropertyValue> prop;
	Any *a = new Any(prop);
	result = warp(a);

	return true;
}

bool OOTest::createSequence(wstring &result, size_t size)
{
	Sequence<Any> _sequence;
	Any * a = new Any(_sequence);
	result = warp(a);
	return true;
}

bool OOTest::setSequenceValue(const wstring &sequence, const size_t elem, const wstring &data)
{
	Any *a = unwarp(sequence);
	if(a == NULL || a->getValueTypeClass() != TypeClass_SEQUENCE)
		return false;

	Sequence<Any> _sequence;
	*a >>= _sequence;
	if(_sequence.getLength() <= elem)
		_sequence.realloc(elem+1);

	Any * anyvalue = unwarp(data);
	if(anyvalue == NULL)
		_sequence[elem] = Any(w2o(data));
	else
		_sequence[elem] = *anyvalue;

	*a <<= _sequence;
	return true;
}

bool OOTest::getSequenceValue(const wstring &sequence, const size_t elem, wstring &data)
{
	Any *a = unwarp(sequence);
	if(a == NULL || a->getValueTypeClass() != TypeClass_SEQUENCE)
		return false;

	Sequence<Any> _sequence;
	*a >>= _sequence;
	if(_sequence.getLength() <= elem)
		return false;

	Any *b = new Any(_sequence[elem]);
	data = warp(b);
	return true;
}

bool OOTest::setProperty(const wstring & property, const wstring & name, const wstring & value)
{
	Any *a = unwarp(property);
	if(a == NULL)
		return false;

	Sequence<Any> parg(1);
	parg[0] = *a;

	try {
		if(!__factory.is()) {
			log << "Error quering factory" << endl;
			return false;
		}

		OUString uname = w2o(name);

		Reference<XInvocation2> inv (__factory->createInstanceWithArguments(parg), UNO_QUERY);
		if(!inv->hasProperty(uname)) {
			log << "No such property" << endl;
			return false;
		}

		Any *anyvalue = unwarp(value);
		if(anyvalue != NULL)
			inv->setValue(uname, *anyvalue);
		else
			inv->setValue(uname, Any(w2o(value)));
	}

	catch(IllegalArgumentException &e) {
		log << "Illegal argument exception" << endl;
	}
	catch(CannotConvertException &e) {
		log << "Can not convert arguments exception" << endl;
	}
	catch(InvocationTargetException &e) {
		log << "Invocation target exception" << endl;
	}
	catch(Exception &e) {
		OString o = OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US );
		log << "Exception caught: " << o.pData->buffer << endl;
	}
	return true;
}

bool OOTest::getProperty(const wstring & property, const wstring & name, wstring & value)
{
	Any *a = unwarp(property);
	if(a == NULL)
		return false;

	Sequence<Any> parg(1);
	parg[0] = *a;

	try {
		if(!__factory.is()) {
			log << "Error quering factory" << endl;
			return false;
		}

		OUString uname = w2o(name);

		Reference<XInvocation2> inv (__factory->createInstanceWithArguments(parg), UNO_QUERY);
		if(!inv->hasProperty(uname)) {
			log << "No such property" << endl;
			return false;
		}

		Any *a = new Any(inv->getValue(uname));
		value = warp(a);

		return true;
	}

	catch(IllegalArgumentException &e) {
		log << "Illegal argument exception" << endl;
	}
	catch(CannotConvertException &e) {
		log << "Can not convert arguments exception" << endl;
	}
	catch(InvocationTargetException &e) {
		log << "Invocation target exception" << endl;
	}
	catch(Exception &e) {
		OString o = OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US );
		log << "Exception caught: " << o.pData->buffer << endl;
	}
	return false;
}

bool OOTest::getValueType(const wstring &any, wstring &type)
{
	Any *a = unwarp(any);
	if(a == NULL)
		return false;

	type = o2w(a->getValueTypeName());
	return true;
}

bool OOTest::cleanupCache()
{
	cleanup();
	__factory.clear();
	__service.clear();
	__context.clear();
	clearLogBuffer();
}

bool OOTest::getLogBuffer(wstring &dest)
{
	dest = log.str();
	clearLogBuffer();
	return true;
}

wstring OOTest::o2w(const OUString & src)
{
	wstring result;

	for(size_t len = 0 ; len < src.pData->length ; ++len)
		result.push_back(static_cast<wchar_t>(src.pData->buffer[len]));

	return result;
}



OUString OOTest::w2o(const wstring & src)
{
	OUString result;

	for(size_t len = 0 ; len < src.length() ; ++len)
		result += OUString::valueOf(static_cast<sal_Unicode>(src[len]));

	return result;
}

void OOTest::clearLogBuffer()
{
	log.str(L"");
//	log.open("/log/main.out", ios::app);
}




