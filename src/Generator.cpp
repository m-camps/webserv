//
// Created by Xander Voorvaart on 11/1/22.
//

#include "Generator.hpp"

Generator::Generator(void)
{
}

Generator::~Generator(void)
{
}

////////////// Generators //////////////

/*
 * The status of the site should ALWAYS be the first line of the header.
 * The rest can be any line.
 */
void Generator::generateStatus(Exchange& Exchanger)
{
    HashMap tempHash = Exchanger.getHashMap();
    uint32_t StatusCode = Exchanger.getStatusCode();
    std::string StatusLine = tempHash.find("HTTPVersion")->second;

    StatusLine += " " + ToString(StatusCode) + " \r\n";
    Exchanger.setHeader(StatusLine);
}

/* //////////////////////////// */

void Generator::generateTransferEncoding(Exchange& Exchanger)
{
    std::string TransferEncoding = "Transfer-Encoding: chunked\r\n";
    Exchanger.addLineToHeader(TransferEncoding);
}

/* //////////////////////////// */

void Generator::generateContentLength(Exchange& Exchanger, std::size_t BodyLength)
{
    std::string ContentLength = "Content-Length: " + ToString(BodyLength) + "\r\n";
    Exchanger.addLineToHeader(ContentLength);
}

/* //////////////////////////// */

std::string ConvertDecimalToHex(size_t decimal)
{
    std::stringstream ss;

    ss << std::hex << decimal;
    return (ss.str());
}

std::string Generator::generateChunk(Exchange& Exchanger)
{
    std::string Chunk;
    std::string Body = Exchanger.getBody();

    if (Body.length() > MAXBYTES)
    {
        Chunk = ConvertDecimalToHex(MAXBYTES) + "\r\n";
        Chunk += Body.substr(0, MAXBYTES) + "\r\n\r\n";
        Body = Body.substr(MAXBYTES, Body.length() - MAXBYTES);
        Exchanger.setBody(Body);
    }
    else
    {
        Chunk = ConvertDecimalToHex(Body.length()) + "\r\n";
        Chunk += Body.substr(0, Body.length()) + "\r\n\r\n";
        Exchanger.setBody("");
    }

    return (Chunk);
}

/* //////////////////////////// */

void Generator::generateLocation(Exchange& Exchanger, const std::string NewLocation)
{
    std::string Location = "Location: " + NewLocation + "\r\n";
    Exchanger.addLineToHeader(Location);
}

/* //////////////////////////// */

void Generator::generateContentType(Exchange& Exchanger)
{
    std::string line;
    std::string RequestBody = Exchanger.getHashMapString("Body");
    std::istringstream issBody(RequestBody);

    while (std::getline(issBody, line))
    {
        if (line.compare(0, 13, "Content-Type:") == 0)
        {
            Exchanger.addLineToHeader(line);
            return ;
        }
    }
}

/* //////////////////////////// */

std::string Generator::generateBoundry(Exchange& Exchanger)
{
    std::string ContentType = Exchanger.getHashMapString("Content-Type");

    std::size_t found = ContentType.find('=');
    if (found == std::string::npos)
        throw (std::logic_error("No boundry found"));

    return (ContentType.substr(found + 1, ContentType.length() - found) + "\r\n");
}
