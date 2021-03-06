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
 * $Id: Dictionary.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Dictionary.h>

static char * findCharInString(char * str, char c);
static char * formatString(char * copyValueStringEntry, char * buffer, char * charaEqual);
static char * formatNumber(char * copyValueEntryNumber, char * buffer, char * charaEqual);
static char * valueAfterEqual(char * buffer, char * charaEqual);
static char * parameterNotFound(Dictionary * dictionary, char * copyFormat);
static char * ifModeleIsComplete(Dictionary * dictionary, char * copyFormat, char * stringFormated, char * stringFormatedFinal, char * charaAccolade);
static char * formatSecondParameterNumber(char * charaEqual, char * copyFormat, char * buffer, char * stringFormated, char * stringFormatedFinal);
static char * formatSecondParameterString(char * charaEqual, char * copyFormat, char * buffer, char * stringFormated, char * stringFormatedFinal);
static void setCursorEndOfModele(char * copyFormat);


/** Create an empty dictionary on the heap
 * @return a new dictionary
 * @warning the dictionary should be destroyed using Dictionary_destroy()
 */
Dictionary * IMPLEMENT(Dictionary_create)(void)
{
    Dictionary * dictionary = (Dictionary*) malloc(sizeof(Dictionary));

    if (dictionary == NULL)
        fatalError("malloc error : Attribution of Dictionary * dictionary on the heap failed");

    dictionary->count = 0;
    dictionary->entries = NULL;

    return dictionary;
}

/** Destroy a dictionary
 * @param dictionary the dictionary
 */
void IMPLEMENT(Dictionary_destroy)(Dictionary * dictionary)
{
    int i = 0;

    while (dictionary->count > i)
    {
        if (dictionary->entries[i].type == STRING_ENTRY)
            free(dictionary->entries[i].value.stringValue);

        free(dictionary->entries[i].name);
        i++;
    }
    free(dictionary->entries);
    free(dictionary);
}

/** Get a pointer on the entry associated with the given entry name
 * @param dictionary the dictionary
 * @param name the name of the entry
 * @return a pointer on the entry or NULL if the entry was not found
 */
DictionaryEntry * IMPLEMENT(Dictionary_getEntry)(Dictionary * dictionary, const char * name)
{
    int i = 0;

    if (dictionary->entries == NULL)
        return NULL;

    while (dictionary->count > i)
    {
        if (icaseCompareString(dictionary->entries[i].name, name) == 0)
            return &(dictionary->entries[i]);
        i++;
    }
    return NULL;
}

/** Define or change a dictionary entry as a string
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void IMPLEMENT(Dictionary_setStringEntry)(Dictionary * dictionary, const char * name, const char * value)
{
    DictionaryEntry * entryTemp = Dictionary_getEntry(dictionary, name);

    if (entryTemp == NULL)
    {
        dictionary->count += 1;

        if (dictionary->entries == NULL)
        {
            dictionary->entries = (DictionaryEntry*) malloc(sizeof(DictionaryEntry));

            if (dictionary->entries == NULL)
                fatalError("error malloc : Attribution of dictionary->entries on the heap failed");
        }
        else
        {
            dictionary->entries = realloc(dictionary->entries, sizeof(DictionaryEntry) * (size_t)dictionary->count);

            if (dictionary->entries == NULL)
                fatalError("error realloc : Attribution of dictionary->entries on the heap failed");
        }

        entryTemp = dictionary->entries + (dictionary->count - 1);
        entryTemp->name = duplicateString(name);
        entryTemp->value.stringValue = duplicateString(value);
        entryTemp->type = STRING_ENTRY;
    }
    else
    {
        if (entryTemp->type == STRING_ENTRY)
            free(entryTemp->value.stringValue);

        entryTemp->value.stringValue = duplicateString(value);
        entryTemp->type = STRING_ENTRY;
    }
}

/** Define or change a dictionary entry as a number
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void IMPLEMENT(Dictionary_setNumberEntry)(Dictionary * dictionary, const char * name, double value)
{
    DictionaryEntry * entryTemp = Dictionary_getEntry(dictionary, name);

    if (entryTemp == NULL)
    {
        dictionary->count += 1;

        if (dictionary->entries == NULL)
        {
            dictionary->entries = (DictionaryEntry*) malloc(sizeof(DictionaryEntry));

            if (dictionary->entries == NULL)
                fatalError("error malloc : Attribution of dictionary->entries on the heap failed");
        }
        else
        {
            dictionary->entries = realloc(dictionary->entries, sizeof(DictionaryEntry) * (size_t)dictionary->count);

            if (dictionary->entries == NULL)
                fatalError("error realloc : Attribution of dictionary->entries on the heap failed");
        }

        entryTemp = dictionary->entries + (dictionary->count - 1);
        entryTemp->name = duplicateString(name);
        entryTemp->value.numberValue = value;
        entryTemp->type = NUMBER_ENTRY;
    }
    else
    {
        if (entryTemp->type == STRING_ENTRY)
            free(entryTemp->value.stringValue);

        entryTemp->value.numberValue = value;
        entryTemp->type = NUMBER_ENTRY;
    }
}

/** Create a new string on the heap which is the result of the formatting of format according to the dictionary content
 * @param dictionary the dictionary
 * @param format the string to format
 * @return a new string created on the heap
 * @warning the user is responsible for freeing the returned string
 */
