// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "NAS2D/MathUtils.h"
#include <stdexcept>


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
	if (divisor == 0) {
		throw std::domain_error("Division by zero: divideUp(to_divide, 0)");
	}
	return (to_divide + (divisor - 1)) / divisor;
}

/**
 * Rounds a number up to a power of 2
 *
 * Domain: 1 .. 2^31
 * Values outside the domain may map to 0 (which is not a power of 2)
 * Note: 0 is outside the domain
 */
uint32_t NAS2D::roundUpPowerOf2(uint32_t number)
{
	--number;
	number |= number >> 1;
	number |= number >> 2;
	number |= number >> 4;
	number |= number >> 8;
	number |= number >> 16;
	return ++number;
}
