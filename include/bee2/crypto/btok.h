/*
*******************************************************************************
\file btok.h
\brief STB 34.101.79 (btok): cryptographic tokens
\project bee2 [cryptographic library]
\created 2022.07.04
\version 2022.07.16
\license This program is released under the GNU General Public License
version 3. See Copyright Notices in bee2/info.h.
*******************************************************************************
*/

#ifndef __BEE2_BTOK_H
#define __BEE2_BTOK_H

#include "bee2/defs.h"

/*!
*******************************************************************************
\file btok.h
\brief Криптографическая архитектура СТБ 34.101.79 (btok)
*******************************************************************************
*/


#ifdef __cplusplus
extern "C" {
#endif

/*!
*******************************************************************************
\file btok.h

Реализованы отдельные механизмы СТБ 34.101.79 (btok):
-	управление облегченными (Card Verifiable, CV) сертификатами.

\expect{ERR_BAD_INPUT} Все входные указатели действительны. Исключение
составляют оговоренные случаи. К ним относится запрос на определение объема
памяти, которую требуется зарезервировать при повторном вызове
(конструкция [len?]ptr).  
*******************************************************************************
*/

/*!
*******************************************************************************
\file btok.h

\section btok-cvc CV-сертификаты

Управление CV-сертификатами реализовано в соответствии с профилем, заданным
в СТБ 34.101.79. Профиль назначает:
- использование стандартных долговременных параметров bign-curveXXXv1;
- использование алгоритмов bign-sign для выработки и проверки подписи;
- использование алгоритмов belt-hash, bash384, bash512 для хэширования
  подписываемых данных;
- использование (опциональное) 5-октетного слова прав доступа к прикладной
  программе eId;
- использование (опциональное) 2-октетного слова прав доступа к прикладной
  программе eSign;
- использование специфических объектных идентификаторов, в частности,
  bign-pubkey.

Информационные поля CV-сертификата перечислены в структуре btok_cvc_t.
Сертификат представляет собой DER-код данных полей.

Отсутствие в сертификате слова прав доступа означает, что оно является нулевым,
т.е. что права доступа отстутствуют. По кодировании сертификата нулевое слово
прав доступа опускается. При этом наличие в декодируемом сертификате нулевого
слова не считается ошибкой.

Дата в сертификате задается 6 октетами по схеме YYMMDD (см. tm.h).
*******************************************************************************
*/

/*!	\brief Информационное содержание CV-сертификата */
typedef struct
{
	char authority[13];		/*!< издатель (8 <= strLen(authority) <= 12) */
	char holder[13];		/*!< владелец (8 <= strLen(holder) <= 12) */
	octet pubkey[128];		/*!< открытый ключ */
	size_t pubkey_len;		/*!< длина ключа в октетах (64, 96 или 128) */
	octet from[6];			/*!< дата начала действия (YYMMDD) */
	octet until[6];			/*!< дата окончания действия (YYMMDD) */
	octet hat_eid[5];  		/*!< права доступа к eId (optional) */
	octet hat_esign[2];     /*!< права доступа к eSign (optional) */
	octet sig[96];			/*!< подпись */
	size_t sig_len;			/*!< длина подписи в октетах (48, 72 или 96) */
} btok_cvc_t;

/*!	\brief Проверка содержания CV-сертификата

	Проверяется корректность содержания cvc CV-сертификата.
	Проверка завершается успешно, если:
	- cтроки cvc->authority и cvc->holder состоят из печатаемых символов;
	- длины строк cvc->authority и cvc->holder лежат в диапазоне от 8 до 12;
	- даты cvc->from и cvc->until корректны;
	- cvc->from <= cvc->until;
	- открытый ключ [cvc->pubkey_len]cvc->pubkey корректен (лежит на одной
	  из трех стандартных эллиптических кривых).
	\return ERR_OK, если проверка прошла успешно, и код ошибки в противном
	случае.
	\remark Подпись cvc->sig не проверяется.
*/
err_t btokCVCCheck(
	const btok_cvc_t* cvc		/*!< [in] содержание сертификата */
);

/*!	\brief Проверка содержания CV-сертификата с учетом данных издателя

	Проверяется корректность содержания cvc CV-сертификата с учетом
	содержания cvca сертификата (потенциального) издателя. 
	Проверка завершается успешно, если:
	- btokCVCCheck(cvc) == ERR_OK;
	- cvc->authority == cvca->holder;
	- даты cvca->from и cvca->until корректны;
	- cvca->from <= cvc->from && cvc->from <= cvca->until.
	\return ERR_OK, если проверка прошла успешно, и код ошибки в противном
	случае.
	\remark Подпись cvc->sig не проверяется.
*/
err_t btokCVCCheck2(
	const btok_cvc_t* cvc,	/*!< [in] содержание сертификата */
	const btok_cvc_t* cvca	/*!< [in] содержание сертификата издателя */
);

/*!	\brief Создание CV-сертификата

	Создается CV-сертификат [cert_len?]cert с содержанием cvc. Сертификат
	подписывается на личном ключе [privkey_len]privkey. Подпись сохраняется
	в [cvc->sig_len]cvc->sig. Если cvc->pubkey_len == 0, то открытый ключ
	[cvc->pubkey_len]cvc->pubkey строится по privkey и выходной
	сертификат выступает в роли доказательства владения личным ключом.
	Непосредственно перед созданием сертификата проверяется содержание cvc.
	Проверка завершается успешно, если:
	- btokCVCCheck(cvc) == ERR_OK.
	\return ERR_OK, если сертификат успешно создан, и код ошибки в противном
	случае.
	\remark Используется детерминированный режим выработки подписи. Если
	инициализирован штатный ГСЧ, то дополнительно используются данные от него.
	\remark Устанавливая cvc->pubkey_len == 0 && cvc->authority == cvc->holder,
	можно создать самоподписанный сертификат.
	\remark Устанавливая cvc->pubkey_len == 0 && cvc->authority != cvc->holder,
	можно создать предварительный сертификат, подпись которого будет изменена
	в дальнейшем стороной cvc->authority. Предварительный сертификат
	доказывает владение стороной cvc->holder личным ключом, которому
	соответствует открытый ключ cvc->pubkey. Предварительный сертификат
	выступает в роли запроса на выпуск сертификата.
*/
err_t btokCVCWrap(
    octet cert[],				/*!< [out] сертификат */
	size_t* cert_len,			/*!< [out] длина cert в октетах */
	btok_cvc_t* cvc,			/*!< [in/out] содержание сертификата */
	const octet privkey[],		/*!< [in] личный ключ */
	size_t privkey_len			/*!< [in] длина privkey в октетах */
);

/*!	\brief Разбор CV-сертификата

	Определяется и проверяется содержание cvc CV-сертификата [cert_len]cert.
	При проверке используется открытый ключ [pubkey_len]pubkey.
	Проверка завершается успешно, если:
	- btokCVCCheck(cvc) == ERR_OK;
	- подпись cert признается корректной на открытом ключе pubkey.
	Может передаваться нулевая длина pubkey_len, и тогда:
	- подпись не проверяется, если pubkey == 0;
	- подпись проверяется на открытом ключе сертификата,
	  если pubkey == cvc->pubkey;
	- индуцируется ошибка, если pubkey != 0 && pubkey != cvc->pubkey.
	\return ERR_OK, если сертификат успешно разобран, и код ошибки в
	противном случае.
	\remark Даже если подпись не проверяется, ее длина все равно
	контролируется. Неверная длина расценивается как ошибка формата.
	\remark Длина cert должна в точности равняться cert_len. Противное
	считается ошибкой формата.
*/
err_t btokCVCUnwrap(
	btok_cvc_t* cvc,			/*!< [out] содержание сертификата */
	const octet cert[],			/*!< [in] сертификат */
	size_t cert_len,			/*!< [in] длина cert в октетах */
	const octet pubkey[],		/*!< [in] открытый ключ */
	size_t pubkey_len			/*!< [in] длина pubkey в октетах */
);

/*!	\brief Выпуск CV-сертификата

	Выпускается CV-сертификат [cert_len?]cert с содержанием cvc. При выпуске
	используются личный ключ [privkeya_len]privkeya и сертификат
	[certa_len]certa издателя. Подпись сертификата сохраняется
	в [cvc->sig_len]cvc->sig. Перед выпуском проверяются следующие условия:
	- certa имеет корректный формат;
	- btokCVCCheck2(cvc, cvca) == ERR_OK, где cvca -- содержание certa;
	- открытый ключ в certa соответствует личному ключу privkeya.
	\return ERR_OK, если сертификат успешно выпущен, и код ошибки в противном
	случае.
	\remark Используется детерминированный режим выработки подписи. Если
	инициализирован штатный ГСЧ, то дополнительно используются данные от него.
*/
err_t btokCVCIss(
	octet cert[],				/*!< [out] сертификат */
	size_t* cert_len,			/*!< [out] длина cert в октетах */
	btok_cvc_t* cvc,			/*!< [in/out] содержание сертификата */
	const octet certa[],		/*!< [in] сертификат издателя */
	size_t certa_len,			/*!< [in] длина certa в октетах */
	const octet privkeya[],		/*!< [in] личный ключ издателя */
	size_t privkeya_len			/*!< [in] длина privkeya в октетах */
);

/*!	\brief Проверка CV-сертификата

	Проверяется корректность CV-сертификата [cert_len]cert на дату date, 
	для чего используется сертификат издателя [certa_len]certa.
	Проверка завершается успешно, если:
	- certa имеет корректный формат;
	- cert разбирается без ошибок на открытом ключе из certa;
	- btokCVCCheck2(cvc, cvca) == ERR_OK, где cvc и cvca -- содержание cert
	  и certa соответственно;
	- date попадает в срок действия cert.
	Указатель date может быть нулевым, и тогда последняя проверка не
	выполняется.
	\return ERR_OK, если сертификат признан корректным, и код ошибки в
	противном случае.
	\remark Текущую дату можно определить с помощью функции tmDate2().
*/
err_t btokCVCVal(
	const octet cert[],		/*!< [in] сертификат */
	size_t cert_len,		/*!< [in] длина cert в октетах */
	const octet certa[],	/*!< [in] сертификат издателя */
	size_t certa_len,		/*!< [in] длина certa в октетах */
	const octet* date		/*!< [in] дата проверки */
);

/*!	\brief Проверка CV-сертификата в цепочке

	Проверяется корректность CV-сертификата [cert_len]cert на дату date, 
	для чего используется содержание cvca сертификата издателя. В случае 
	успеха определяется содержание cvc проверяемого сертификата. 
	Проверка завершается успешно, если:
	- cert имеет корректный формат;
	- имя издателя в cert совпадает с именем владельца в cvca;
	- подпись cert признается корректной на открытом ключе из cvca;
	- срок действия, заданный в cvca, корректен;
	- начало действия cert не выходит за пределы срока действия cvca;
	- date попадает в срок действия cert.
	Указатель date может быть нулевым, и тогда последняя проверка не
	выполняется. Указатель cvc может быть нулевым, и тогда содержание
	проверяемого сертификата не возвращается.
	\return ERR_OK, если сертификат признан корректным, и код ошибки в
	противном случае.
	\remark Текущую дату можно определить с помощью функции tmDate2().
	\remark Интерфейс ориентирован на проверку цепочки сертификатов.
	Цепочка certa, certb, certc,... проверяется следующим образом:
	- btokCVCUnwrap(cvca, certa, certa_len, 0, 0);
	- btokCVCVal(cvcb, certb, certb_len, cvca, 0);
	- btokCVCVal(cvcc, certc, certc_len, cvcb, date);
	- ...
	\remark Если certa является самоподписанным и требуется проверить его
	подпись, то перва строчка в предыдущем коде должна выглядеть следующим
	образом:
	- btokCVCUnwrap(cvca, certa, certa_len, cvca->pubkey, 0);
*/
err_t btokCVCVal2(
	btok_cvc_t* cvc,			/*!< [out] содержание сертификата */
	const octet cert[],			/*!< [in] сертификат */
	size_t cert_len,			/*!< [in] длина cert в октетах */
	const btok_cvc_t* cvca,		/*!< [in] содержание сертификата издателя */
	const octet* date			/*!< [in] дата проверки */
);

/*!	\brief Проверка соответствия CV-сертификата

	Проверяется соответствие между CV-сертификатом [cert_len]cert и личным
	ключом [privkey_len]privkey.
	Проверка завершается успешно, если:
	- cert имеет корректный формат;
	- открытый ключ cert соответствует privkey.
	\return ERR_OK, если соответствие подтверждено, и код ошибки в
	противном случае.
*/
err_t btokCVCMatch(
	const octet cert[],			/*!< [in] сертификат */
	size_t cert_len,			/*!< [in] длина cert в октетах */
	const octet privkey[],		/*!< [in] личный ключ */
	size_t privkey_len			/*!< [in] длина privkey в октетах */
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__BEE2_BTOK_H */
