/*
*******************************************************************************
\file u32.h
\brief 32-bit words
\project bee2 [cryptographic library]
\created 2015.10.28
\version 2019.07.08
\license This program is released under the GNU General Public License 
version 3. See Copyright Notices in bee2/info.h.
*******************************************************************************
*/

/*!
*******************************************************************************
\file u32.h
\brief 32-разрядные слова
*******************************************************************************
*/

#ifndef __BEE2_U32_H
#define __BEE2_U32_H

#include "bee2/defs.h"
#include "bee2/core/safe.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
*******************************************************************************
\file u32.h

Реализованы операции над 32-разрядными словами и массивами таких слов.

\pre В функции передаются корректные буферы памяти.
*******************************************************************************
*/

#define U32_0 ((u32)0)
#define U32_1 ((u32)1)
#define U32_MAX ((u32)(U32_0 - U32_1))

/*!	\def u32RotHi
	\brief Циклический сдвиг u32-слова на d позиций в сторону старших разрядов
	\pre 0 < d < 32.
*/
#define u32RotHi(w, d)\
	((u32)((w) << (d) | (w) >> (32 - (d))))

/*!	\def u32RotLo
	\brief Циклический сдвиг u32-слова на d позиций в сторону младших разрядов
	\pre 0 < d < 32.
*/
#define u32RotLo(w, d)\
	((u32)((w) >> (d) | (w) << (32 - (d))))

/*!	\brief Реверс октетов

	Выполняется реверс октетов u32-слова w.
	\return Слово с переставленными октетами.
*/
u32 u32Rev(
	register u32 w		/*!< [in] слово */
);

/*!	\brief Реверс октетов массива слов

	Выполняется реверс октетов массива [count]buf из u32-слов.
*/
void u32Rev2(
	u32 buf[],			/*!< [in/out] массив слов */
	size_t count		/*!< [in] число элементов */
);
	
/*!	\brief Вес

	Определяется число ненулевых битов в u32-слове w.
	\return Число ненулевых битов.
*/
size_t u32Weight(
	register u32 w		/*!< [in] слово */
);

/*!	\brief Четность

	Определяется сумма по модулю 2 битов u32-слова w.
	\return Сумма битов.
*/
bool_t u32Parity(
	register u32 w		/*!< [in] слово */
);

/*!	\brief Число младших нулевых битов

	Определяется длина серии из нулевых младших битов u32-слова w.
	\return Длина серии.
	\remark CTZ == Count of Trailing Zeros
	\safe Имеется ускоренная нерегулярная редакция.
	*/
size_t u32CTZ(
	register u32 w		/*!< [in] слово */
);

size_t SAFE(u32CTZ)(register u32 w);
size_t FAST(u32CTZ)(register u32 w);

/*!	\brief Число старших нулевых битов

	Определяется длина серии из нулевых старших битов машинного слова w.
	\return Длина серии.
	\remark CLZ == Count of Leading Zeros
	\safe Имеется ускоренная нерегулярная редакция.
*/
size_t u32CLZ(
	register u32 w		/*!< [in] слово */
);

size_t SAFE(u32CLZ)(register u32 w);
size_t FAST(u32CLZ)(register u32 w);

/*!	\brief Тасование битов

	Биты младшей половинки u32-слова w перемещаются в четные позиции,
	биты старшей половинки -- в нечетные.
	\return Слово с растасованными битами.
*/
u32 u32Shuffle(
	register u32 w		/*!< [in] слово */
);

/*!	\brief Обратное тасование битов

	Четные биты u32-слова w группируются в его младшей половинке, 
	нечетные -- в старшей.
	\return Слово с группированными битами.
*/
u32 u32Deshuffle(
	register u32 w		/*!< [in] слово */
);

/*!	\brief Аддитивно-мультипликативное обращение

	Выполняется адиттивное и мультипликативное обращение
	u32-слова-как-числа w по модулю 2^32.
	\pre w -- нечетное.
	\return - w^{-1} \mod 2^32.
	\remark Вычисляемое слово используется в редукции Монтгомери.
*/
u32 u32NegInv(
	register u32 w		/*!< [in] слово */
);

/*!	\brief Загрузка из буфера памяти

	Буфер [count]src преобразуется в массив [(count + 3) / 4]dest u32-слов.
*/
void u32From(
	u32 dest[],			/*!< [out] приемник */
	const void* src,	/*!< [in] источник */
	size_t count		/*!< [in] число октетов */
);

/*!	\brief Выгрузка в буфер памяти

	Буфер [count]dest формируется по массиву [(count + 3) / 4]src u32-слов.
*/
void u32To(
	void* dest,			/*!< [out] приемник */
	size_t count,		/*!< [in] число октетов */
	const u32 src[]		/*!< [in] источник */
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BEE2_U32_H */
