
//System includes
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#ifdef _WIN32
#include <Windows.h>
#else
#include <dirent.h>
#endif

//Local includes
#include "DirectoryContents.h"

using namespace std;

cDirectoryContents::cDirectoryContents(const string& strDirectoryPath, const string& strMask, bool bRecursive) :
    m_strDirectoryPath(strDirectoryPath),
    m_strMask(strMask),
    m_bRecursive(bRecursive)
{
    populateFileList();
}

cDirectoryContents::~cDirectoryContents()
{
}

void cDirectoryContents::populateFileList()
{
    //Empty the list if it isn't empty

    if(m_vstrFilenames.size())
    {
        m_vstrFilenames.clear();
    }

    searchPathForFiles(m_strDirectoryPath);

    //Sort the file names alphabetically and return vector
    std::sort(m_vstrFilenames.begin(), m_vstrFilenames.end());
}


void cDirectoryContents::searchPathForFiles(string strDirPath)
{
#ifdef _WIN32
    //This msvc code is currently untested. And doesn't support recursion

    HANDLE hFind;
    WIN32_FIND_DATA FindFileData;

    string strFindString = strDirPath;
    strFindString.append("\\*")

            //Add the mask to the search if it is specified
            if(m_strMask.length())
    {
        strFindString.append(strMask);
        strFindString.append("*");
    }


    if((hFind = FindFirstFile(strFindString.c_str(), &FindFileData)) == INVALID_HANDLE_VALUE)
    {
        throw cDirectoryContentsException("Unable to open specified directory.")
    }

    do
    {
        //Prepend the directory name to the filename.

        string strFullFilePath = strDirPath;
        if(strFullFilePath[strFullFilePath.length()-1] != '\\')
            strFullFilePath.append("\\");

        //Add the next file name in the directory

        strFullFilePath.append(FindFileData.cFileName);

        //Push the complete file path into the string list

        m_vstrFilenames.push_back(strFullFilePath);
    }
    while(FindNextFile(hFind, &FindFileData));
    FindClose(hFind);
#else

    DIR *dir;
    if((dir = opendir(strDirPath.c_str())) == NULL)
    {
        string strError = "Unable to open specified directory: \"";
        strError.append(strDirPath).append("\"");

        throw cDirectoryContentsException(strError);
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        //Leave out current dir handle
        if(strcmp(entry->d_name, ".") == 0)
            continue;

        //and parent dir handle
        if(strcmp(entry->d_name, "..") == 0)
            continue;

        //check for mask match if it exists
        if(strstr(entry->d_name, m_strMask.c_str()) != 0 || (!(bool)m_strMask.length()))
        {
            //Prepend the directory path
            string strFullFilename = strDirPath;
            if(strFullFilename[strFullFilename.length()-1] != '/')
                strFullFilename.append("/");

            //Add the next filename and add to the vector
            strFullFilename.append(entry->d_name);
            m_vstrFilenames.push_back(strFullFilename);
        }

        //Recurse into subdirs if option is enabled
        if(entry->d_type == 4 && m_bRecursive)
        {
            //Prepend the directory path
            string strSubDirPath = strDirPath;
            if(strSubDirPath[strSubDirPath.length()-1] != '/')
                strSubDirPath.append("/");

            //Add the subdirectory name and recursive decend into it
            strSubDirPath.append(entry->d_name);
            searchPathForFiles(strSubDirPath);
        }

    }
    closedir(dir);

#endif
}

vector<string> cDirectoryContents::getContents() const
{
    return m_vstrFilenames;
}

string cDirectoryContents::getPath() const
{
    return m_strDirectoryPath;
}

string cDirectoryContents::getMask() const
{
    return m_strMask;
}

bool cDirectoryContents::isRecursive() const
{
    return m_bRecursive;
}


