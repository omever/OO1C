/*************************************************************************
 *
 *  $RCSfile: office_connect.cxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: rt $ $Date: 2008-07-11 14:24:32 $
 *
 *  The Contents of this file are made available subject to the terms of
 *  the BSD license.
 *  
 *  Copyright (c) 2003 by Sun Microsystems, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Sun Microsystems, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *     
 *************************************************************************/

#include <stdio.h>

#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/reflection/XIdlReflection.hpp>
#include <com/sun/star/reflection/XIdlClass.hpp>
#include <com/sun/star/reflection/XIdlArray.hpp>
#include <com/sun/star/reflection/XIdlMember.hpp>
#include <com/sun/star/reflection/XIdlMethod.hpp>

using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::bridge;
using namespace com::sun::star::frame;
using namespace com::sun::star::reflection;
using namespace rtl;
using namespace cppu;

int main( )
{
	// create the initial component context
	Reference< XComponentContext > rComponentContext;
	try {
	    rComponentContext = ::cppu::bootstrap();
	
//		defaultBootstrap_InitialComponentContext();
	// retrieve the servicemanager from the context
	Reference< XMultiComponentFactory > rServiceManager = 
		rComponentContext->getServiceManager();

	// instantiate a sample service with the servicemanager.
	Reference< XInterface > rInstance =
		rServiceManager->createInstanceWithContext( 
			OUString::createFromAscii("com.sun.star.bridge.UnoUrlResolver" ),
			rComponentContext );

    // gets the service manager from the office
    Reference< XMultiComponentFactory > cmgr(
        rComponentContext->getServiceManager() );

    Reference< XInterface > r = cmgr->createInstanceWithContext( 
        OUString::createFromAscii("com.sun.star.frame.Desktop"), rComponentContext);
    
    Any a(r);
    
    Reference< XInterface > xInterface = cmgr->createInstanceWithContext(
	OUString::createFromAscii("com.sun.star.reflection.CoreReflection"), rComponentContext);
    if(xInterface.is()) {
	Reference< XIdlReflection > m(xInterface, UNO_QUERY);
        Reference< XIdlClass > idl1 = m->getType(a);

	Sequence< Reference< XIdlMethod > > mlist = idl1->getMethods();
	for(int i = 0; i < mlist.getLength(); ++i) {
		OString o = OUStringToOString( mlist[i]->getName(), RTL_TEXTENCODING_ASCII_US );
		printf( "Method: %s\n", o.pData->buffer );
	}
    } else {
	printf("Error IS!\n");
    }
        printf( "Connected sucessfully to the office\n" );
	}	
	catch( Exception &e )
	{
		OString o = OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US );
		printf( "Error: %s\n", o.pData->buffer );
		return 1;
	}
	return 0;
}
