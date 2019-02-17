#include "regkey.h"

VOID QueryKey(HKEY hKey)
{
    CHAR     achKey[MAX_PATH];
    CHAR     achClass[MAX_PATH] = {0};  // buffer for class name
    DWORD    cchClassName = MAX_PATH;  // length of class string
    DWORD    cSubKeys;                 // number of subkeys
    DWORD    cbMaxSubKey;              // longest subkey size
    DWORD    cchMaxClass;              // longest class string
    DWORD    cValues;              // number of values for key
    DWORD    cchMaxValue;          // longest value name
    DWORD    cbMaxValueData;       // longest value data
    DWORD    cbSecurityDescriptor; // size of security descriptor
    FILETIME ftLastWriteTime;      // last write time

    DWORD i, j;
    DWORD retCode, retValue;

    CHAR  achValue[MAX_VALUE_NAME] = {0};
    DWORD cchValue = MAX_VALUE_NAME;
    CHAR  achBuff[80] = {0};

    // Get the class name and the value count.
    RegQueryInfoKey(hKey,        // key handle
        (LPWSTR)achClass,                // buffer for class name
        &cchClassName,           // length of class string
        NULL,                    // reserved
        &cSubKeys,               // number of subkeys
        &cbMaxSubKey,            // longest subkey size
        &cchMaxClass,            // longest class string
        &cValues,                // number of values for this key
        &cchMaxValue,            // longest value name
        &cbMaxValueData,         // longest value data
        &cbSecurityDescriptor,   // security descriptor
        &ftLastWriteTime);       // last write time



    // Enumerate the child keys, until RegEnumKeyEx fails. Then
    // get the name of each child key and copy it into the list box.

    for (i = 0, retCode = ERROR_SUCCESS;
    retCode == ERROR_SUCCESS; i++)
    {
        DWORD cName = MAX_PATH;
        retCode = RegEnumKeyEx(hKey,
            i,
            (LPWSTR)achKey,
            &cName,
            NULL,
            NULL,
            NULL,
            &ftLastWriteTime);
        if (retCode == (DWORD)ERROR_SUCCESS)
        {
            qDebug() << QString::fromWCharArray((wchar_t*)achKey) << "*****";
        }
    }

    if (cValues)
    {
        for (j = 0, retValue = ERROR_SUCCESS;
        j < cValues; j++)
        {
            cchValue = MAX_VALUE_NAME;
            achValue[0] = '\0';
            retValue = RegEnumValue(hKey,
                                    j,
                                    (LPWSTR)achValue,
                &cchValue,
                NULL,
                NULL,    // &dwType,
                NULL,    // &bData,
                NULL);   // &bcData

            if (retValue == (DWORD)ERROR_SUCCESS)
            {
                //wsprintf(achBuff,
                //    "Line:%d 0 based index = %d, retValue = %d, "
                //    "ValueLen = %d",
                //    __LINE__, j, retValue, cchValue);
                qDebug() << achKey;
            }

            achBuff[0] = '\0';

            // Add each value to a list box.

        }


    }
}