char * IMPLEMENT(Dictionary_format)(Dictionary * dictionary, const char * format)
{
    char * copyFormatSmall = NULL, * charaAccolade = NULL, * stringFormatedFinal = duplicateString(""), * copyOfParameterNotFound = NULL;
    char stringFormated[2048] = "";
    size_t movePourcentage = 0, beforePourcentage = 0;

    memset(stringFormated, '\0', sizeof(char) * 2048);

    while (movePourcentage <= stringLength(format))
    {
        while (format[movePourcentage] != '%' && stringLength(format) != movePourcentage)
            movePourcentage++;

        copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) + 1);
        free(stringFormatedFinal);

        if (stringLength(stringFormated) != 0)
            stringFormatedFinal = insertString(stringFormated, (int)(movePourcentage - beforePourcentage), format + beforePourcentage, 1);

        beforePourcentage = movePourcentage;

        while (format[movePourcentage+1] != '%')
            movePourcentage++;

        copyFormatSmall = insertString("", 0 , format + beforePourcentage + 1, (int)(movePourcentage - beforePourcentage));

        charaAccolade = findCharInString(copyFormatSmall, '{');

        if (charaAccolade == NULL)
        {
            copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) + 1);
            free(stringFormatedFinal);
            copyOfParameterNotFound = parameterNotFound(dictionary, copyFormatSmall);
            stringFormatedFinal = concatenateString(stringFormated, copyOfParameterNotFound);
            setCursorEndOfModele(copyFormatSmall);
        }
        else if(copyFormatSmall[0] == '%')
        {
            copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) + 1);
            free(stringFormatedFinal);
            stringFormatedFinal = concatenateString(stringFormated, "%");
            setCursorEndOfModele(copyFormatSmall);
        }
        else
        {
            stringFormatedFinal = ifModeleIsComplete(dictionary, copyFormatSmall, stringFormated, stringFormatedFinal, charaAccolade);
        }

        if (copyFormatSmall == NULL)
            printf("\n\n -- TRADUCTION OF MODEl FAILED -- \n\n");

        if (copyFormatSmall[1] != '\0')
        {
            copyStringWithLength(copyFormatSmall, copyFormatSmall + 3, stringLength(copyFormatSmall) -2);
            copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) + 1);
            free(stringFormatedFinal);
            stringFormatedFinal = insertString(stringFormated, (int)stringLength(stringFormated), " ", 1);
        }
    }
    free(copyFormatSmall);
    free(copyOfParameterNotFound);
    return stringFormatedFinal;
    }

/**Function is called when model is complete to format a string or number
 * @param dictionary the dictionary
 * @param copyFormat a copy of format
 * @param stringFormated a temporary string
 * @param stringFormatedFinal the future string returned
 * @param charaAccolade a string contain accolade charactere
 * @return a pointer to the string formated
*/

