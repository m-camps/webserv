//
// Created by Xander Voorvaart on 10/10/22.
//

#include "Exchange.hpp"

Exchange::Exchange(void)
    : _statusCode(200), _SocketFD(0)
{
}

Exchange::Exchange(Server NewSever, int32_t NewSocketFD)
    : _server(NewSever), _body(""),
        _statusCode(200), _SocketFD(NewSocketFD)
{
}

Exchange::Exchange(const Exchange &ref)
    : _server(ref._server), _dictHeader(ref._dictHeader),
        _body(ref._body), _statusCode(ref._statusCode), _SocketFD(ref._SocketFD)
{
}

Exchange::~Exchange(void)
{
}

Exchange& Exchange::operator=(const Exchange& ref)
{
    if (this != &ref)
    {
    }
    return (*this);
}

Server Exchange::getServer(void) const
{
    return (_server);
}

HashMap Exchange::getHashMap(void) const
{
    return (_dictHeader);
}

std::string Exchange::getBody(void) const
{
    return (_body);
}

int32_t Exchange::getStatusCode(void) const
{
    return (_statusCode);
}

int32_t Exchange::getSocketFD(void) const
{
    return (_SocketFD);
}

void Exchange::setServer(const Server NewServer)
{
    _server = NewServer;
}

void Exchange::setHashMap(const HashMap NewHashMap)
{
    _dictHeader = NewHashMap;
}

void Exchange::setBody(const std::string NewBody)
{
    _body = NewBody;
}

void Exchange::setStatusCode(const uint32_t NewStatus)
{
    _statusCode = NewStatus;
}

void Exchange::setHeader(const std::string NewHeader)
{
    _header = NewHeader;
}

void Exchange::addHashMapNode(const std::string NameNode, const std::string ContentNode)
{
    _dictHeader[NameNode] = ContentNode;
}
