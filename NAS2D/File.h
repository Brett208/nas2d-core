// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include <string>
#include <utility>


namespace NAS2D {

/**
 * \class File
 * \brief Represent a File object.
 *
 * The File object represents a file as a stream of bytes.
 */
class File
{
public:

	using byte = char; /**< Byte. */
	using const_byte = const char; /**< Const byte. */
	using RawByteStream = const_byte*; /**< Pointer to a const_byte. */

	using iterator = std::string::iterator; /**< Forward iterator for a File byte stream. */
	using reverse_iterator = std::string::reverse_iterator; /**< Reverse iterator for a File byte stream. */
	using ByteStream = std::string; /**< Byte stream. */


	File()
	{}

	/**
	 * \param	stream	A ByteStream representing the file.
	 * \param	name	The full name of the file including path.
	 */
	File(ByteStream stream, std::string name) :
		mByteStream(std::move(stream)),
		mFileName(std::move(name))
	{}

	~File()
	{}


	File(const File& _f) :
		mByteStream(_f.mByteStream),
		mFileName(_f.mFileName)
	{}


	/**
	 * Copy operator.
	 */
	File& operator=(const File& _f)
	{
		mByteStream = _f.mByteStream;
		mFileName = _f.mFileName;
		return *this;
	}

	/**
	 * Gets a reference to the internal ByteStream.
	 *
	 * \note	This gets a \c non-const reference to the internal \c ByteStream
	 *			so that modifications can be made as necessary.
	 */
	ByteStream& bytes() { return mByteStream; }


	/**
	 * Gets a reference to the internal ByteStream.
	 *
	 * \note	This gets a \c const reference to the internal \c ByteStream.
	 */
	const ByteStream& bytes() const { return mByteStream; }


	/**
	 * Gets a raw pointer to a \c const \c byte stream.
	 *
	 * \note	This function is provided as a convenience for
	 *			use with low-level and legacy C libraries that
	 *			need a const char* byte stream.
	 */
	RawByteStream raw_bytes() const { return mByteStream.c_str(); }


	/**
	 * Gets the size, in bytes, of the File.
	 */
	std::size_t size() const { return mByteStream.size(); }


	/**
	 * Gets the size, in bytes, of the File.
	 */
	std::size_t length() const { return size(); }

	/**
	 * Resizes the File.
	 *
	 * \param	size	Number of bytes to resize the File to.
	 *
	 * \warning		Providing a size parameter smaller than the current File
	 *				will truncate the existing data. There is no way to
	 *				recover the data once the File is resized.
	 */
	void resize(std::size_t size) { mByteStream.resize(size); }


	/**
	 * Resizes the File.
	 *
	 * \param	size	Number of bytes to resize the File to.
	 * \param	b		Byte value to use to fill any additional bytes gained after resizing.
	 *
	 * \warning		Providing a size parameter smaller than the current File
	 *				will truncate the existing data. There is no way to
	 *				recover the data once the File is resized.
	 */
	void resize(std::size_t size, byte b) { mByteStream.resize(size, b); }

	/**
	 * Indicates that the File is empty.
	 */
	bool empty() const { return mByteStream.empty(); }

	/**
	 * Gets an iterator to the beginning of the File's byte stream.
	 */
	iterator begin() { return mByteStream.begin(); }

	/**
	 * Gets an iterator to the end of the File's byte stream.
	 */
	iterator end() { return mByteStream.end(); }

	/**
	 * Gets a reverse iterator to the beginning of the File's byte stream.
	 */
	reverse_iterator rbegin() { return mByteStream.rbegin(); }

	/**
	 * Gets a reverse iterator to the end of the File's byte stream.
	 */
	reverse_iterator rend() { return mByteStream.rend(); }

	/**
	 * Gets an iterator to the byte at a specified position.
	 *
	 * \param pos	Position of the iterator to get.
	 */
	iterator seek(std::ptrdiff_t pos) { iterator it = mByteStream.begin() + pos; return it; }

	/**
	 * Gets a reverse iterator to the byte at a specified position.
	 *
	 * \param pos	Position of the iterator to get.
	 *
	 * \see seek
	 */
	reverse_iterator rseek(std::ptrdiff_t pos) { reverse_iterator it = mByteStream.rbegin() + pos; return it; }

	/**
	 * Gets a byte from the byte stream at a specified position.
	 *
	 * \param pos Position of the byte to get.
	 *
	 * \warning	Out of range positions yield undefined behavior. Some compilers will
	 *			throw an \c out_of_range exception.
	 */
	byte& operator[](std::size_t pos) { return mByteStream[pos]; }

	/**
	 * Gets a const byte from the byte stream at a specified position.
	 *
	 * \param pos Position of the byte to get.
	 *
	 * \warning	Out of range positions yield undefined behavior. Some compilers will
	 *			throw an \c out_of_range exception.
	 */
	const_byte& operator[](std::size_t pos) const { return mByteStream[pos]; }

	/**
	 * Clears the File and leaves it completely empty.
	 */
	void clear() { mByteStream = ""; mFileName = ""; }


	/**
	 * Gets the file name.
	 *
	 * \note	Filenames include both the individual file's
	 *			name and full directory path.
	 */
	std::string filename() const { return mFileName; }

private:
	ByteStream mByteStream; /**< Internal stream of bytes. */
	std::string mFileName; /**< Internal filename including directory path. */
};

} // namespace
