#include <sstream>
#include <string>

#include "warpcontainer.h"

using namespace std;

Warp::Warp()
{
	__any.clear();
}

string Warp::warp(const Any *a) const
{
	__any.push_back(a);
	size_t id = __any.size() - 1;
	
	sstream str;
	str << "WARP" << '\0' << "ANY" << '\0' << id;
	return str.str();
}

void Warp::unwarp(const tVariant& data)
{
	
}