char * ifModeleIsComplete(Dictionary * dictionary, char * copyFormat, char * stringFormated, char * stringFormatedFinal, char * charaAccolade)
{
    DictionaryEntry * entry;
    char buffer[1024] = "", bufferForNumberEntry[1024] = "";
    char * charaEqual = NULL;
    char * copyOfFormat = NULL;

    memset(buffer, '\0', sizeof(char) * 1024);
    memset(bufferForNumberEntry, '\0', sizeof(char) * 1024);
    copyStringWithLength(buffer, copyFormat, stringLength(copyFormat) - stringLength(charaAccolade) + 1);
    entry = Dictionary_getEntry(dictionary, buffer);

    FILE * test = fopen("/tmp/facturation/data/null", "a");
    int i = 0;
    while (i != dictionary->count)
    {
        fputs(dictionary->entries[i].name, test);
        fputs("\n", test);
        i++;
    }

    fputs(buffer, test);
    fclose(test);

    if (entry == NULL)
    {
        fatalError("Entry not found !");
    }

    charaEqual = findCharInString(copyFormat, '=');
    copyStringWithLength(buffer, charaAccolade+1, stringLength(charaAccolade) - stringLength(charaEqual));

    if (entry->type == STRING_ENTRY)
    {
        copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) + 1);
        free(stringFormatedFinal);
        copyOfFormat = formatString(duplicateString(entry->value.stringValue), buffer, charaEqual);
        stringFormatedFinal = concatenateString(stringFormated, copyOfFormat);
        setCursorEndOfModele(copyFormat);

        if (copyFormat[0] == ',')
            stringFormatedFinal = formatSecondParameterString(charaEqual, copyFormat, buffer, stringFormated, stringFormatedFinal);
    }
    else if (entry->type == NUMBER_ENTRY)
    {
        sprintf (bufferForNumberEntry, "%f", entry->value.numberValue);
        copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) + 1);
        free(stringFormatedFinal);
        copyOfFormat = formatNumber(duplicateString(bufferForNumberEntry), buffer, charaEqual);
        stringFormatedFinal = concatenateString(stringFormated, copyOfFormat);
        setCursorEndOfModele(copyFormat);

        if (copyFormat[0] == ',')
            stringFormatedFinal = formatSecondParameterNumber(charaEqual, copyFormat, buffer, stringFormated, stringFormatedFinal);
    }
    else
        return NULL;

    free(copyOfFormat);
    return stringFormatedFinal;
}

/**Function format the string
 * @param copyValueStringEntry a copy of string value entry
 * @param buffer a buffer
 * @param charaEqual a string contain equal charactere
 * @return a pointer to the string formated
*/

char * formatString(char * copyValueStringEntry, char * buffer, char * charaEqual)
{
    char * charTemp = (char*)malloc(sizeof(char) * stringLength(copyValueStringEntry) + 1);

    if (charTemp == NULL)
                fatalError("error malloc : Attribution of charTemp on the heap failed");

    copyStringWithLength(charTemp, copyValueStringEntry, stringLength(copyValueStringEntry) + 1);

    if (icaseCompareString(buffer, "case") == 0)
    {
        if (charaEqual[1] >= 'A' && charaEqual[1] <= 'Z')
            makeUpperCaseString(copyValueStringEntry);
        else if (charaEqual[1] >= 'a' && charaEqual[1] <= 'z')
            makeLowerCaseString(copyValueStringEntry);
    }
    else if (icaseCompareString(buffer, "min") == 0)
    {
        while (atoi(valueAfterEqual(buffer, charaEqual)) > (int)stringLength(copyValueStringEntry))
        {
            free(copyValueStringEntry);
            copyValueStringEntry = insertString(charTemp, (int)stringLength(charTemp), " ", 1);
            charTemp = realloc(charTemp, stringLength(copyValueStringEntry) + 2);
            copyStringWithLength(charTemp, copyValueStringEntry, stringLength(copyValueStringEntry) +1);
        }
    }
    else if (icaseCompareString(buffer, "max") == 0)
    {
        copyStringWithLength(copyValueStringEntry, copyValueStringEntry, (size_t)atoi(valueAfterEqual(buffer, charaEqual))+1);
    }

    free(charTemp);
    return copyValueStringEntry;
}

/**Function format the number
 * @param result a copy of number value entry
 * @param buffer a buffer
 * @param charaEqual a string contain equal charactere
 * @return a pointer to the string formated
*/

char * formatNumber(char * result, char * buffer, char * charaEqual)
{
    double doubleOfCopy = 0.0;
    int valueAfterEqualInt = 0;
    char copyOfResult[1024] = "";
    char * parameterOfSprintf = NULL;

    memset(copyOfResult, '\0', sizeof(char) * 1024);

    if (icaseCompareString(buffer, "precision") == 0)
    {
        doubleOfCopy = atof(result);
        buffer = valueAfterEqual(buffer, charaEqual);
        parameterOfSprintf = insertString("%.f", 2, buffer, (int)stringLength(buffer));
        sprintf (result, parameterOfSprintf, doubleOfCopy);
    }
    else if (icaseCompareString(buffer, "min") == 0)
    {
        valueAfterEqualInt = atoi(valueAfterEqual(buffer, charaEqual));

        while (valueAfterEqualInt > (int)stringLength(result))
        {
            copyStringWithLength(copyOfResult, result, stringLength(result) + 1);
            free(result);
            result = insertString(copyOfResult, 0, " ", 1);
        }
    }
    free(parameterOfSprintf);
    return result;
}


