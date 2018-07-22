#include "..\e2dtool.h"

e2d::Resource::Resource(const String & fileName)
	: _isFile(true)
	, _fileName(fileName)
	, _resNameId(0)
	, _resType()
{
}

e2d::Resource::Resource(size_t resNameId, const String & resType)
	: _isFile(false)
	, _fileName()
	, _resNameId(resNameId)
	, _resType(resType)
{
}

bool e2d::Resource::isFile() const
{
	return _isFile;
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
	return _isFile ? _fileName.getHashCode() : _resNameId;
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
