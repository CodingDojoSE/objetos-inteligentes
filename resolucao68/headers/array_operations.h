/*********************** Licensing *******************************************************
*
*  Copyright 2016 @ Lucas Roman
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 3 of the License, or ( at
*  your option ) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************************
*/



/**
 * Preprocessor directive designed to cause the current source file to be included only once in a
 * single compilation. Thus, serves the same purpose as #include guards, but with several
 * advantages, including: less code, avoidance of name clashes, and sometimes improvement in
 * compilation speed. In main file this is enabled by default.
 */
#pragma once



/**
 * Change this typedef to change the type of the element for the bit vector.
 * Rule is for it to be small enough to waste less memory;
 */
typedef unsigned char bitarr_el;

/**
 * Abstraction provided to access a bitarray. Caution: do not access it with []
 */
typedef bitarr_el* bitarr_t;

/**
 * Works like a C++ constructor.
 * @param arrSize		The size of the array
 * @return				A pointer to the array.
 */
bitarr_t createBitArray(size_t arrSize) {
	bitarr_t ar;
	ar = new bitarr_el[arrSize/sizeof(bitarr_el) + 1];
	return ar;
}

/**
 * Frees memory from bit array;
 * @param arr			Memory to be freed
 */
void destroyBitArray(bitarr_t arr) {
	delete[] arr;
}

/**
 * Checks to see if the bit at the given position is set
 * @param arr			The bit array
 * @param pos			The position
 * @return				true if bit is 1 else false
 */
bool checkPosition(bitarr_t arr, size_t pos) {
	//Calculate region
	size_t region = pos/sizeof(bitarr_el);
	//Get element at region
	bitarr_el* element = arr + region;
	//Calculate index inside element
	size_t elementPos = pos % sizeof(bitarr_el);
	//Check bit of element
	return (*element) & (1 << (elementPos));
}

/**
 * Sets bit on given position of the array
 * @param arr			The bit array
 * @param pos			The position in the array
 * @param value			If value is true, sets bit to 1 else 0.
 */
void setPosition(bitarr_t arr, size_t pos, bool value) {
	//Calculate region
	size_t region = pos/sizeof(bitarr_el);
	//Get element at region
	bitarr_el* element = arr + region;
	//Calculate index inside element
	size_t elementPos = pos % sizeof(bitarr_el);
	if(value) {
		*element |= (1 << elementPos);
	}
	else {
		*element &= !(1 << elementPos);
	}
}


