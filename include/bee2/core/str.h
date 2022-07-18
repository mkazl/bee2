/*
*******************************************************************************
\file str.h
\brief Strings
\project bee2 [cryptographic library]
\created 2013.02.04
\version 2022.07.14
\license This program is released under the GNU General Public License 
version 3. See Copyright Notices in bee2/info.h.
*******************************************************************************
*/

/*!
*******************************************************************************
\file str.h
\brief Строки
*******************************************************************************
*/

#ifndef __BEE2_STR_H
#define __BEE2_STR_H

#include <string.h>
#include "bee2/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
*******************************************************************************
\file str.h

Реализованы или переопределены манипуляции над строками. Строка
представляет собой последовательность символов-октетов, которая
заканчивается нулевым октетом.

\pre Во все функции, кроме strIsValid(), передаются корректные строки.
*******************************************************************************
*/

/*!	\brief Длина строки

	Определяется длина строки str -- число символов до завершающего нулевого.
	\return Длина строки
*/
size_t strLen(
  const char* str		/*< [in] строка */
);

/*!	\brief Ограниченная длина строки

	Определяется длина строки str в пределах окна из count первых символов.
	\return MIN2(strLen(str), count).
*/
size_t strLen2(
  const char* str,		/*< [in] строка */
  size_t count			/*< [in] длина префикса */
);

/*!	\brief Корректная строка?

	Проверяется, что строка str корректна.
	\return Признак успеха.
*/
bool_t strIsValid(
	const char* str		/*!< [in] строка */
);

/*!	\brief Копирование строки

	Строка src копируется в dest.
	\pre По адресу dest зарезервировано strLen(src) + 1 октетов.
	\pre Буферы src и dest не пересекаются.
*/
void strCopy(
	char* dest,			/*!< [out] строка-назначение */
	const char* src		/*!< [in] строка-источник */
);

/*!	\brief Сравнение строк
	
	Строки str1 и str2 сравниваются лексикографически.
	\return 1, если str1 > str2, или -1, если str1 < str2,
	или 0, если str1 == str2.
	\safe Функция нерегулярна.
*/
int strCmp(
	const char* str1,	/*!< [in] первая строка */
	const char* str2	/*!< [in] вторая строка */
);

/*!	Проверяется совпадение строк str1 и str2.
	\return Признак совпадения.
	\safe Функция нерегулярна.
*/
#define strEq(str1, str2) (strCmp(str1, str2) == 0)

/*!	\brief Заполнение символом

	Строка str заполняется символом ch.
*/
void strSet(
	char* str,		/*!< [in] строка */
	char ch			/*!< [in] символ */
);

/*
*******************************************************************************
Дополнительные функции
*******************************************************************************
*/

/*!	\brief Цифовая?

	Проверяется, что строка str состоит только из символов-цифр '0'-'9'.
	\return Признак успеха.
	\safe Функция нерегулярна.
*/
bool_t strIsNumeric(
	const char* str		/*!< [in] строка */
);

/*!	\brief Буквенно-цифовая?

	Проверяется, что строка str состоит только из символов-цифр '0'-'9'
	и символов букв 'A'-'Z', 'a'-'z'.
	\return Признак успеха.
	\safe Функция нерегулярна.
*/
bool_t strIsAlphanumeric(
	const char* str		/*!< [in] строка */
);

/*!	\brief Из печатаемых символов?

	Проверяется, что каждый символ строки str принадлежит алфавиту, составленному
	из латинских букв, цифр и символов строки " '()+,-./:=?".
	\return Признак успеха.
	\safe Функция нерегулярна.
	\remark Целевой алфавит -- это алфавит PrintableString АСН.1.
*/
bool_t strIsPrintable(
	const char* str		/*!< [in] строка */
);

/*!	\brief Начинается?

	Проверяется, что строка str начинается с префикca prefix.
	\return Признак успеха.
	\safe Функция нерегулярна.
*/
bool_t strStartsWith(
	const char* str,	/*!< [in] строка */
	const char* prefix	/*!< [in] префикс */
);

/*!	\brief Заканчивается?

	Проверяется, что строка str заканчивается суффиксом suffix.
	\return Признак успеха.
	\safe Функция нерегулярна.
*/
bool_t strEndsWith(
	const char* str,	/*!< [in] строка */
	const char* suffix	/*!< [in] суффикс */
);

/*!	\brief Разворот строки

	Символы строки str переписываются в обратном порядке.
*/
void strRev(
	char* str		/*!< [in] строка */
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BEE2_STR_H */
