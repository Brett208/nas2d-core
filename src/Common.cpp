// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "NAS2D/Common.h"

#include <algorithm>
#include <cctype>
#include <numeric>
#include <sstream>

const int NAS2D_MAJOR_VERSION = 1;
const int NAS2D_MINOR_VERSION = 4;
const int NAS2D_PATCH_VERSION = 2;

/**
 * Gets a string containing the version of NAS2D being used.
 */
std::string NAS2D::versionString()
{
	std::ostringstream ss;
	ss << versionMajor() << "." << versionMinor() << "." << versionPatch();
	return ss.str();
}

/**
 * Gets version major.
 */
int NAS2D::versionMajor()
{
	return NAS2D_MAJOR_VERSION;
}

/**
 * Gets version minor.
 */
int NAS2D::versionMinor()
{
	return NAS2D_MINOR_VERSION;
}

/**
 * Gets version patch.
 */
int NAS2D::versionPatch()
{
	return NAS2D_PATCH_VERSION;
}

/**
 * \fn isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH)
 *
 * Determines if a 2D coordinate is within a rectangular area.
 *
 * \param pointX	X-Coordinate of point to test.
 * \param pointY	Y-Coordinate of point to test.
 * \param rectX		X-Coordinate of origin point of the Rectangular area to test.
 * \param rectY		Y-Coordinate of origin point of the Rectangular area to test.
 * \param rectW		Width of the Rectangular area to test.
 * \param rectH		Height of the Rectangular area to test.
 *
 * \return Returns true if point is within rectangular area.
 */
bool NAS2D::isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH)
{
	return (pointX >= rectX && pointX <= rectX + rectW && pointY >= rectY && pointY <= rectY + rectH);
}

/**
 * \fn isPointInRect(const Point_2d& point, const Rectangle_2d& rect)
 *
 * Determines if a 2D point is within a rectangular area.
 *
 * \param point	2D point to test.
 * \param rect	Rectangular area to test point against.
 *
 * \return Returns true if point is within rectangular area.
 */
bool NAS2D::isPointInRect(const Point_2d& point, const Rectangle_2d& rect)
{
	return (point.x() >= rect.x() && point.x() <= rect.x() + rect.width() && point.y() >= rect.y() && point.y() <= rect.y() + rect.height());
}

/**
 * \fn isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2)
 *
 * Determines if two rectangles, A and B, intersect.
 *
 * \param aX	X-Coordinate of origin point of rectangle A.
 * \param aY	Y-Coordinate of origin point of rectangle A.
 * \param aX2	X-Coordinate of end point of rectangle A.
 * \param aY2	Y-Coordinate of end point of rectangle A.
 * \param bX	X-Coordinate of origin point of rectangle B.
 * \param bY	Y-Coordinate of origin point of rectangle B.
 * \param bX2	X-Coordinate of end point of rectangle B.
 * \param bY2	Y-Coordinate of end point of rectangle B.
 *
 * \return Returns true if rectangles intersect.
 */
bool NAS2D::isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2)
{
	return (aX <= bX2 && aX2 >= bX && aY <= bY2 && aY2 >= bY);
}


/**
 * \fn isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b)
 *
 * Determines if two rectangles, A and B, intersect.
 *
 * \param a	Rectangle to test.
 * \param b	Rectangle to test against.
 *
 * \return Returns true if rectangles intersect.
 */
bool NAS2D::isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b)
{
	return (a.x() <= (b.x() + b.width()) && (a.x() + a.width()) >= b.x() && a.y() <= (b.y() + b.height()) && (a.y() + a.height()) >= b.y());
}

/**
 * \fn toLowercase(const std::string& str)
 *
 * Converts a string to lowercase.
 *
 * \param str	Source string.
 *
 * \return	Returns the converted string.
 */
std::string NAS2D::toLowercase(std::string str)
{
	std::transform(std::begin(str), std::end(str), std::begin(str), [](unsigned char c) noexcept->unsigned char { return static_cast<unsigned char>(::tolower(c)); });
	return str;
}

/**
 * \fn toUppercase(const std::string& str)
 *
 * Converts a string to uppercase.
 *
 * \param str	Source string.
 *
 * \return	Returns the converted string.
 */
std::string NAS2D::toUppercase(std::string str)
{
	std::transform(std::begin(str), std::end(str), std::begin(str), [](unsigned char c) noexcept->unsigned char { return static_cast<unsigned char>(::toupper(c)); });
	return str;
}

