/*
*******************************************************************************
\file bign_lcl.h
\brief STB 34.101.45 (bign): local declarations
\project bee2 [cryptographic library]
\created 2014.04.03
\version 2023.09.25
\copyright The Bee2 authors
\license Licensed under the Apache License, Version 2.0 (see LICENSE.txt).
*******************************************************************************
*/

#ifndef __BEE2_BIGN_LCL_H
#define __BEE2_BIGN_LCL_H

#include "bee2/crypto/bign.h"
#include "bee2/math/ec.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*******************************************************************************
Эллиптические кривые СТБ 34.101.45 могут использоваться в других модулях. 
Определения, даваемые ниже, позволяют использовать низкоуровневые функции bign,
предназначенные для управления эллиптической кривой.
*******************************************************************************
*/

/*!	\brief Проверка работоспособности параметров

	Проводится минимальная проверка работоспособности параметров params,
	обеспечивающая работоспобность высокоуровневых функций. Конкретнее,
	проверяется выполнение следующих ограничений на поля params:
	- l == 128 || l == 192 || l == 256;
	- p и q -- 2l-битовые нечетные числа;
	- p[0] % 4 == 3;
	- a != 0 и b != 0;
	- неиспользуемые октеты p, a, b, yG обнулены.
	\return ERR_OK в случае успеха и код ошибки в противном	случае.
*/

bool_t bignIsOperable(
	const bign_params* params	/*!< [in] долговременные параметры */
);

/*!	\brief Потребности в стеке 

	Определяется глубина стека, который требуется высокоуровневой функции 
	для работы с эллиптической кривой, описываемой объясненными ниже 
	размерностями n, f_deep, ec_d, ec_deep. 
	\remark При расчете глубины не следует учитывать память для 
	размещения описаний базового поля и эллиптической кривой.
*/

typedef size_t (*bign_deep_i)(
	size_t n,				/*!< [in] число слов для хранения элемента поля */
	size_t f_deep,			/*!< [in] глубина стека базового поля */
	size_t ec_d,			/*!< [in] число проективных координат */
	size_t ec_deep			/*!< [in] глубина стека эллиптической кривой */
);

/*!	\brief Длина состояния

	Возвращается длина состояния (в октетах) высокоуровневой функции, 
	которая работает на уровне стойкости l и имеет потребности в стеке deep.
	\pre l == 128 || l == 192 || l == 256.
	\return Длина состояния.
	\remark Состояние включает как локальные переменные, так и стек.
*/
size_t bignStart_keep(
	size_t l,				/*!< [in] уровень стойкости */
	bign_deep_i deep		/*!< [in] потребности в стековой памяти */
);

/*!	\brief Начало работы с параметрами

	По долговременным параметрам params по адресу state формируется описание 
	эллиптической кривой.
	\pre bignIsOperable(params).
	\return ERR_OK, если описание успешно создано, и код ошибки в противном 
	случае.
	\remark Высокоуровневый механизм должен работать по следующей схеме:
	-	описать потребности в стеке с помощью функции интерфейса bign_deep_i;
	-	определить длину состояния, вызвав bignStart_keep();
	-	определить начало стека как память в state сразу после описания 
		эллиптической кривой.
	.
*/
err_t bignStart(
	void* state,				/*!< [out] состояние */
	const bign_params* params	/*!< [in] долговременные параметры */
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BEE2_BIGN_LCL_H */