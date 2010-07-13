#ifndef WARPCONTAINER_H
#define WARPCONTAINER_H

#include <string>

class Warp
{
public:
	Warp();
	~Warp();
	std::string warp(const Any *) const;
	void unwarp(const tVariant&);
protected:
private:
	std::vector<Any *> __any;
};

#endif
