//
// Created by Xander Voorvaart on 10/10/22.
//

#include "Exchange.hpp"

#pragma region "ctor & dtor"

Exchange::Exchange(void)
    : _statusCode(200), _SocketFD(0)
{
}

/* //////////////////////////// */

Exchange::Exchange(Server NewSever, int32_t NewSocketFD)
    : _body(""), _statusCode(200), _server(NewSever), _SocketFD(NewSocketFD)
{
}

/* //////////////////////////// */

Exchange::Exchange(const Exchange &ref)
    : _dictHeader(ref._dictHeader), _body(ref._body),
        _statusCode(ref._statusCode), _server(ref._server), _SocketFD(ref._SocketFD)
{
}

/* //////////////////////////// */

Exchange::~Exchange(void)
{
}

#pragma endregion ctoranddtor

/* //////////////////////////// */

Exchange& Exchange::operator=(const Exchange& ref)
{
    if (this != &ref)
    {
    }
    return (*this);
}

#pragma region getter

/* ////////// Getter //////////// */

Server Exchange::getServer(void) const
{
    return (_server);
}

/* //////////////////////////// */

HashMap Exchange::getHashMap(void) const
{
    return (_dictHeader);
}

/* //////////////////////////// */

std::string Exchange::getHashMapString(const std::string& RequestedMap) const
{
        std::string second = _dictHeader.find(RequestedMap)->second;

        if (second == "")
            throw (std::invalid_argument("Invalid string"));
        return (second);
}

/* //////////////////////////// */

std::string Exchange::getHeader(void) const
{
	return (_header);
}

/* //////////////////////////// */

std::string Exchange::getBody(void) const
{
    return (_body);
}

/* //////////////////////////// */

uint32_t Exchange::getStatusCode(void) const
{
    return (_statusCode);
}

/* //////////////////////////// */

int32_t Exchange::getSocketFD(void) const
{
    return (_SocketFD);
}

/* //////////////////////////// */

void Exchange::setHashMap(const HashMap NewHashMap)
{
    _dictHeader = NewHashMap;
}

#pragma endregion getter

#pragma region setter

/* ////////// Setter //////////// */

void Exchange::setBody(const std::string NewBody)
{
    _body = NewBody;
}

/* //////////////////////////// */

void Exchange::setStatusCode(const uint32_t NewStatus)
{
    _statusCode = NewStatus;
}

/* //////////////////////////// */

void Exchange::setHeader(const std::string NewHeader)
{
    _header = NewHeader;
}

#pragma endregion setter

#pragma region adders

/* ////////// Adders //////////// */

void Exchange::addHashMapNode(const std::string NameNode, const std::string ContentNode)
{
    _dictHeader[NameNode] = ContentNode;
}

/* //////////////////////////// */

void Exchange::addLineToHeader(const std::string NewLine)
{
	std::string NewHeader = getHeader() + NewLine;

	setHeader(NewHeader);
}

#pragma endregion adders
