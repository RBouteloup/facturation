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
 * $Id: MyString.c 293 2011-10-02 15:05:44Z sebtic $
 */

#include <MyString.h>
#include <limits.h>

/** Like the tolower() function. It converts the letter c to lower case, if possible.
 * @param c the letter to convert
 * @return the lower case letter associated to c if c is a letter, or c otherwise
 */
char IMPLEMENT(toLowerChar)(char c)
{
    int caraToInt = (int)c;

    if (c >= 'A' && c <= 'Z')
    {
        caraToInt = caraToInt + ('a' - 'A');
        c = (char)caraToInt;
        return c;
    }
    return c;
}

/** Like the toupper() function. It converts the letter c to upper case, if possible.
 * @param c the letter to convert
 * @return the upper case letter associated to c if c is a letter, or c otherwise
 */
char IMPLEMENT(toUpperChar)(char c)
{
    int charToInt = (int)c;

    if (c >= 'a' && c <= 'z')
    {
        charToInt = charToInt - ('a' - 'A');
        c = (char)charToInt;
        return c;
    }
    return c;
}

/** Like the strcmp() function. It compares the two strings str1 and str2.
 * It returns an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2.
 * @param str1 the first string
 * @param str2 the second string
 * @return an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2
 */
int IMPLEMENT(compareString)(const char * str1, const char * str2)
{
    size_t count = 0;
    int result = 0;

    while (count <= stringLength(str1) && result == 0)
    {
        if (str1[count] < str2[count])
            result = -1;

        else if (str1[count] > str2[count])
            result = 1;

            count++;
    }
    return result;
}

/** Like the strcasecmp() function. It compares the two strings str1 and str2, ignoring the case of the characters.
 * It returns an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2.
 * @param str1 the first string
 * @param str2 the second string
 * @return an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2
 */
int IMPLEMENT(icaseCompareString)(const char * str1, const char * str2)
{
    size_t count = 0;
    int result = 0;

    while (count <= stringLength(str1) && result == 0)
    {
        if (toLowerChar(str1[count]) < toLowerChar(str2[count]))
            result = -1;

        else if (toLowerChar(str1[count]) > toLowerChar(str2[count]))
            result = 1;

            count++;
    }
    return result;
}

/** Like the strlen() function. It calculates the length of the string str, not including the terminating '\\0' character.
 * @param str the string
 * @return the number of characters in str.
 */
size_t IMPLEMENT(stringLength)(const char * str)
{
    size_t count = 0;
    while (str[count] != '\0')
        count++;

    return count;
}

/** Copy the string pointed to by src, including the terminating null byte ('\\0'), to the buffer pointed to by dest.
 * @warning The strings may not overlap, and the destination string dest must be large enough to receive the copy.
 *
 * @param dest the destination string
 * @param src the source string
 */
void IMPLEMENT(copyString)(char * dest, const char * src)
{
    copyStringWithLength(dest, src, stringLength(src) + 1);
}

/** Copy the first destSize characters of the string pointed to by src, including the terminating null byte ('\\0'), to the buffer pointed to by dest.
 * @warning The strings may not overlap, and the destination string dest must be large enough to receive the copy.
 * @warning If src is longer than destSize characters, the string is truncated and the terminating null byte ('\\0') is added.
 *
 * @param dest the destination string
 * @param src the source string
 * @param destSize the maximal number of characters to copy
 */
void IMPLEMENT(copyStringWithLength)(char * dest, const char * src, size_t destSize)
{
    size_t count = 0;
    while ((src[count] != '\0') && (count+1 < destSize))
    {
        dest[count] = src[count];
        count++;
    }
    dest[count] = '\0';
}

/** Like the strdup() function. It creates a copy of the string on the heap.
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param str the string to duplicate
 */
char * IMPLEMENT(duplicateString)(const char * str)
{
    size_t sizeChaine = stringLength(str) + 1;
    char * copyStr = (char*)malloc(sizeChaine);

    if (copyStr == NULL)
        fatalError("malloc error : Allocation of copyStr on the heap failed");

    copyStringWithLength(copyStr, str, sizeChaine);
    return copyStr;
}

/** Test if the string str begins by the string start, ignoring the case of the characters.
 * @param start the string which should be at the beginning of str
 * @param str the string to test
 * @return a non null number (true) if the str begins with the string start, false otherwise
 */
int IMPLEMENT(icaseStartWith)(const char * start, const char * str)
{
    int count = 0, result = 0;

    while (toLowerChar(start[count]) == toLowerChar(str[count]) && start[count] != '\0')
    {
        count++;
        result = 1;
    }
    if (start[count] != '\0')
        result = 0;

    return result;
}

/** Test if the string str ends by the string start, ignoring the case of the characters.
 * @param end the string which should be at the end of str
 * @param str the string to test
 * @return a non null number (true) if the str ends with the string end, false otherwise
 */
int IMPLEMENT(icaseEndWith)(const char * end, const char * str)
{
    size_t sizeOfStr = stringLength(str) - 1;
    size_t sizeOfEnd = stringLength(end) - 1;
    int result = 0;

    while (toLowerChar(str[sizeOfStr]) == toLowerChar(end[sizeOfEnd]) && sizeOfEnd != 0)
    {
        sizeOfStr--;
        sizeOfEnd--;
        result = 1;
    }
    if (toLowerChar(str[sizeOfStr]) != toLowerChar(end[sizeOfEnd]))
        result = 0;

    return result;
}

