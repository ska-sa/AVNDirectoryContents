
#ifndef _DIRECTORY_CONTENTS_
#define _DIRECTORY_CONTENTS_

//System includes
#include <string>
#include <vector>

struct cDirectoryContentsException : public std::exception
{
    //Strictly speaking using a std::string is supposed to be incorrect because it could throw and exception itself.
    //This is unlike ever to have so we will use it for now.

    std::string m_strMessage;

    cDirectoryContentsException(std::string strMessage) : m_strMessage(strMessage) {}
    ~cDirectoryContentsException() throw() {}

    virtual const char* what() const throw()
    {
        return m_strMessage.c_str();
    }
};

class cDirectoryContents
{
	public:
		cDirectoryContents(const std::string& strDirectoryPath, const std::string& strMask = "", bool bRecursive = false);
		~cDirectoryContents();

		std::vector<std::string> getContents() const;
		std::string getPath() const;
		std::string getMask() const;
		bool isRecursive() const;
	private:
		std::string m_strDirectoryPath;
		std::string m_strMask;
		bool m_bRecursive;
		std::vector<std::string> m_vstrFilenames;

		void populateFileList();
		void searchPathForFiles(std::string strDirPath);

};

#endif //_DIRECTORY_CONTENTS_
