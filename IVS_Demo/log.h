#pragma once
enum GQY_LOG_TYPE
{
	GQY_LOG_INFO=0,
	GQY_LOG_WARNING=1,
	GQY_LOG_ERROR=2,
};
void GqyInsertLog(int logType, char* format, ...);