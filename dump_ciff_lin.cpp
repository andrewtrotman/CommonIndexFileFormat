/*
	DUMP_CIFF_LIN.CPP
	-----------------
	Copyright (c) 2019 Andrew Trotman
	Released under the 2-clause BSD license (See:https://en.wikipedia.org/wiki/BSD_licenses)
*/
/*!
	@file
	@author Andrew Trotman
	@copyright 2019 Andrew Trotman

	@brief Read and dump an index in Jimmy Lin's Common Index File Format.
*/
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>

#include "ciff_lin.h"

/*
		FILE::READ_ENTIRE_FILE()
		------------------------
		This uses a combination of "C" FILE I/O and C++ strings in order to copy the contents of a file into an internal buffer.
		There are many different ways to do this, but this is the fastest according to this link: http://insanecoding.blogspot.co.nz/2011/11/how-to-read-in-file-in-c.html
		Note that there does not appear to be a way in C++ to avoid the initialisation of the string buffer.

		Returns the length of the file in bytes - which is also the size of the string buffer once read.
	*/
		size_t read_entire_file(const std::string &filename, std::string &into)
		{
		FILE *fp;							// "C" pointer to the file
		struct stat details;				// file system's details of the file
		size_t file_length = 0;			// length of the file in bytes

		/*
			Fopen() the file then fstat() it.  The alternative is to stat() then fopen() - but that is wrong because the file might change between the two calls.
		*/
		if ((fp = fopen(filename.c_str(), "rb")) != nullptr)
			{
			if (fstat(fileno(fp), &details) == 0)
				if ((file_length = details.st_size) != 0)
					{
					into.resize(file_length);
					if (fread(&into[0], details.st_size, 1, fp) != 1)
						into.resize(0);
					}
			fclose(fp);
			}

		return file_length;
		}
/*
	MAIN()
	------
*/
int main(int argc, const char *argv[])
	{
	std::string file;
	size_t file_size = read_entire_file(argv[1], file);
	JASS::ciff_lin source((uint8_t *)&file[0], file_size);

	for (const auto &posting : source)
		{
		std::cout.write((char *)posting.term.start, posting.term.length);
		std::cout << " " << posting.document_frequency << " " << posting.collection_frequency << "\n";
		}

	return 0;
	}
