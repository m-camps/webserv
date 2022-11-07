//
// Created by Xander Voorvaart on 10/10/2022.
//

#include "Respond.hpp"

#define ERROR 1

#pragma region "ctor & dtor"

////////// Ctor & Dtor ///////////

Respond::Respond(Exchange& ExchangeRef)
	: _Exchanger(ExchangeRef)
{
	RespondToClient();
}

/* //////////////////////////// */

Respond::Respond(const Respond& ref)
		: _Exchanger(ref._Exchanger)
{
	RespondToClient();
}

/* //////////////////////////// */

Respond::~Respond(void)
{
}

#pragma endregion "ctor & dtor"

//////////// Operator ////////////

Respond& Respond::operator=(const Respond& ref)
{
    if (this != &ref)
    {
    }
    return (*this);
}

//////////// Responder ////////////

/* ///////// External Functions ////////// */

bool isForbiddenPath(const std::string& Path)
{
    std::size_t found = Path.find("../");
    if (found == std::string::npos)
        return (false);
    return (true);
}

/* //////////////////////////// */

uint32_t modifyStatusCode(std::string Path, const std::string& relativePath)
{
    if (isForbiddenPath(Path))
        return (e_Forbidden);
    if ("/" == Path)
        return (e_Redir);
    if (access(relativePath.c_str(), R_OK) != 0)
        return (e_NotFound);
    return (e_OK);
}

void Respond::ResponseBuilder(void)
{
    try
    {
        void (Respond::*FuncPointer)(void) = NULL;
        std::string Method = _Exchanger.getHashMapString("HTTPMethod");

        const s_Methods CompareMethods[3] = {
                {"GET",    &Respond::BuildGet},
                {"POST",   &Respond::BuildPost},
                {"DELETE", &Respond::BuildDelete}
        };

        for (int32_t i = 0; i < 3; i++) {
            if (Method == CompareMethods[i].Method) {
                FuncPointer = CompareMethods[i].FuncPointer;
                (this->*FuncPointer)();
                return;
            }
        }

//        BuildHeader(); <-- 405 Method is not allowed
    }
    catch (const std::exception &e)
    {
        throw (e);
    }
}

/* //////////////////////////// */

void Respond::RespondToClient(void)
{
    std::string response;
    std::string Header;
    std::string body;
    ssize_t ret;

    ResponseBuilder();

    ret = write(_Exchanger.getSocketFD(), response.data(), response.length());
    if (ret < 0 || ret != (ssize_t)response.length())
    {
        std::string ErrorMsg = std::strerror(errno);
        throw (std::runtime_error(ErrorMsg));
    }
}
