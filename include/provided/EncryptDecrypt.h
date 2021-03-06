/**
 * Copyright 2010 Sébastien Aupetit <sebastien.aupetit@univ-tours.fr>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 */

#ifndef FACTURATION_PROVIDED_ENCRYPTDECRYPT_H
#define FACTURATION_PROVIDED_ENCRYPTDECRYPT_H

/** @defgroup ProvidedEncryptDecrypt Provided functions
 * @ingroup EncryptDecrypt
 * @{
 */

/** encrypt a string using the key
 * @param key the key
 * @param str the string to encrypt
 * @note The string is encrypted in-place
 */
void provided_encrypt(const char * key, char * str);

/** decrypt a string using the key
 * @param key the key
 * @param str the string to decrypt
 * @note The string is decrypted in-place
 */
void provided_decrypt(const char * key, char * str);

/** @} */

#endif
