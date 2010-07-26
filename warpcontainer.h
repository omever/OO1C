#ifndef WARPCONTAINER_H
#define WARPCONTAINER_H

#include <string>
#include <vector>
#include <sstream>
//#include "types.h"

#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/lang/XSingleComponentFactory.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/reflection/XIdlReflection.hpp>
#include <com/sun/star/reflection/XIdlClass.hpp>
#include <com/sun/star/reflection/XIdlArray.hpp>
#include <com/sun/star/reflection/XIdlMember.hpp>
#include <com/sun/star/reflection/XIdlMethod.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>

using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::bridge;
using namespace com::sun::star::frame;
using namespace com::sun::star::reflection;
using namespace com::sun::star::beans;
using namespace rtl;
using namespace cppu;

class Warp
{
public:
	Warp();
	~Warp();
	std::wstring warp(Any *);
	Any * unwarp(const std::wstring);
protected:
private:
	std::vector<Any *> __any;
};

#endif
