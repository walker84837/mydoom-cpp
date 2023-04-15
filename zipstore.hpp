#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <string>
#include <fstream>
#include <iterator>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <Windows.h>

#pragma pack(push, 1)
struct zip_header_t {
DWORD signature;        /* 0x04034b50 */
WORD ver_needed;
WORD flags;
WORD method;
WORD lastmod_time;
WORD lastmod_date;
DWORD crc;
DWORD compressed_size;
DWORD uncompressed_size;
WORD filename_length;
WORD extra_length;
};

struct zip_eod_t {
DWORD signature;        /* 0x06054b50 */
WORD disk_no;
WORD disk_dirst;
WORD disk_dir_entries;
WORD dir_entries;
DWORD dir_size;
DWORD dir_offs;
WORD comment_len;
};

struct zip_dir_t {
DWORD signature;    /* 0x02014b50 */
WORD made_by;
WORD ver_needed;
WORD flags;
WORD method;
WORD lastmod_time;
WORD lastmod_date;
DWORD crc;
DWORD compressed_size;
DWORD uncompressed_size;
WORD filename_length;
WORD extra_length;
WORD comment_length;
WORD disk_no;
WORD internal_attr;
DWORD external_attr;
DWORD local_offs;
};
#pragma pack(pop)

int zip_store(char* in, char* out, char* store_as) {
	// Open the input file
	std::ifstream input(in, std::ios::binary);
	if (!input) {
	std::cerr << "Error: Could not open file: " << in << std::endl;
	return 1;
	}

	// Determine input file size
	input.seekg(0, std::ios::end);
	std::size_t filesize = input.tellg();
	input.seekg(0, std::ios::beg);

	// Open the output file
	std::ofstream output(out, std::ios::binary);
	if (!output) {
		std::cerr << "Error: Could not open file: " << out << std::endl;
		return 1;
	}

	// Write local file header
	zip_header_t header = {};
	header.signature = 0x04034b50;
	header.filename_length = static_cast<unsigned short>(strlen(store_as));
	header.compressed_size = static_cast<unsigned long>(filesize);
	header.uncompressed_size = static_cast<unsigned long>(filesize);
	memcpy(out, &header, sizeof(header));
	output.write(store_as, header.filename_length);
	output.write((char*)0, header.extra_length);

	// Write file data
	std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>(), std::ostreambuf_iterator<char>(output));

	// Write end of central directory record
	zip_eod_t eod = {};
	eod.signature = 0x06054b50;
	eod.disk_no = 0;
	eod.disk_dirst = 0;
	eod.disk_dir_entries = 1;
	eod.dir_entries = 1;
	eod.dir_size = sizeof(zip_dir_t) + header.filename_length;
	eod.dir_offs = static_cast<unsigned long>(output.tellp());
	memcpy(out, &eod, sizeof(eod));
	output.write(store_as, header.filename_length);

	// Write central directory file header
	zip_dir_t dir = {};
	dir.signature = 0x02014b50;
	dir.made_by = 0;
	dir.ver_needed = 20;
	dir.flags = 0;
	dir.method = 0;
	dir.lastmod_time = 0;
	dir.lastmod_date = 0;
	dir.crc = 0;
	dir.compressed_size = static_cast<unsigned long>(filesize);
	dir.uncompressed_size = static_cast<unsigned long>(filesize);
	dir.filename_length = header.filename_length;
	dir.extra_length = 0;
	dir.comment_length = 0;
	dir.disk_no = 0;
	dir.internal_attr = 0;
	dir.external_attr = 0;
	dir.local_offs = 0;

	memcpy(out, &dir, sizeof(dir));
	output.write(store_as, header.filename_length);

	return 0;
}