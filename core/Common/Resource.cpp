#include "..\e2dtool.h"

e2d::Resource::Resource(const String & fileName)
	: _isResource(false)
	, _fileName(fileName)
	, _resNameId(0)
	, _resType()
{
}

e2d::Resource::Resource(size_t resNameId, const String & resType)
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

size_t e2d::Resource::getResNameId() const
{
	return _resNameId;
}

const e2d::String & e2d::Resource::getResType() const
{
	return _resType;
}

size_t e2d::Resource::getKey() const
{
	return _isResource ? _resNameId : _fileName.getHashCode();
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
