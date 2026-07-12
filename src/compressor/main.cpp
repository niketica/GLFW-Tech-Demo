#include <iostream>
#include "PackWriter.h"
#include "PackReader.h"
#include "PathUtil.h"

static const char* PASSPHRASE = "SuperSecretPassphrase";

void printUsage(void)
{
	printf("\n");
	printf("Adds or extracts files from a compressed archive.\n");
	printf("Usage: MyCompressor.exe <archive> <[+|-]> <filename>\n");
	printf("\n");
}

void operationAdd(const char* archiveName, const char* fullyQualifiedFileName)
{
	printf("Add folder %s to archive %s\n", fullyQualifiedFileName, archiveName);
	niketica::compressor::PackWriter packWriter(PASSPHRASE);
	packWriter.packFolder(fullyQualifiedFileName, archiveName);
}

void operationExtract(const char* archiveName, const char* fullyQualifiedFileName)
{
	printf("Extract folder %s from archive %s\n", fullyQualifiedFileName, archiveName);
	niketica::compressor::PakReader pak(PASSPHRASE);
	pak.load(archiveName);
	auto file_data = pak.getFile(fullyQualifiedFileName);

	std::string textFile(file_data.begin(), file_data.end());
	printf("Text: %s\n", textFile.c_str());

}

int main(int argc, char* argv[])
{
	char* archiveName = nullptr;
	char* strOperation = nullptr;
	char* fullyQualifiedFileName = nullptr;

	if (argc != 4)
	{
		printUsage();
		return 0;
	}

	archiveName = argv[1];
	strOperation = argv[2];

    std::string inputPath(argv[3]);
    std::string normalizedPath = niketica::compressor::normalizePath(inputPath);
    fullyQualifiedFileName = const_cast<char*>(normalizedPath.c_str());

	if (_stricmp(strOperation, "+") == 0)
	{
		operationAdd(archiveName, fullyQualifiedFileName);
	}
	else if (_stricmp(strOperation, "-") == 0)
	{
		operationExtract(archiveName, fullyQualifiedFileName);
	}
	else
	{
		printUsage();
		return 0;
	}

    return 0;
}
