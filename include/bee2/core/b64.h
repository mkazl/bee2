/*
*******************************************************************************
\file b64.h
\brief The Base64 encoding
\project bee2 [cryptographic library]
\created 2016.06.16
\version 2021.04.21
\license This program is released under the GNU General Public License 
version 3. See Copyright Notices in bee2/info.h.
*******************************************************************************
*/

/*!
*******************************************************************************
\file b64.h
\brief Кодирование base64
*******************************************************************************
*/

#ifndef __BEE2_B64_H
#define __BEE2_B64_H

#include "bee2/defs.h"
#include "bee2/core/safe.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
*******************************************************************************
\file b64.h

Реализовано кодирование буферов памяти base64-строками по правилам RFC 4848.
Base64-строка -- это слово в алфавите 
	b64_alphabet = {'A',...,'F','a',...,'f','0',...,'9','+','/'},
которое может дополняться одним или двумя символами '=' и длина которого 
всегда кратна 4.

Правила кодирования:
1 Кодируемые данные разбиваются на блоки --- тройки октетов или 24-ки битов. 
  Каждая 24-ка разбивается на четыре 6-ки. Каждая 6-ка кодируется 1 символом 
  b64_alphabet: 000000 --- символом 'A',...., 111111 -- символом '/'.
2 Если последний блок данных является неполным, то  он дополняется до полного 
  8 или 16 нулевыми битами, а затем кодируется 3 или 2 символами b64_alphabet.
3 Более точно, если блок дополняется 8 нулевыми битами, то кодируются первые 
  18 = 6 * 3 битов блока (16 значащих битов и 2 дописанных нуля). 
  Если же блок дополняется 16  нулевыми битами, то кодируются первые 
  12 = 6 * 2 битов (8 значащих и 4 нуля).
4 Окончательное кодовое слово дополняется символами '=' до тех пор, пока его 
  длина не станет кратной 4. Если последний блок данных полный, то символ '=' 
  не дописывается. Если последний блок состоял из 2 октетов, то будет 
  дописан 1 символ '=', если из 1 октета -- 2 символа. 

\pre Во все функции, кроме возможно b64IsValid(), передаются корректные строки 
и буферы памяти.
*******************************************************************************
*/

/*!	\brief Корректная base64-строка?

	Проверяется корректность base64-строки b64. Строка считается корректной, 
	если:
	- ее длина кратна 4;
	- строка может заканчиваться 1 или 2 символами '=', все остальные символы
	  принадлежат алфавиту enc_table;
	- если последний блок строки имеет вид abc=, то в 6-кe битов, 
	  соответствующих символу c, два младших  бита нулевые;
	- если последний блок строки имеет вид ab==, то в 6-кe битов, 
	  соответствующих символу b, четыре младших бита нулевые.
	\return Признак корректности.
	\safe Функция нерегулярна.
*/
bool_t b64IsValid(
	const char* b64		/*!< [in] base64-строка */
);

/*!	\brief Кодирование буфера памяти

	Буфер [count]src кодируется base64-строкой [4 * ((count + 2) / 3) + 1]dest.
	\pre Буферы dest и src не пересекаются.
*/
void b64From(
	char* dest,			/*!< [out] строка-приемник */
	const void* src,	/*!< [in] память-источник */
	size_t count		/*!< [in] число октетов */
);

/*!	\brief Декодирование буфера памяти

	Base64-строка src декодируется в строку октетов [count?]dest.
	\pre Если dest != 0, то буфер [count]dest корректен и его размер
	достаточен для размещения декодированных данных.
	\pre Буферы dest и src не пересекаются.
	\pre b64IsValid(src) == TRUE. 
	\remark Декодированные данные всегда уместятся  
	в буфер из 3 * strLen(src) / 4 октетов.
*/
void b64To(
	void* dest,			/*!< [out] память-приемник */
	size_t* count,		/*!< [in/out] размер dest / декодированных данных */
	const char* src		/*!< [in] строка-источник */
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BEE2_B64_H */
