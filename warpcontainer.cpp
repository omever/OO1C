#include <iostream>
#include <sstream>
#include <string>

#include "warpcontainer.h"

using namespace std;

Warp::Warp()
{
	__any.clear();
}

Warp::~Warp()
{
	std::vector<Any *>::iterator i, iend = __any.end();
	for(i = __any.begin(); i != iend ; ++i)
		delete *i;
}

wstring Warp::warp(Any *a)
{
	__any.push_back(a);
	size_t id = __any.size() - 1;
	
	wstringstream str;
	str << "WARP" << ':' << "ANY" << ':' << id;
	return str.str();
}

Any * Warp::unwarp(wstring str)
{
	if(str.compare(0, 5, L"WARP:"))
		return NULL;

    size_t s1 = str.find(':', 5);
    if(s1 == string::npos)
        return NULL;

    wstring atype(str.substr(5, s1 - 5));
    wstring addr(str.substr(s1+1));

    wcout << atype << endl;
    wcout << addr << endl;
    wstringstream s;
    s << addr;

    s >> s1;

    if(atype == L"ANY" && s1 >=0 && s1 < __any.size()) {
        return __any[s1];
    }

    return NULL;
}