/**Function search and return the value after equal charactere
 * @param buffer a buffer
 * @param charaEqual a string contain equal charactere
 * @return the string contain the number after equal charactere
*/

char * valueAfterEqual(char * buffer, char * charaEqual)
{
    int i = 1;
    memset(buffer, '\0', sizeof(char) * 1024);

    while (charaEqual[i] >= '0' && charaEqual[i] <= '9')
    {
        buffer[i-1] = charaEqual[i];
        i++;
    }
    return buffer;
}


/** Function search a charactere in a string.
 * @param str a string
 * @param c charactere to find
 * @return a pointer to the charactere found
*/

char * findCharInString(char * str, char c)
{
    int i = 0;
    char * resultat = NULL;

    while (str[i] != '\0' && resultat == NULL)
    {
        if (str[i] == c)
        {
            resultat = &str[i];
        }
        i++;
    }
    return resultat;
}

/**Function format the string a second time
 * @param charaEqual a string contain a equal charactere
 * @param copyFormat a string contain a copy of format
 * @param buffer a buffer
 * @param stringFormated a temporary string
 * @param stringFormatedFinal the future returned value
 * @return a pointer to the new string formated
*/

char * formatSecondParameterString(char * charaEqual, char * copyFormat, char * buffer, char * stringFormated, char * stringFormatedFinal)
{
    charaEqual = findCharInString(copyFormat, '=');
    memset(buffer, '\0', sizeof(char) * 1024);
    copyStringWithLength(buffer, copyFormat+1, stringLength(copyFormat+1) - stringLength(charaEqual) + 1);
    copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) + 1);
    free(stringFormatedFinal);
    stringFormatedFinal = formatString(duplicateString(stringFormated), buffer, charaEqual);
    setCursorEndOfModele(copyFormat);
    return stringFormatedFinal;
}

/**Function format the string a second time
 * @param charaEqual a string contain a equal charactere
 * @param copyFormat a string contain a copy of format
 * @param buffer a buffer
 * @param stringFormated a temporary string
 * @param stringFormatedFinal the future returned value
 * @return a pointer to the new string formated
*/

char * formatSecondParameterNumber(char * charaEqual, char * copyFormat, char * buffer, char * stringFormated, char * stringFormatedFinal)
{
    charaEqual = findCharInString(copyFormat, '=');
    memset(buffer, '\0', sizeof(char) * 1024);
    copyStringWithLength(buffer, copyFormat+1, stringLength(copyFormat+1) - stringLength(charaEqual) + 1);
    copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) + 1);
    free(stringFormatedFinal);
    stringFormatedFinal = formatNumber(duplicateString(stringFormated), buffer, charaEqual);
    setCursorEndOfModele(copyFormat);
    return stringFormatedFinal;
}


/** Function is called when model haven't parameter
 * @param dictionary a dictionary
 * @param copyFormat a string contain a copy of format
 * @param buffer a buffer
 * @return a pointer to the new string formated
*/

char * parameterNotFound(Dictionary * dictionary, char * copyFormat)
{
    DictionaryEntry * entry;
    char * result = NULL;
    char buffer[1024] = "";

    memset(buffer, '\0', sizeof(char) * 1024);
    copyStringWithLength(buffer, copyFormat, stringLength(copyFormat) - stringLength(findCharInString(copyFormat, '%')) + 1);
    entry = Dictionary_getEntry(dictionary, buffer);

    if (entry != NULL)
    {
        if (entry->type == STRING_ENTRY)
            result = duplicateString(entry->value.stringValue);
        else if (entry->type == NUMBER_ENTRY)
        {
            memset(buffer, '\0', sizeof(char) * 1024);
            sprintf (buffer, "%.6f", entry->value.numberValue);
            result = duplicateString(buffer);
        }
        else
            result = duplicateString("");
    }
    else
        result = duplicateString("");
    return result;
}

/**Function cut a part of string formated (model one)
 * @param copyFormat a string contain a copy of format
 * @return a pointer to the new string copyFormat
*/

void setCursorEndOfModele(char * copyFormat)
{
    size_t i = 1;

    while (copyFormat[i] != ',' && copyFormat[i] != '%')
        i++;

    copyStringWithLength(copyFormat, copyFormat+i, stringLength(copyFormat) - i + 1);
}


