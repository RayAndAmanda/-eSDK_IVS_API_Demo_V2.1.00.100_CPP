#include "stdafx.h"
static int max_log_file_size = 10 * 1024 * 1024;
static char* sipplay_log_file = "D:\\gqylog.txt";
static FILE* fp = NULL;
static char* getCurTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	return asctime(timeinfo);
}
static CString gethmtime()
{
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	//strDate.Format(_T("%4d-%2d-%2d"),st.wYear,st.wMonth,st.wDay);
	strTime.Format(_T("%2d:%2d:%2d:%2d"),st.wHour,st.wMinute,st.wSecond,st.wMilliseconds) ;
	return strTime;
}
static void InsertGQYLog(char* content, int type)//group的作用是为LOG分组
{
	if (fp == NULL)
	{
		fp = fopen(sipplay_log_file, "a+");
	}
	if (fp)
	{
		if (ftell(fp) > max_log_file_size)
		{
			fclose(fp);
			fp = fopen(sipplay_log_file, "w");
		}
		if (fp)
		{
			CString temp;
			switch(type)
			{
			 case 0:
					temp.Format("%s(%s):%s\n",gethmtime(),_T("INFO"),content);
					break;
			case 1:
                    temp.Format("%s(%s):%s\n",gethmtime(),_T("WARNING"),content);
					break;
			case 2:
					temp.Format("%s(%s):%s\n",gethmtime(),_T("ERROR"),content);
					break;		
			}
			fwrite(temp.GetBuffer(), 1, temp.GetLength(), fp);		
			fflush(fp);
		}

	}
	
}


void GqyInsertLog(int logType, char* format, ...)
{
	
	va_list args;
	va_start(args, format);
	INT len = _vscprintf(format, args) + 1;
	char* buffer = (char*)malloc(len * sizeof(char));
	vsprintf_s(buffer, len * sizeof(char), format, args);
	InsertGQYLog(buffer, logType);
	free(buffer);
	va_end(args);
}