/** Create a new string on the heap which is the result of the concatenation of the two strings.
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param str1 the first string
 * @param str2 the second string
 * @return the new string
 */
char * IMPLEMENT(concatenateString)(const char * str1, const char * str2)
{
    size_t sizeOfStr1 = stringLength(str1);
    size_t sizeOfStr2 = stringLength(str2);

    char * concatStr = (char*)malloc(sizeOfStr1 + sizeOfStr2 + 1);

    if (concatStr == NULL)
        fatalError("malloc error : Allocation of concatStr on the heap failed");

    copyStringWithLength(concatStr, str1,sizeOfStr1 + 1);
    copyStringWithLength(concatStr + sizeOfStr1, str2, sizeOfStr2 + 1);

    return concatStr;
}

/** Like the index() function. It returns a pointer to the first occurrence of the character c in the string str.
 * @param str the string to search in
 * @param c the character to find
 * @return a pointer to the first occurrence of the character c in the string str if c is in str, NULL otherwise
 */
const char * IMPLEMENT(indexOfChar)(const char * str, char c)
{
    int count = 0;
    const char * resultat = NULL;

    while (str[count] != '\0' && resultat == NULL)
    {
        if (str[count] == c)
            resultat = &str[count];

        count++;
    }
    return resultat;
}

/** Create a copy on the heap of part of a string. The new string contains the characters pointed from start (inclusive) to end (exclusive).
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param start a pointer to the first character of the new string
 * @param end a pointer to the next character of the new string (the pointed character is not part of the new string)
 * @return the new string
 *
 * @code
 * char * str = "abcdef";
 * char * s1 = subString(str,str);
 * char * s2 = subString(str,str+strlen(str));
 * char * s3 = subString(str+1,str+2);
 *
 * We get:
 *    s1: "";
 *    s2: "abcdef";
 *    s3: "b";
 * @endcode
 */
char * IMPLEMENT(subString)(const char * start, const char * end)
{
    size_t count = 0;
    size_t diffLengthStartEnd = (size_t)end-(size_t)start;
    char * cpyString = (char*)malloc(diffLengthStartEnd + 1);

    if (cpyString == NULL)
        fatalError("malloc error : Allocation of cpyString on the heap failed");

    while (count != diffLengthStartEnd)
    {
        cpyString[count] = start[count];
        count++;
    }
    cpyString[count] = '\0';
    return cpyString;
}

/** Like the strstr() function. It returns a pointer to the first occurrence of the string aiguille in the string meule_de_foin.
 * @param meule_de_foin the string to search in
 * @param aiguille the string to find
 * @return a pointer to the first occurrence of the string aiguille in the string meule_de_foin if aiguille is in meule_de_foin, NULL otherwise
 */
const char * IMPLEMENT(indexOfString)(const char *meule_de_foin, const char *aiguille)
{
    int count1 = 0, count2 = 0;
    const char * result = NULL;

    while (meule_de_foin[count1] != '\0' && count2 != -1)
    {
        if (meule_de_foin[count1] == aiguille[count2])
        {
            count1++;
            count2++;
            if (aiguille[count2] == '\0')
            {
                count2 = -1;
                result = &meule_de_foin[count1];
                result -= stringLength(aiguille);
            }
        }
        else
        {
            if (count2 != 0)
                count1--;

            count1++;
            count2 = 0;
        }
    }
    return result;
}

/** Convert a string to upper case.
 * @param str the string to convert
 */
void IMPLEMENT(makeUpperCaseString)(char * str)
{
    int count = 0;
    while (str[count] != '\0')
    {
        str[count] = toUpperChar(str[count]);
        count++;
    }
}

/** Convert a string to lower case.
 * @param str the string to convert
 */
void IMPLEMENT(makeLowerCaseString)(char * str)
{
    int count = 0;
    while (str[count] != '\0')
    {
        str[count] = toLowerChar(str[count]);
        count++;
    }
}

/** Create a new string on the heap which contents is the result of the insertion in src of insertLength characters from  toBeInserted at position insertPosition.
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param src the string in which the insertion is done
 * @param insertPosition the position where the insertion is done
 * @param toBeInserted a pointer on the first character to insert
 * @param insertLength the number of character to insert
 * @return the new string
 */
char * IMPLEMENT(insertString)(const char * src, int insertPosition, const char * toBeInserted, int insertLength)
{
    int count1 = 0, count2 = 0;
    char * copySrc = (char*)malloc(stringLength(src) + stringLength(toBeInserted) + 1);

    if (copySrc == NULL)
        fatalError("malloc error : Allocation of copySrc on the heap failed");

    while (src[count1] != '\0' || count2 == 0)
    {
        if (count2 != 0)
        {
            copySrc[count1+insertLength] = src[count1];
            count1++;
        }
        else if (count1 != insertPosition)
        {
            copySrc[count1] = src[count1];
            count1++;
        }
        else
        {
            while (toBeInserted[count2] != '\0' && count2 < insertLength)
            {
                copySrc[count1] = toBeInserted[count2];
                count2++;
                count1++;
            }
            count1 -= insertLength;
        }
    }
    copySrc[count1+insertLength] = '\0';
    return copySrc;
}