std::vector<std::string> NAS2D::split(std::string str, char delim /*= ','*/)
{
	const auto potential_count = 1 + std::count(std::begin(str), std::end(str), delim);
	NAS2D::StringList result{};
	result.reserve(potential_count);

	std::istringstream ss(str);

	std::string curString{};
	while (std::getline(ss, curString, delim))
	{
		result.push_back(curString);
	}
    if(ss.eof() && str.back() == delim) {
		result.push_back(std::string{});
    }
	result.shrink_to_fit();
	return result;
}
std::vector<std::string> NAS2D::splitSkipEmpty(std::string str, char delim /*= ','*/)
{
	const auto potential_count = 1 + std::count(std::begin(str), std::end(str), delim);
	NAS2D::StringList result{};
	result.reserve(potential_count);

	std::istringstream ss(str);

	std::string curString{};
	while (std::getline(ss, curString, delim))
	{
		if (curString.empty()) { continue; }
		result.push_back(curString);
	}
	result.shrink_to_fit();
	return result;
}

std::pair<std::string, std::string> NAS2D::splitOnFirst(const std::string& str, char delim)
{
	const auto delim_loc = str.find_first_of(delim);
	if (delim_loc == std::string::npos)
	{
		return std::make_pair(str, std::string{});
	}
	else
	{
		return std::make_pair(str.substr(0, delim_loc), str.substr(delim_loc + 1));
	}
}

std::pair<std::string, std::string> NAS2D::splitOnLast(const std::string& str, char delim)
{
	const auto delim_loc = str.find_last_of(delim);
	if (delim_loc == std::string::npos)
	{
		return std::make_pair(std::string{}, str);
	}
	else
	{
		return std::make_pair(str.substr(0, delim_loc), str.substr(delim_loc + 1));
	}
}

std::string NAS2D::join(std::vector<std::string> strs)
{
	const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + b.size(); };
	auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
	std::string result;
	result.reserve(total_size);
	for (const auto& s : strs)
	{
		result += s;
	}
	result.shrink_to_fit();
	return result;
}

std::string NAS2D::join(std::vector<std::string> strs, char delim)
{
	const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + std::size_t{1u} + b.size(); };
	auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
	std::string result;
	result.reserve(total_size);

	for (auto iter = std::begin(strs); iter != std::end(strs); ++iter)
	{
		result += (*iter);
		if (iter + 1 != std::end(strs))
		{
			result.push_back(delim);
		}
	}

	result.shrink_to_fit();
	return result;
}
std::string NAS2D::joinSkipEmpty(std::vector<std::string> strs)
{
	const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + b.size(); };
	auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
	std::string result;
	result.reserve(total_size);
	for (const auto& s : strs)
	{
		if (s.empty()) { continue; }
		result += s;
	}
	result.shrink_to_fit();
	return result;
}

std::string NAS2D::joinSkipEmpty(std::vector<std::string> strs, char delim)
{
	const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + std::size_t{1u} + b.size(); };
	auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
	std::string result;
	result.reserve(total_size);

	for (auto iter = std::begin(strs); iter != std::end(strs); ++iter)
	{
		if ((*iter).empty()) { continue; }
		result += (*iter);
		if (iter + 1 != std::end(strs))
		{
			result.push_back(delim);
		}
	}

	result.shrink_to_fit();
	return result;
}

std::string NAS2D::trimWhitespace(std::string string)
{
	const auto first_non_space = string.find_first_not_of(" \r\n\t\v\f");
	if (first_non_space == std::string::npos)
	{
		return std::string{};
	}
	const auto last_non_space = string.find_last_not_of(" \r\n\t\v\f");
	return string.substr(first_non_space, last_non_space - first_non_space + 1);
}

bool NAS2D::startsWith(std::string_view string, std::string_view start) noexcept
{
	return string.compare(0, start.size(), start) == 0;
}

bool NAS2D::endsWith(std::string_view string, std::string_view end) noexcept
{
	return string.compare(string.size() - end.size(), end.size(), end) == 0;
}

bool NAS2D::startsWith(std::string_view string, char start) noexcept
{
	return !string.empty() && string.front() == start;
}

bool NAS2D::endsWith(std::string_view string, char end) noexcept
{
	return !string.empty() && string.back() == end;
}

/**
 * \fn int divideUp(int a, int b)
 *
 * Basic integer division that rounds up to the nearest whole number.
 *
 * \param	to_divide	Number to be divided.
 * \param	divisor		Divisor.
 *
 * \return	Returns the divided number rounded up to the nearest whole number.
 */
int NAS2D::divideUp(int to_divide, int divisor)
{
	return (to_divide + (divisor - 1)) / divisor;
}
