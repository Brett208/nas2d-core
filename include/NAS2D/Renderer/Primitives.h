// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include <cstdint>

namespace NAS2D {

/**
 * \class	Color_4ub
 * \brief	RGBA Color.
 */
class Color_4ub
{
public:
	Color_4ub() = default;
	Color_4ub(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

public:
	void operator()(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	uint8_t red() const;
	uint8_t green() const;
	uint8_t blue() const;
	uint8_t alpha() const;

	void red(uint8_t red);
	void green(uint8_t green);
	void blue(uint8_t blue);
	void alpha(uint8_t alpha);

private:
	unsigned char mR = 255, mG = 255, mB = 255, mA = 255;
};


class Rectangle_2d;
class Rectangle_2df;


/**
 * \class	Rectangle_2d
 * \brief	2D rectangle.
 */
class Rectangle_2d
{
public:
	Rectangle_2d() = default;
	Rectangle_2d(int x, int y, int w, int h);
	Rectangle_2d(const Rectangle_2df& rect);

public:
	void operator()(int x, int y, int w, int h);

	bool operator==(const Rectangle_2d& rect);
	bool operator==(const Rectangle_2df& rect);

	bool operator!=(const Rectangle_2d& rect);
	bool operator!=(const Rectangle_2df& rect);

	Rectangle_2d& operator+=(const Rectangle_2d& rect);
	Rectangle_2d& operator+=(const Rectangle_2df& rect);

	Rectangle_2d& operator-=(const Rectangle_2d& rect);
	Rectangle_2d& operator-=(const Rectangle_2df& rect);

	Rectangle_2d& operator*=(const Rectangle_2d& rect);
	Rectangle_2d& operator*=(const Rectangle_2df& rect);

	const Rectangle_2d operator+(const Rectangle_2d& rect);
	const Rectangle_2d operator+(const Rectangle_2df& rect);

	const Rectangle_2d operator-(const Rectangle_2d& rect);
	const Rectangle_2d operator-(const Rectangle_2df& rect);

	const Rectangle_2d operator*(const Rectangle_2d& rect);
	const Rectangle_2d operator*(const Rectangle_2df& rect);

public:
	bool null();

	void x(int x);
	int x() const;
	int& x();

	void y(int y);
	int y() const;
	int& y();

	void width(int w);
	int width() const;
	int& width();

	void height(int h);
	int height() const;
	int& height();

	int center_x() const;
	int center_y() const;

private:
	int mX = 0, mY = 0, mW = 0, mH = 0;
};


/**
 * \class	Rectangle_2df
 * \brief	Floating point 2D Rectangle.
 */
class Rectangle_2df
{
public:
	Rectangle_2df() = default;
	Rectangle_2df(float x, float y, float w, float h);

public:
	void operator()(float x, float y, float w, float h);

	bool operator==(const Rectangle_2d& rect);
	bool operator==(const Rectangle_2df& rect);

	bool operator!=(const Rectangle_2d& rect);
	bool operator!=(const Rectangle_2df& rect);

	Rectangle_2df& operator+=(const Rectangle_2d& rect);
	Rectangle_2df& operator+=(const Rectangle_2df& rect);

	Rectangle_2df& operator-=(const Rectangle_2d& rect);
	Rectangle_2df& operator-=(const Rectangle_2df& rect);

	Rectangle_2df& operator*=(const Rectangle_2d& rect);
	Rectangle_2df& operator*=(const Rectangle_2df& rect);

	const Rectangle_2df operator+(const Rectangle_2d& rect);
	const Rectangle_2df operator+(const Rectangle_2df& rect);

	const Rectangle_2df operator-(const Rectangle_2d& rect);
	const Rectangle_2df operator-(const Rectangle_2df& rect);

	const Rectangle_2df operator*(const Rectangle_2d& rect);
	const Rectangle_2df operator*(const Rectangle_2df& rect);

public:
	bool null();

	void x(float x);
	float x() const;
	float& x();

	void y(float y);
	float y() const;
	float& y();

	void width(float w);
	float width() const;
	float& width();

	void height(float h);
	float height() const;
	float& height();

	float center_x() const;
	float center_y() const;

private:
	float mX = 0.0f, mY = 0.0f, mW = 0.0f, mH = 0.0f;
};


class Point_2d;
class Point_2df;


/**
 * \class	Point_2d
 * \brief	2D point.
 */
class Point_2d
{
public:
	Point_2d() = default;
	Point_2d(int x, int y);
	Point_2d(const Point_2df& _p);

public:
	void operator()(int x, int y);

	bool operator==(const Point_2d& pt);
	bool operator==(const Point_2df& pt);

	bool operator!=(const Point_2d& pt);
	bool operator!=(const Point_2df& pt);

	Point_2d& operator+=(const Point_2d& pt);
	Point_2d& operator+=(const Point_2df& pt);

	Point_2d& operator-=(const Point_2d& pt);
	Point_2d& operator-=(const Point_2df& pt);

	Point_2d& operator*=(const Point_2d& pt);
	Point_2d& operator*=(const Point_2df& pt);

	const Point_2d operator+(const Point_2d& pt);
	const Point_2d operator+(const Point_2df& pt);

	const Point_2d operator-(const Point_2d& pt);
	const Point_2d operator-(const Point_2df& pt);

	const Point_2d operator*(const Point_2d& pt);
	const Point_2d operator*(const Point_2df& pt);

public:
	void x(int x);
	int x() const;
	int& x();

	void y(int y);
	int y() const;
	int& y();

private:
	int mX = 0, mY = 0;
};


/**
 * \class	Point_2df
 * \brief	Floating point 2D Point.
 */
class Point_2df
{
public:
	Point_2df() = default;
	Point_2df(float x, float y);
	Point_2df(const Point_2d& _p);

public:
	void operator()(float _x, float _y);

	bool operator==(const Point_2d& pt);
	bool operator==(const Point_2df& pt);

	bool operator!=(const Point_2d& pt);
	bool operator!=(const Point_2df& pt);

	Point_2df& operator+=(const Point_2d& pt);
	Point_2df& operator+=(const Point_2df& pt);

	Point_2df& operator-=(const Point_2d& pt);
	Point_2df& operator-=(const Point_2df& pt);

	Point_2df& operator*=(const Point_2d& pt);
	Point_2df& operator*=(const Point_2df& pt);

	const Point_2df operator+(const Point_2d& pt);
	const Point_2df operator+(const Point_2df& pt);

	const Point_2df operator-(const Point_2d& pt);
	const Point_2df operator-(const Point_2df& pt);

	const Point_2df operator*(const Point_2d& pt);
	const Point_2df operator*(const Point_2df& pt);

public:
	void x(float x);
	float x() const;
	float& x();

	void y(float y);
	float y() const;
	float& y();

private:
	float mX = 0.0f, mY = 0.0f;
};

} // namespace
