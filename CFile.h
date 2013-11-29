/*

#ifndef _C_FILE_H
#define _C_FILE_H
#include<fstream>

class CMyFile
{
public:
	void operator >>(DWORD );
	void Open(wstring & wstr,);
	void Close(wstring & wstr);
protected:
	wstring m_wstrFileName;
	fstream  m_File;
};


void CMyFile::Open(wstring & wsr)
{
	m_wstrFileName = wsr;
	m_File.open( m_wstrFileName .c_str() , fstream::in | fstream::out);	
}
void CMyFile::Close(wstring & wsr)
{
	Close()
}
#endif*/