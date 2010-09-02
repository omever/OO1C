/*
 * ootest.cpp
 *
 *  Created on: 25.07.2010
 *      Author: om
 */

#include <iostream>
#include "OOTest.h"

using namespace std;

int main(void)
{
	OOTest t;
	wstring context;
	if(t.getContext(context)) {
		cout << "Success" << endl;
	} else {
		cout << "File: " << __FILE__ << ", line: " << __LINE__ << " FAIL: :(" << endl;
	}

	wcout << context << endl;

	wstring manager;
	wstring meth = L"getServiceManager";
	wstring par;

	t.createSequence(par, 0);

	if(t.callMethod(context, meth, par, manager)) {
		wcout << L"Success: " << manager << endl;
	} else {
		wcout << "File: " << __FILE__ << ", line: " << __LINE__ << " FAIL: :(" << endl;
		return -1;
	}


	t.createSequence(par, 0);
	t.setSequenceValue(par, 0, L"com.sun.star.frame.Desktop");
	t.setSequenceValue(par, 1, context);

	wstring desktop;
	meth = L"createInstanceWithContext";
	if(t.callMethod(manager, meth, par, desktop)) {
		wcout << L"Success: " << desktop << endl;
	} else {
		wcout << "File: " << __FILE__ << ", line: " << __LINE__ << " FAIL: :(" << endl;
		return -1;
	}

	vector<wstring> methods;
	if(t.dumpMethodNames(desktop, methods)) {
		cout << "Success" << endl;
		for(int i = 0; i < methods.size(); ++i)
			wcout << L"+ " << methods[i] << endl;
	} else {
		cout << "File: " << __FILE__ << ", line: " << __LINE__ << " FAIL:" << endl;
	}

	wstring tmp;
	t.createSequence(tmp, 0);

	wstring oprops;
	wstring tr;
	t.createStruct(L"com.sun.star.beans.PropertyValue", oprops);
	//t.createPropertyValue(oprops);
	PropertyValue val;
/*	val.Name = t.w2o(L"Hidden");
	val.Value = Any(true);
*/
	val.Name = t.w2o(L"123");
	val.Value = Any();
	Any *a = new Any;
	*a <<= val;

	oprops = t.warp(a);
	t.setProperty(oprops, L"Name", L"Hidden");
	t.setProperty(oprops, L"Value", t.warp(new Any(true)));

	a = t.unwarp(oprops);
	*a >>= val;
	wcout << t.o2w(val.Name) << endl;

	t.getLogBuffer(tr);
	wcout << tr << endl;
	return 0;

	t.setSequenceValue(tmp, 0, oprops);

	t.createSequence(par, 0);
	t.setSequenceValue(par, 0, L"private:factory/scalc");
	t.setSequenceValue(par, 1, L"_blank");
	t.setSequenceValue(par, 2, L"0");
	t.setSequenceValue(par, 3, tmp);

	meth = L"loadComponentFromURL";
	wstring scalc;
	t.callMethod(desktop, meth, par, scalc);

    t.createSequence(par, 0);
    wstring sheets;
    meth = L"getSheets";
    t.callMethod(scalc, meth, par, sheets);

    t.createSequence(par, 0);
    t.setSequenceValue(par, 0, L"0");
    wstring sheet;
    meth = L"getByIndex";
    t.callMethod(sheets, meth, par, sheet);

t.createSequence(par, 0);
    t.setSequenceValue(par, 0, t.warp(new Any((sal_Int32)4)));
    t.setSequenceValue(par, 1, t.warp(new Any((sal_Int32)4)));
    wstring cell;
    meth = L"getCellByPosition";
    t.callMethod(sheet, meth, par, cell);

    t.createSequence(par, 0);
    wstring cursor;
    meth = L"createTextCursor";
    t.callMethod(cell, meth, par, cursor);

    methods.clear();
        if(t.dumpMethodNames(cursor, methods)) {
                cout << "Success" << endl;
                for(int i = 0; i < methods.size(); ++i)
                        wcout << L"+ " << methods[i] << endl;
        } else {
                cout << "File: " << __FILE__ << ", line: " << __LINE__ << " FAIL:" << endl;
        }

    t.createSequence(par, 0);
    t.setSequenceValue(par, 0, cursor);
    t.setSequenceValue(par, 1, L"Привет НЕГРЫ");
    t.setSequenceValue(par, 2, L"False");
    meth = L"insertString";
    wstring result;
    t.callMethod(cell, meth, par, result);

    wstring StyleFamilies;
    t.createSequence(par, 0);
    t.callMethod(scalc, L"getStyleFamilies", par, StyleFamilies);

    wstring props;

    t.createSequence(par, 0);
    t.setSequenceValue(par, 0, L"PageStyles");

    methods.clear();

    t.callMethod(StyleFamilies, L"getByName", par, props);


    wstring scnt;
    t.createSequence(par, 0);
    t.callMethod(props, L"getCount", par, scnt);

    wstring props_seq;
    t.createSequence(props_seq, 0);

    wstringstream ss(scnt);
    int cnt;
    ss >> cnt;

    for(int i = 0; i < cnt; ++i ) {
    	wstringstream ss;
    	ss << i;
    	t.setSequenceValue(props_seq, 0, ss.str());

    	wstring style;
    	t.callMethod(props, L"getByIndex", props_seq, style);
    	t.setProperty(style, L"ScaleToPagesX", L"1");
    }

    wstring pp;
    t.createSequence(pp, 0);

    t.createSequence(par, 0);

    wstring str;
    wstring filterdata;
    t.createSequence(filterdata, 0);

/*    t.createStruct(L"com.sun.star.beans.PropertyValue", str);
    t.setProperty(str, L"Name", L"Quality");
    t.setProperty(str, L"Value", L"70");
    t.setSequenceValue(filterdata, 0, str);

    t.createStruct(L"com.sun.star.beans.PropertyValue", str);
    t.setProperty(str, L"Name", L"ReduceImageResolution");
    t.setProperty(str, L"Value", L"false");
    t.setSequenceValue(filterdata, 1, str);
*/
    t.createStruct(L"com.sun.star.beans.PropertyValue", str);
    t.setProperty(str, L"Name", L"FilterName");
    t.setProperty(str, L"Value", L"calc_pdf_Export");
    t.setSequenceValue(pp, 0, str);
/*
    t.createStruct(L"com.sun.star.beans.PropertyValue", str);
    t.setProperty(str, L"Name", L"FilterData");
    t.setProperty(str, L"Value", filterdata);
    t.setSequenceValue(pp, 1, str);
*/

    t.setSequenceValue(par, 0, L"file:///home/om/workspace/ootest/test.pdf");
    t.setSequenceValue(par, 1, pp);

    meth = L"storeToURL";
    t.callMethod(scalc, meth, par, result);


    t.createSequence(pp, 0);
    t.setSequenceValue(pp, 0, L"True");
    meth = L"close";
    t.callMethod(scalc, meth, pp, result);

    cout << "well done!" << endl;

    wstring dest;
    t.getLogBuffer(dest);

    wcout << dest << endl;
	return 0;
}
