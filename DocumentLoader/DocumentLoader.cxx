/*************************************************************************
 *
 *  $RCSfile: DocumentLoader.cxx,v $
 *
 *  $Revision: 1.13 $
 *
 *  last change: $Author: rt $ $Date: 2008-07-11 14:25:41 $
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

/*****************************************************************************
 *****************************************************************************
 *
 * Simple client application using the UnoUrlResolver service.
 *
 *****************************************************************************
 *****************************************************************************/
#include <stdio.h>
#include <wchar.h>

#include <cppuhelper/bootstrap.hxx>

#include <osl/file.hxx>
#include <osl/process.h>

#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/registry/XSimpleRegistry.hpp>

#include <string.h>

using namespace rtl;
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::beans;
using namespace com::sun::star::bridge;
using namespace com::sun::star::frame;
using namespace com::sun::star::registry;


//============================================================================
int SAL_CALL main( int argc, char **argv )
{
    OUString sConnectionString(RTL_CONSTASCII_USTRINGPARAM("uno:socket,host=localhost,port=2083;urp;StarOffice.ServiceManager"));
 	if (argc < 2)
	{
		printf("using: DocumentLoader <file_url> [<uno_connection_url>]\n\n"
			   "example: DocumentLoader  \"file:///e:/temp/test.odt\" \"uno:socket,host=localhost,port=2083;urp;StarOffice.ServiceManager\"\n");
		exit(1);
	}
 	if (argc == 3)
	{
		sConnectionString = OUString::createFromAscii(argv[2]);        
	}
    
	// Creates a simple registry service instance.
    Reference< XSimpleRegistry > xSimpleRegistry(
        ::cppu::createSimpleRegistry() );

    // Connects the registry to a persistent data source represented by an URL.
    xSimpleRegistry->open( OUString( RTL_CONSTASCII_USTRINGPARAM(
        "DocumentLoader.rdb") ), sal_True, sal_False );

    /* Bootstraps an initial component context with service manager upon a given
       registry. This includes insertion of initial services:
       - (registry) service manager, shared lib loader,
       - simple registry, nested registry,
       - implementation registration
       - registry typedescription provider, typedescription manager (also
         installs it into cppu core)
    */
    Reference< XComponentContext > xComponentContext(
        ::cppu::bootstrap_InitialComponentContext( xSimpleRegistry ) );
    
    /* Gets the service manager instance to be used (or null). This method has
       been added for convenience, because the service manager is a often used
       object.
    */
	Reference< XMultiComponentFactory > xMultiComponentFactoryClient(
		xComponentContext->getServiceManager() );

    /* Creates an instance of a component which supports the services specified
       by the factory.
    */
    Reference< XInterface > xInterface =
        xMultiComponentFactoryClient->createInstanceWithContext( 
            OUString::createFromAscii( "com.sun.star.bridge.UnoUrlResolver" ),
            xComponentContext );

    Reference< XUnoUrlResolver > resolver( xInterface, UNO_QUERY );

    // Resolves the component context from the office, on the uno URL given by argv[1].
    try
    {    
        xInterface = Reference< XInterface >( 
            resolver->resolve( sConnectionString ), UNO_QUERY );
    }
    catch ( Exception& e )
    {
		printf("Error: cannot establish a connection using '%s':\n       %s\n",
               OUStringToOString(sConnectionString, RTL_TEXTENCODING_ASCII_US).getStr(),
               OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr());
		exit(1);        
    }
    
    // gets the server component context as property of the office component factory
    Reference< XPropertySet > xPropSet( xInterface, UNO_QUERY );
    xPropSet->getPropertyValue( OUString::createFromAscii("DefaultContext") ) >>= xComponentContext;

    // gets the service manager from the office
    Reference< XMultiComponentFactory > xMultiComponentFactoryServer(
        xComponentContext->getServiceManager() );
  
    /* Creates an instance of a component which supports the services specified
       by the factory. Important: using the office component context.
    */
    Reference < XComponentLoader > xComponentLoader(
        xMultiComponentFactoryServer->createInstanceWithContext( 
            OUString( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.frame.Desktop" ) ),
            xComponentContext ), UNO_QUERY );
	
    /* Loads a component specified by an URL into the specified new or existing
       frame.
    */
    OUString sAbsoluteDocUrl, sWorkingDir, sDocPathUrl;
    osl_getProcessWorkingDir(&sWorkingDir.pData);
    osl::FileBase::getFileURLFromSystemPath( OUString::createFromAscii(argv[1]), sDocPathUrl);
    osl::FileBase::getAbsoluteFileURL( sWorkingDir, sDocPathUrl, sAbsoluteDocUrl);
    
    Reference< XComponent > xComponent = xComponentLoader->loadComponentFromURL(
        sAbsoluteDocUrl, OUString( RTL_CONSTASCII_USTRINGPARAM("_blank") ), 0,
        Sequence < ::com::sun::star::beans::PropertyValue >() );

	// dispose the local service manager
    Reference< XComponent >::query( xMultiComponentFactoryClient )->dispose();

    return 0;
}
