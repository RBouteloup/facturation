/*
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
 * $Id: DocumentUtil.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <DocumentUtil.h>

static size_t countCharOfInt(long id, int base);


/** Create a new string on the heap which represents the parameter as a number in basis 36.
 * @param id the number to convert
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(computeDocumentNumber)(long id)
{
    size_t nbrCharInId = countCharOfInt(id, 36);
    char * stringBase36 = (char*)calloc(nbrCharInId, sizeof(char));

    if (stringBase36 == NULL)
        fatalError("calloc error : Allocation of stringBase36 on the heap failed");

    while (nbrCharInId > 1)
    {
        if (id % 36 >= 10)
        {
            stringBase36[nbrCharInId-2] = (char)((id % 36) + 'A' - 10);
        }
        else
        {
            stringBase36[nbrCharInId-2] = (char)((id % 36) + '0');
        }
        id /= 36;
        nbrCharInId--;
    }
    return stringBase36;
}

/** Create a new string on the heap which represents the date in the format DD/MM/YYYY.
 * @param day the day
 * @param month the month
 * @param year the year
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(formatDate)(int day, int month, int year)
{
    int count = 0, temp = year;
    char * date2String = (char*)malloc(11 * sizeof(char));

    if (date2String == NULL)
        fatalError("malloc error : Allocation of date2String on the heap failed");

    while (count < 10)
    {
        switch (count)
        {
        case 2:
            date2String[count] = '/';
            count++;
            break;
        case 5:
            date2String[count] = '/';
            count++;
            break;
        default:
            date2String[count] = 'a';
            count++;
            break;
        }
    }
    count--;
    while (count >= 0)
    {
        switch(count)
        {
        case 2:
            temp = day;
            break;
        case 5:
            temp = month;
            break;
        default:
            date2String[count] = (char)((temp % 10) + '0');
            temp = temp / 10;
            break;
        }
        count--;
    }
    date2String[10] = '\0';
    return date2String;
}


/** Write a string in a binary file
 * @param str the string
 * @param file the file
 */
void IMPLEMENT(writeString)(const char * str, FILE * file)
{
    size_t nbrChara = stringLength(str);

    if (fwrite(&nbrChara, sizeof(size_t), 1, file) < 1)
        fatalError("fwrite error : return value is not valid.");

    size_t result = fwrite(str, stringLength(str), 1, file);

    if (result < 1 && stringLength(str) != 0)
        fatalError("fwrite error : return value is not valid.");
}

/** Read a string from a binary file
 * @param file the file
 * @return a new string created on the heap which contains the read string
 * @see writeString()
 */
char * IMPLEMENT(readString)(FILE * file)
{
    size_t nbrChara = 0;

    if (fread(&nbrChara, sizeof(size_t), 1, file) < 1)
        fatalError("fwrite error : return value is not valid.");

    char * newString = (char*)malloc(sizeof(char) * nbrChara + 1);

    if (newString == NULL)
        fatalError("malloc error : Allocation of char * newString failed.");

    memset(newString, '\0', sizeof(char) * nbrChara + 1);

    size_t result = fread(newString, nbrChara, 1, file);

    if (result < 1 && nbrChara != 0)
        fatalError("fwrite error : return value is not valid.");

    return newString;
}


/** Count the number of letter in integer
 * @param id the number to count
 * @param base the base to convert the number
 * @return a new size_t that contain the number of letter in integer taking into consideration the base
 */
static size_t countCharOfInt(long id, int base)
{
	size_t compt = 0;

	if (id == 0)
        return compt + 2;

	while (id != 0)
	{
		id /= base;
		compt++;
	}
	return compt + 1;
}
