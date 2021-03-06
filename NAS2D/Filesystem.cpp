// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Filesystem.h"
#include "Exception.h"

#include <physfs.h>

#include <climits>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <limits>


using namespace NAS2D;
using namespace NAS2D::Exception;


static bool closeFile(void* file)
{
	if (!file) { return false; }

	if (PHYSFS_close(static_cast<PHYSFS_File*>(file)) != 0) { return true; }

	throw filesystem_file_handle_still_open(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
}



enum MountPosition
{
	MOUNT_PREPEND = 0,
	MOUNT_APPEND = 1,
};


Filesystem::Filesystem(const std::string& argv_0, const std::string& appName, const std::string& organizationName) :
	mAppName(appName),
	mOrganizationName(organizationName)
{
	if (PHYSFS_isInit()) { throw filesystem_already_initialized(); }

	if (PHYSFS_init(argv_0.c_str()) == 0)
	{
		throw filesystem_backend_init_failure(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}


/**
 * Shuts down PhysFS and cleans up.
 */
Filesystem::~Filesystem()
{
	PHYSFS_deinit();
}


/**
 * Determines the path to the folder where the executable is located
 *
 * The base path may or may not be the current working directory.
 */
std::string Filesystem::basePath() const
{
	return PHYSFS_getBaseDir();
}


/**
 * Determines the path to the folder where user preferences are stored
 *
 * The user should have write access to the preferences folder, which is generally under their user folder.
 *
 * \note This path is dependent on the Operating System (OS).
 * \note Path may be empty if the folder could not be created.
 */
std::string Filesystem::prefPath() const
{
	auto prefDir = PHYSFS_getPrefDir(mOrganizationName.c_str(), mAppName.c_str());
	return (prefDir != nullptr) ? prefDir : "";
}


/**
 * Mount a folder with read access
 *
 * \param path	File path to add.
 *
 * \return Nonzero on success, zero on error.
 */
int Filesystem::mountSoftFail(const std::string& path) const
{
	return PHYSFS_mount(path.c_str(), "/", MountPosition::MOUNT_APPEND);
}


/**
 * Mount a folder with read access
 *
 * \param path	File path to add.
 */
void Filesystem::mount(const std::string& path) const
{
	if (mountSoftFail(path) == 0)
	{
		throw std::runtime_error(std::string("Couldn't add '") + path + "' to search path: " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}


/**
 * Mount a folder with read and write access
 *
 * \param path	File path to add.
 */
void Filesystem::mountReadWrite(const std::string& path) const
{
	// Mount for read access
	mount(path);

	// Mount for write access
	if (PHYSFS_setWriteDir(path.c_str()) == 0)
	{
		throw std::runtime_error(std::string("Couldn't add write folder '") + path + "': " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}


/**
 * Removes a directory or supported archive from the Search Path.
 *
 * \param path	File path to remove.
 */
void Filesystem::unmount(const std::string& path) const
{
	if (PHYSFS_unmount(path.c_str()) == 0)
	{
		throw std::runtime_error(std::string("Couldn't remove '") + path + "' from search path : " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}


/**
 * Returns a list of directories in the Search Path.
 */
std::vector<std::string> Filesystem::searchPath() const
{
	std::vector<std::string> searchPath;

	auto searchPathList = PHYSFS_getSearchPath();
	for (char **i = searchPathList; *i != nullptr; ++i)
	{
		searchPath.push_back(*i);
	}
	PHYSFS_freeList(searchPathList);

	return searchPath;
}


/**
 * Returns a list of files within a given directory.
 *
 * \param	dir	Directory to search within the searchpath.
 * \param	filter		Optional extension filter. Only use the extension without a wildcard (*) character or period (e.g., 'png' vs '*.png' or '.png').
 *
 * \note	This function will also return the names of any directories in a specified search path
 */
std::vector<std::string> Filesystem::directoryList(const std::string& dir, const std::string& filter) const
{
	char **rc = PHYSFS_enumerateFiles(dir.c_str());

	std::vector<std::string> fileList;
	if (filter.empty())
	{
		for (char **i = rc; *i != nullptr; i++)
		{
			fileList.push_back(*i);
		}
	}
	else
	{
		std::size_t filterLen = filter.size();
		for (char **i = rc; *i != nullptr; i++)
		{
			std::string tmpStr = *i;
			if (tmpStr.rfind(filter, tmpStr.length() - filterLen) != std::string::npos)
			{
				fileList.push_back(*i);
			}
		}
	}

	PHYSFS_freeList(rc);

	return fileList;
}


/**
 * Deletes a specified file.
 *
 * \param	filename	Path of the file to delete relative to the Filesystem root directory.
 */
void Filesystem::del(const std::string& filename) const
{
	if (PHYSFS_delete(filename.c_str()) == 0)
	{
		throw std::runtime_error(std::string("Unable to delete '") + filename + "':" + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}


/**
 * Opens a file.
 *
 * \param filename	Path of the file to load.
 *
 * \return Returns a File.
 */
File Filesystem::open(const std::string& filename) const
{
	PHYSFS_file* myFile = PHYSFS_openRead(filename.c_str());
	if (!myFile)
	{
		closeFile(myFile);
		throw std::runtime_error(std::string("Unable to load '") + filename + "': " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	// Ensure that the file size is greater than zero and can fit in a std::size_t
	auto fileLength = PHYSFS_fileLength(myFile);
	if (fileLength < 0 || static_cast<PHYSFS_uint64>(fileLength) > std::numeric_limits<std::size_t>::max())
	{
		closeFile(myFile);
		throw std::runtime_error(std::string("File '") + filename + "' is too large or size could not be determined");
	}

	// Create buffer large enough to hold entire file
	const auto bufferSize = static_cast<std::size_t>(fileLength);
	std::string fileBuffer;
	fileBuffer.resize(bufferSize);

	// Read file data into buffer and close file
	const auto actualReadLength = PHYSFS_readBytes(myFile, fileBuffer.data(), bufferSize);
	closeFile(myFile);

	// Ensure we read the expected length
	if (actualReadLength < fileLength)
	{
		throw std::runtime_error(std::string("Unable to load '") + filename + "': " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	return File{std::move(fileBuffer), filename};
}


/**
 * Creates a new directory within the primary search path.
 *
 * \param path	Path of the directory to create.
 */
void Filesystem::makeDirectory(const std::string& path) const
{
	if (PHYSFS_mkdir(path.c_str()) == 0)
	{
		throw std::runtime_error(std::string("Unable to create directory '" + path + "': ") + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}


/**
 * Determines if a given path is a directory rather than a file.
 *
 * \param path	Path to check.
 */
bool Filesystem::isDirectory(const std::string& path) const
{
	PHYSFS_Stat stat;
	return (PHYSFS_stat(path.c_str(), &stat) != 0) && (stat.filetype == PHYSFS_FILETYPE_DIRECTORY);
}


/**
 * Checks for the existence of a file.
 *
 * \param	filename	File path to check.
 *
 * Returns Returns \c true if the specified file exists. Otherwise, returns \c false.
 */
bool Filesystem::exists(const std::string& filename) const
{
	return PHYSFS_exists(filename.c_str()) != 0;
}


/**
 * Writes a file to disk.
 *
 * \param	file		A reference to a \c const \c File object.
 * \param	overwrite	Flag indicating if a file should be overwritten if it already exists. Default is true.
 */
void Filesystem::write(const File& file, bool overwrite) const
{
	if (!overwrite && exists(file.filename()))
	{
		throw std::runtime_error(std::string("File exists: ") + file.filename());
	}

	PHYSFS_file* myFile = PHYSFS_openWrite(file.filename().c_str());
	if (!myFile)
	{
		throw std::runtime_error(std::string("Couldn't open '") + file.filename() + "' for writing: " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	if (PHYSFS_writeBytes(myFile, file.bytes().c_str(), static_cast<PHYSFS_uint32>(file.size())) < static_cast<PHYSFS_sint64>(file.size()))
	{
		closeFile(myFile);
		throw std::runtime_error(std::string("Error occured while writing to file '") + file.filename() + "': " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	closeFile(myFile);
}


/**
 * Convenience function to get the working directory of a file.
 *
 * \param filename	A file path.
 *
 * \note	File paths should not have any trailing '/' characters.
 */
std::string Filesystem::workingPath(const std::string& filename) const
{
	if (!filename.empty())
	{
		std::string tmpStr(filename);
		std::size_t pos = tmpStr.rfind("/");
		tmpStr = tmpStr.substr(0, pos + 1);
		return tmpStr;
	}
	else
	{
		return std::string();
	}
}


/**
 * Gets the extension of a given file path.
 *
 * \param	path	Path to check for an extension.
 *
 * \return	Returns a string containing the file extension, including the dot (".").
 *			An empty string will be returned if the file has no extension.
 */
std::string Filesystem::extension(const std::string& path) const
{
	// This is a naive approach but works for most cases.
	std::size_t pos = path.find_last_of(".");

	if (pos != std::string::npos)
	{
		return path.substr(pos);
	}
	return std::string();
}

/**
 * Gets the dir separator for the current platform
 *
 * The dir separator separates the directories within a path. This is typically either "\\" for Windows, or "/" for Linux.
 *
 * \note The path separator may be more than one character
 */
std::string Filesystem::dirSeparator() const
{
	return PHYSFS_getDirSeparator();
}
