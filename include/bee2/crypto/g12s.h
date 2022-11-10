﻿/*
*******************************************************************************
\file g12s.h
\brief GOST R 34.10-94 (Russia): digital signature algorithms
\project bee2 [cryptographic library]
\created 2012.04.27
\version 2015.08.27
\license This program is released under the GNU General Public License 
version 3. See Copyright Notices in bee2/info.h.
*******************************************************************************
*/

/*!
*******************************************************************************
\file g12s.h
\brief Алгоритмы ГОСТ Р 34.10-94 (Россия)
*******************************************************************************
*/

#ifndef __BEE2_G12S_H
#define __BEE2_G12S_H

#include "bee2/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
*******************************************************************************
\file g12s.h

\section g12s-common ГОСТ Р 34.10-2012: Общие положения

Реализованы алгоритмы ГОСТ Р 34.10-2012 (g12s). При ссылках на алгоритмы, 
таблицы, другие объекты подразумеваются разделы ГОСТ Р 34.10-2012, в которых 
эти объекты определены.

\expect{ERR_BAD_INPUT} Все входные указатели корректны.

\safe todo
*******************************************************************************
*/

/*!
*******************************************************************************
\file g12s.h

\section g12s-params Управление долговременными параметрами

Структура g12s_params описывает долговременные параметры g12s. Поля структуры
обозначаются также, как в п. 5.2. 

Максимальные размерности G12S_FIELD_SIZE, G12S_ORDER_SIZE соответствуют уровню 
стойкости l == 512.

При g12s_params::l == 256 в полях g12s_params::p и g12s_params::q используется 
только первая половина октетов. При g12s_params::l == 512 используются 
все октеты.

В полях g12s_params::a, g12s_params::b, g12s_params::xP, g12s_params::yP 
используется no октетов, где no --- длина g12s_params::p в октетах:
no = memNonZeroSize(g12s_params::p, G12S_FIELD_SIZE * g12s_params::l / 512).

Неиспользуемые октеты могут быть заданы произвольным образом.

Все поля структуры g12s_params, за исключением поля g12s_params::n, кодируют 
определенные числа. При кодировании используются правила little-endian.
*******************************************************************************
*/

#define G12S_FIELD_SIZE		(64 + 4)
#define G12S_ORDER_SIZE		(64)

/*
*******************************************************************************
Управление долговременными параметрами
*******************************************************************************
*/

/*!	\brief Долговременные параметры g12s */
typedef struct g12s_params
{
	u32 l;							/*!< уровень стойкости (256 или 512) */
	octet p[G12S_FIELD_SIZE];		/*!< модуль p */
	octet a[G12S_FIELD_SIZE];		/*!< коэффициент a */
	octet b[G12S_FIELD_SIZE];		/*!< коэффициент b */
	octet q[G12S_ORDER_SIZE];		/*!< порядок группы точек */
	u32 n;							/*!< кофактор */
	octet xP[G12S_FIELD_SIZE];		/*!< x-координата точки P */
	octet yP[G12S_FIELD_SIZE];		/*!< y-координата точки P */
} g12s_params;

/*!	\brief Загрузка стандартных долговременных параметров

	В params загружаются стандартные долговременные параметры с именем name.
	Поддерживаются следующие имена:
	-	"1.2.643.2.2.35.0" (параметры проверочного примера A.1, l == 256);
	-	"1.2.643.2.2.35.1" (параметры КриптоПро, набор A, l == 256);
	-	"1.2.643.2.2.35.2" (параметры КриптоПро, набор B, l == 256);
	-	"1.2.643.2.2.35.3" (параметры КриптоПро, набор C, l == 256);
	-	"1.2.643.2.9.1.8.1" (параметры КриптоКом, l == 256);
	-	"1.2.643.7.1.2.1.2.0" (параметры проверочного примера A.2, l == 512);
	-	"1.2.643.7.1.2.1.2.1" (рабочие параметры paramsetA, l == 512); 
	-	"1.2.643.7.1.2.1.2.2" (рабочие параметры paramsetB, l == 512).
	.
	\remark Параметры и их имена (идентификаторы) взяты из следующих 
	источников:
	-	файл gost_params.c, который является частью OpenSSL;
	-	проект "Методические рекомендации по заданию параметров эллиптических 
		кривых в соответствии с ГОСТ Р 34.10-2012" 
		(http://www.tc26.ru, октябрь 2013 г.);
	-	документ "Идентификаторы объектов технического комитета по 
		стандартизации "Криптографическая защита информации" (ТК26)"
		(http://www.tc26.ru/metodiki/OID_TK_26/index.php, апрель 2014 г.).
	.
	\return ERR_OK, если параметры успешно загружены, и код ошибки
	в противном случае.
*/
err_t g12sStdParams(
	g12s_params* params,		/*!< [out] стандартные параметры */
	const char* name			/*!< [in] имя параметров */
);

