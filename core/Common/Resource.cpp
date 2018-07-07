#include "..\e2dtool.h"

e2d::Resource::Resource(const String & fileName)
	: _isResource(false)
	, _fileName(fileName)
	, _resNameId(0)
	, _resType()
{
}

e2d::Resource::Resource(int resNameId, const String & resType)
	: _isResource(true)
	, _fileName()
	, _resNameId(resNameId)
	, _resType(resType)
{
}

bool e2d::Resource::isResource() const
{
	return _isResource;
}

const e2d::String & e2d::Resource::getFileName() const
{
	return _fileName;
}

int e2d::Resource::getResNameId() const
{
	return _resNameId;
}

const e2d::String & e2d::Resource::getResType() const
{
	return _resType;
}

int e2d::Resource::getKey() const
{
	if (_isResource)
	{
		return _resNameId;
	}
	else
	{
		return _fileName.getHashCode();
	}
}

bool e2d::Resource::operator>(const Resource &res) const
{
	return this->getKey() > res.getKey();
}

bool e2d::Resource::operator>=(const Resource &res) const
{
	return this->getKey() >= res.getKey();
}

bool e2d::Resource::operator<(const Resource &res) const
{
	return this->getKey() < res.getKey();
}

bool e2d::Resource::operator<=(const Resource &res) const
{
	return this->getKey() <= res.getKey();
}
