//
// Created by Xander Voorvaart on 10/11/22.
//

#include <iostream>
#include <fstream>

int64_t getLength(std::ifstream& File)
{
	int64_t len;

	File.seekg(0, File.end);
	len = File.tellg();
	File.seekg(0, File.beg);
	return (len);
}

/*
 * /data/www/ does not work...
 * data/www/ does... lol
 *
 * Please protect readFile with Try-Catch.
*/
std::string readFile(const std::string& RespondedFile)
{
	int64_t len;
	std::ifstream File;
	std::string FileContentStr;

	File.open(RespondedFile);
	if (!File.is_open())
		throw (std::invalid_argument("File could not be opened."));

	len = getLength(File);
	char *FileContent = new char [len];
	try
	{
		File.read(FileContent, len);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
	File.close();
	FileContentStr.append(FileContent, len);
	delete [] FileContent;
	return (FileContentStr);
}
