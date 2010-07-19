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
}

string Warp::warp(Any *a)
{
	__any.push_back(a);
	size_t id = __any.size() - 1;
	
	stringstream str;
	str << "WARP" << ':' << "ANY" << ':' << id;
	return str.str();
}

Any * Warp::unwarp(string str)
{
	if(str.compare(0, 5, "WARP:"))
		return NULL;

    size_t s1 = str.find(':', 5);
    if(s1 == string::npos)
        return NULL;

    string atype(str.substr(5, s1 - 5));
    string addr(str.substr(s1+1));

    stringstream s;
    s << addr;

    s >> s1;

    if(atype == "ANY" && s1 >=0 && s1 < __any.size()) {
        return __any[s1];
    }

    return NULL;
}