/*!	\brief Проверка долговременных параметров

	Проверяется корректность долговременных параметров params.
	\return ERR_OK, если параметры корректны, и код ошибки в противном
	случае.
*/
err_t g12sValParams(
	const g12s_params* params	/*!< [in] параметры */
);

/*!
*******************************************************************************
\file g12s.h

\section g12s-keys Управление ключами

Ключи распространяются вместе с долговременными параметрами. Долговременный
параметр l определяет длину личного ключа (privkey): ключ состоит 
из l / 4 октетов. Открытый ключ (pubkey) состоит из 2 * no октетов,
где no --- длина модуля p в октетах.

Буфер privkey и половинки буфера pubkey представляют собой кодовые 
представления определенных чисел. При кодировании используются правила 
little-endian. Первая половинка pubkey кодирует x-координату открытого ключа, 
вторая половинка -- y-координату.
*******************************************************************************
*/

/*!	\brief Генерация пары ключей

	Для заданных долговременных параметров params генерируются личный privkey
	и открытый pubkey ключи. При генерации используется генератор rng
	и его состояние rng_state.
	\expect{ERR_BAD_PARAMS} Параметры params корректны.
	\expect{ERR_BAD_RNG} Генератор rng (с состоянием rng_state) корректен.
	\expect Используется криптографически стойкий генератор rng.
	\return ERR_OK, если ключи сгенерированы, и код ошибки в противном
	случае.
*/
err_t g12sGenKeypair(
	octet privkey[],				/*!< [out] личный ключ */
	octet pubkey[],				/*!< [out] открытый ключ */
	const g12s_params* params,	/*!< [in] долговременные параметры */
	gen_i rng,					/*!< [in] генератор случайных чисел */
	void* rng_state				/*!< [in/out] состояние генератора */
);

/*!
*******************************************************************************
\file g12s.h

\section g12s-sign Электронная цифровая подпись (ЭЦП)

Данные, которые используются при выработке / проверке подписи,
распространяются вместе с долговременными параметрами. Долговременный
параметр l определяет длину хэш-значения hash и подписи sig в октетах:
hash состоит из l / 8 октетов, sig -- из l / 4 октетов.

Буфер hash и половинки буфера sig являются кодовыми представлениями 
определенных чисел. При кодировании используются правила big-endian
(см. формулы (14), (19) и шаг 6 алгоритма выработки ЭЦП). 
Первая половинка sig кодирует число r, вторая половинка -- число s.
*******************************************************************************
*/

/*!	\brief Выработка ЭЦП

	Вырабатывается подпись sig сообщения с хэш-значением hash. Подпись 
	вырабатывается на личном ключе privkey. При выработке ЭЦП
	используются долговременные параметры params и генератор rng
	с состоянием rng_state.
	\expect{ERR_BAD_PARAMS} Параметры params корректны.
	\expect{ERR_BAD_PRIVKEY} Личный ключ privkey корректен.
	\expect{ERR_BAD_RNG} Генератор rng (с состоянием rng_state) корректен.
	\expect Генератор rng является криптографически стойким.
	\return ERR_OK, если подпись выработана, и код ошибки в противном
	случае.
*/
err_t g12sSign(
	octet sig[],					/*!< [out] подпись */
	const g12s_params* params,	/*!< [in] долговременные параметры */
	const octet hash[],			/*!< [in] хэш-значение */
	const octet privkey[],		/*!< [in] личный ключ */
	gen_i rng,					/*!< [in] генератор случайных чисел */
	void* rng_state				/*!< [in/out] состояние генератора */
);

/*!	\brief Проверка ЭЦП

	Проверяется ЭЦП sig сообщения с хэш-значением hash. При проверке 
	используются долговременные параметры params и открытый ключ pubkey. 
	\expect{ERR_BAD_PARAMS} Параметры params корректны.
	\expect{ERR_BAD_PUBKEY} Открытый ключ pubkey корректен.
	\return ERR_OK, если подпись корректна, и код ошибки в противном случае.
	\remark При нарушении ограничений на ЭЦП возвращается код ERR_BAD_SIG.
*/
err_t g12sVerify(
	const g12s_params* params,	/*!< [in] долговременные параметры */
	const octet hash[],			/*!< [in] хэш-значение */
	const octet sig[],			/*!< [in] подпись */
	const octet pubkey[]		/*!< [in] открытый ключ */
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* __BEE2_G12S_H */
