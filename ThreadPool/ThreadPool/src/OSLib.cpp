#include "OSLib.h"

#ifdef OS_WINDOWS

void lib_get_float32(unsigned char *pbuff, float *out)
{
	unsigned char *ptr = (unsigned char *) out;

	*ptr++ = *pbuff++;
	*ptr++ = *pbuff++;
	*ptr++ = *pbuff++;
	*ptr++ = *pbuff++;
}

//process

int lib_process_create(PROS_HANDLE *phandle, char *pfilename, char *pcmdline)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	if (CreateProcess(NULL/*pfilename*/, pcmdline,
	NULL,
	NULL,
	FALSE, 0,
	NULL,
	NULL, &si, &pi))
	{
		*phandle = pi.hProcess;
		return 0;
	}
	else
	{
		return -1;
	}
}

int lib_process_destroy(PROS_HANDLE handle, int iSignal)
{
	if (NULL == handle)
	{
		return -1;
	}

	if (CloseHandle(handle))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int lib_processV2_create(PROS_HANDLE *phandle, void (*pfun)(char *pname), char *pname)
{
	return 0;
}

//thread

//0 success; -1 error
int lib_task_create(char *pname, int priority, int stackSize, TASK_HANDLE *phandle, LIB_TASK_ROUTINE routine, void *param)
{
	unsigned int dwId;
	TASK_HANDLE h;

	h = (TASK_HANDLE) _beginthreadex(0, 0, routine, param, 0, &dwId);

	if (h == (TASK_HANDLE) -1)
		return -1;

	if (phandle)
	{
		*phandle = h;
		return 0;
	}
	else
	{
		CloseHandle(h);
		return -1;
	}
}

//0 success; -1 error
int lib_task_detach(TASK_HANDLE phandle)
{
	CloseHandle(phandle);
	return 0;
}

//0 success; -1 error
int lib_task_join(TASK_HANDLE phandle, void **pretcode)
{
	if (WaitForSingleObject(phandle, INFINITE) == WAIT_OBJECT_0)
	{
		if (pretcode)
			GetExitCodeThread(phandle, (unsigned long*) pretcode);

		return 0;
	}

	return -1;
}

void lib_task_exit(unsigned int retval)
{
	_endthreadex(retval);
}

//mutex

//0 success; other error
int lib_mutex_init(HMUTEX *mutex)
{
	InitializeCriticalSection(mutex);
	return 0;
}

//0 success; other error
int lib_mutex_lock(HMUTEX *mutex)
{
	EnterCriticalSection(mutex);
	return 0;
}

//0 success; other error
int lib_mutex_unlock(HMUTEX *mutex)
{
	LeaveCriticalSection(mutex);
	return 0;
}

//0 success; other error
int lib_mutex_destroy(HMUTEX *mutex)
{
	DeleteCriticalSection(mutex);
	return 0;
}

//cond

//0 success; other error
int lib_cond_init(HCOND *cond)
{
	return 0;
}

//0 success; other error
int lib_cond_wait(HMUTEX *mutex, HCOND *cond, int milliseconds)
{
	return 0;
}

//0 success; other error
int lib_cond_signal(HCOND *cond)
{
	return 0;
}

//0 success; other error
int lib_cond_broadcast(HCOND *cond)
{
	return 0;
}

//0 success; other error
int lib_cond_destroy(HCOND *cond)
{
	return 0;
}

//no name semaphore

//0 success; -1 error
int lib_noname_sem_create(HNSEM *sem, unsigned int value)
{
	*sem = CreateSemaphore(0, value, 0x7FFFFFFF, NULL);
	if (*sem == NULL)
		return -1;

	return 0;
}

//0 success; -1 error
int lib_noname_sem_destroy(HNSEM *sem)
{
	if (CloseHandle(*sem))
		return 0;
	return -1;
}

//0 success; -1 error
int lib_noname_sem_wait(HNSEM *sem)
{
	int ret = WaitForSingleObject(*sem, INFINITE);

	if (ret == WAIT_OBJECT_0)
		return 0;
	else
		return -1;
}

//0 success; -1 error
int lib_noname_sem_post(HNSEM *sem)
{
	if (ReleaseSemaphore(*sem, 1, 0))
		return 0;

	return -1;
}

//0 success; -1 error
int lib_noname_sem_getvalue(HNSEM *sem)
{
	/*
	 long value=0;

	 if(ReleaseSemaphore(*sem, 0, &value))
	 return -1;
	 else
	 return value;
	 */
	return 0;
}

//name semaphore

//-1失败; 0成功
int lib_name_sem_create(char *pfilename, unsigned int max_val, unsigned int init_val, HSEM *phandle)
{
	HSEM hSemObj = NULL;

	if ((NULL == pfilename) || (NULL == phandle))
	{
		return -1;
	}

	hSemObj = CreateSemaphore(NULL, init_val, max_val, pfilename);
	if (NULL == hSemObj)
	{
		return -1;
	}

	*phandle = hSemObj;

	return 0;
}

//-1失败; 0成功
int lib_name_sem_open(char *pfilename, HSEM *phandle)
{
	HSEM hSemObj = NULL;

	if ((NULL == pfilename) || (NULL == phandle))
	{
		return -1;
	}

	hSemObj = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, pfilename);
	if (NULL == hSemObj)
	{
		return -1;
	}
	*phandle = hSemObj;

	return 0;
}

//0 success; -1 error
int lib_name_sem_close(HSEM hSemObj)
{
	if (NULL == hSemObj)
	{
		return -1;
	}

	if (CloseHandle(hSemObj))
		return 0;

	return -1;
}

//0 success; -1 error
int lib_name_sem_destroy(HSEM hSemObj)
{
	if (NULL == hSemObj)
	{
		return -1;
	}

	if (CloseHandle(hSemObj))
		return 0;

	return -1;
}

//0 success; -1 error
int lib_name_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds)
{
	int ret = 0;

	if (NULL == hSemObj)
	{
		return -1;
	}

	ret = WaitForSingleObject(hSemObj, dwMilliseconds);

	if (ret == WAIT_OBJECT_0)
		return 0;

	return -1;
}

//0 success; -1 error
int lib_beat_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds)
{
	int ret = 0;

	if (NULL == hSemObj)
	{
		return -1;
	}

	ret = WaitForSingleObject(hSemObj, dwMilliseconds);

	if (ret == WAIT_OBJECT_0)
		return 0;

	return -1;
}

//0 success; -1 error
int lib_name_sem_post(HSEM hSemObj)
{
	if (NULL == hSemObj)
	{
		return -1;
	}

	if (ReleaseSemaphore(hSemObj, 1, NULL))
		return 0;

	if (GetLastError() == 289)
	{
		return 1; //信号量已满
	}

	return -1;
}

//0 success; -1 error
int lib_beat_sem_post(HSEM hSemObj)
{
	if (NULL == hSemObj)
	{
		return -1;
	}

	if (ReleaseSemaphore(hSemObj, 1, NULL))
		return 0;

	if (GetLastError() == 289)
	{
		return 1; //信号量已满
	}

	return -1;
}

//0 success; -1 error，windows下不能获取当前信号量的值
int lib_name_sem_getvalue(HSEM hSemObj)
{
	return 0;
}

//-1失败，0成功
int lib_shm_create(char *pfilename, unsigned int max_size, SHM_HANDLE *phandle)
{
	SHM_HANDLE hMapObj;

	if ((NULL == pfilename) || (max_size <= 0) || (NULL == phandle))
	{
		return -1;
	}

	hMapObj = CreateFileMapping((HANDLE) 0xFFFFFFFF, NULL, PAGE_READWRITE, 0, max_size, pfilename);
	if (hMapObj == NULL)
	{
		return -1;
	}

	*phandle = hMapObj;
	return 0;
}

//-1失败，0成功
int lib_shm_open(char *pfilename, SHM_HANDLE *phandle)
{
	SHM_HANDLE hMapObj;

	if ((NULL == pfilename) || (NULL == phandle))
	{
		return -1;
	}

	hMapObj = OpenFileMapping(FILE_MAP_WRITE, FALSE, pfilename);
	if (hMapObj == NULL)
	{
		return -1;
	}

	*phandle = hMapObj;
	return 0;
}

//NULL失败，其它成功
void* lib_shm_map(SHM_HANDLE hShmObj)
{
	void *pMapAddr;

	if (NULL == hShmObj)
	{
		return NULL;
	}

	pMapAddr = MapViewOfFile(hShmObj, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

	return pMapAddr;
}

int lib_shm_unmap(void *hMapAddr)
{
	if (NULL == hMapAddr)
	{
		return -1;
	}

	if (UnmapViewOfFile(hMapAddr))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int lib_shm_close(SHM_HANDLE hMapObj)
{
	if (NULL == hMapObj)
	{
		return -1;
	}

	if (CloseHandle(hMapObj))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int lib_shm_destroy(SHM_HANDLE hMapObj)
{
	if (NULL == hMapObj)
	{
		return -1;
	}

	if (CloseHandle(hMapObj))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

//dynamic linking loader

//0 success; -1 error
HANDLE lib_load_library(const char *filename)
{
	HANDLE handle;

	if (filename == NULL)
	{
		return NULL;
	}

	handle = LoadLibrary(filename);

	return handle;
}

//0 success; -1 error
void *lib_get_addr(HANDLE handle, const char *module)
{
	void *ptr = NULL;

	if (handle == NULL || module == NULL)
	{
		return NULL;
	}

	ptr = (void *) GetProcAddress((HINSTANCE) handle, module);

	return ptr;
}

//0 success; -1 error
int lib_free_library(HANDLE handle)
{
	if (handle == NULL)
	{
		return -1;
	}

	FreeLibrary((HINSTANCE) handle);

	return 0;
}

//NULL无错误,否则有错误
char *lib_get_liberror()
{
	return NULL;
}

//some useful functions

//0 success; other error
int lib_sleep(unsigned int milliseconds)
{
	Sleep(milliseconds);
	return 0;
}

//0 success; -1 error
int lib_getlocaltime(lib_systime *psystime)
{
	GetLocalTime(psystime);
	return 0;
}

//0 success; -1 error
int lib_setlocaltime(const lib_systime *psystime)
{
	if (SetLocalTime(psystime))
		return 0;

	return -1;
}

int lib_getcurrenttime(time_t *pSeconds, unsigned short *pMilliSeconds)
{
	struct _timeb timebuffer;

	if (NULL == pSeconds || NULL == pMilliSeconds)
	{
		return -1;
	}

	_ftime(&timebuffer);
	*pSeconds = timebuffer.time;
	*pMilliSeconds = timebuffer.millitm;

	return 0;
}

int lib_time_init(lib_time_ms *ptTime)
{
	if (NULL == ptTime)
	{
		return -1;
	}

	//初始化参数
	ptTime->lastCheckTime = 0;
	ptTime->wrapCount = 0;
	ptTime->ticksPerSec = 0;
	ptTime->initMsTime = _lib_time_get(ptTime);

	return 0;
}

double _lib_time_get(lib_time_ms *ptTime)
{
	double totalMsTime = 0.0;
	unsigned int milliSeconds = 0;

	if (NULL == ptTime)
	{
		return totalMsTime;
	}

	//获取毫秒计数
	milliSeconds = GetTickCount();

	//如果时间低位越界，则进1到高位
	if (milliSeconds < ptTime->lastCheckTime)
	{
		ptTime->wrapCount += 1;
	}

	//根据高位和低位毫秒数，计算总毫秒
	totalMsTime = (double) ((((__int64) ptTime->wrapCount) << 32) + (__int64) milliSeconds);
	ptTime->lastCheckTime = milliSeconds;/* save for next check	*/

	return totalMsTime;
}

//0 success; -1 error
int lib_create_dir(const char *pdir)
{
	char *psrc = NULL;
	char *pdest = NULL;
	char tmpPath[32];
	char fulPath[256];
	int ret = 0;

	if (NULL == pdir || strlen(pdir) <= 1)
	{
		return -1;
	}

	psrc = (char*) pdir;
	memset(fulPath, 0, 256);
	while (1)
	{
		memset(tmpPath, 0, 32);
		pdest = tmpPath;
		while (*psrc != 0 && *psrc != '\\')
		{
			*pdest++ = *psrc++;
		}

		if (strlen(tmpPath) > 0)
		{
			strcpy(fulPath + strlen(fulPath), tmpPath);
			if (lib_pathfile_exist(fulPath) != 0)
			{
				ret = _mkdir(fulPath);
			}
		}

		if (0 == *psrc || ret != 0)
		{
			break; //遍历结束或创建目录失败
		}
		else
		{
			//*psrc=='/'
			fulPath[strlen(fulPath)] = '\\';
			psrc++;
		}
	}

	return ret;
}

//0 yes; -1 no
int lib_pathfile_exist(const char *pathfile)
{
	if (_access(pathfile, 0) != 0)
		return -1;

	return 0;

//	if(PathFileExists(pathfile))
//		return 0;
//	else
//		return -1;
}

//0 yes; -1 no
int lib_pathfile_exec(const char *pathfile)
{
	if (_access(pathfile, 2) != 0)
		return -1;

	return 0;
}

//0 yes; -1 no
int lib_pathfile_rw(const char *pathfile)
{
	if (_access(pathfile, 6) != 0)
		return -1;

	return 0;
}

int lib_get_file_size(const char *file_name)
{
	struct _stat file_desc;

	if (_stat(file_name, &file_desc) == 0)
	{
		return file_desc.st_size;
	}

	return -1;
}

//>=0 实际文件数目; -1 error
int lib_get_filenum_of_dir(const char *pathfile)
{
	int total_num = 0;
	WIN32_FIND_DATA find_data;
	HANDLE hFind;
	char dir_name[SHR_MAX_FILE_SIZE];

	if (NULL == pathfile)
	{
		return -1;
	}

	sprintf(dir_name, "%s\\*.*", pathfile);
	hFind = FindFirstFile(dir_name, &find_data);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return -1;
	}

	do
	{
		if (find_data.cFileName[0] != '.')
		{
			total_num++;
		}

	} while (FindNextFile(hFind, &find_data));

	FindClose(hFind);

	return total_num;
}

//>=0 实际文件数目; -1 error
int lib_get_filelist_of_dir(const char *pathfile, SHR_FILE_ATTRI *pfilelist, int maxnum)
{
	int total_num = 0;
	WIN32_FIND_DATA find_data;
	HANDLE hFind;
	char dir_name[SHR_MAX_FILE_SIZE];
	lib_systime tTime;

	if (NULL == pathfile || NULL == pfilelist || maxnum <= 0)
	{
		return -1;
	}

	sprintf(dir_name, "%s\\*.*", pathfile);
	hFind = FindFirstFile(dir_name, &find_data);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return -1;
	}

	do
	{
		if ((FILE_ATTRIBUTE_DIRECTORY & find_data.dwFileAttributes) == FILE_ATTRIBUTE_DIRECTORY)
		{
			if ('.' != find_data.cFileName[0])
			{
				FileTimeToLocalFileTime(&find_data.ftLastWriteTime, &find_data.ftLastWriteTime);
				FileTimeToSystemTime(&find_data.ftLastWriteTime, &tTime);
				pfilelist[total_num].last_modified = shrtime_to_timet(&tTime);
				pfilelist[total_num].size = find_data.nFileSizeLow;
				strcpy(pfilelist[total_num].name, find_data.cFileName);
				strcat(pfilelist[total_num].name, "\\");
				total_num++;
			}
		}
		else
		{
			FileTimeToLocalFileTime(&find_data.ftLastWriteTime, &find_data.ftLastWriteTime);
			FileTimeToSystemTime(&find_data.ftLastWriteTime, &tTime);
			pfilelist[total_num].last_modified = shrtime_to_timet(&tTime);
			pfilelist[total_num].size = find_data.nFileSizeLow;
			strcpy(pfilelist[total_num].name, find_data.cFileName);
			total_num++;
		}

		if (total_num >= maxnum)
		{
			break; //超过读取的最大数目
		}
	} while (FindNextFile(hFind, &find_data));

	FindClose(hFind);

	return total_num;
}

//非NULL成功；NULL失败
HRSPORT lib_rsport_open(unsigned int nr)
{
	HRSPORT hCom = 0;
	char port_name[32];

	if (nr <= 0)
	{
		return 0;
	}

	sprintf(port_name, "COM%d", nr);
	hCom = CreateFile(port_name,
	GENERIC_READ | GENERIC_WRITE, 0,
	NULL,
	OPEN_EXISTING,
	FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hCom)
	{
		hCom = 0;
	}

	return hCom;
}

//0成功；非0失败。
int lib_rsport_close(HRSPORT rsport)
{
	if (NULL == rsport)
	{
		return -1;
	}

	if (CloseHandle(rsport))
		return 0;
	else
		return -1;
}

const int baudratemap[] =
{
CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600,
CBR_19200, CBR_38400, CBR_57600, CBR_115200, };

const byte paritymap[] =
{
NOPARITY, EVENPARITY, ODDPARITY, SPACEPARITY, };

const byte databit[] =
{ 5, 6, 7, 8, };

const byte stopbit[] =
{
ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS, };

//0成功；非0失败。
int lib_rsport_setup(HRSPORT rsport, const rsport_attr* pattr)
{
	DCB dcb;

	if (NULL == rsport || NULL == pattr)
	{
		return -1;
	}

	GetCommState(rsport, &dcb);
	dcb.fBinary = pattr->fBinary;
	dcb.BaudRate = baudratemap[pattr->baudrate];
	dcb.fParity = (pattr->parity != rsparity_none);
	dcb.Parity = paritymap[pattr->parity];
	dcb.ByteSize = databit[pattr->databit];
	dcb.StopBits = stopbit[pattr->stopbit];

	if (SetCommState(rsport, &dcb))
		return 0;
	else
		return -1;
}

//0成功；非0失败。
int lib_rsport_set_timeout( HRSPORT rsport, unsigned int rxTimeout, unsigned int txTimeout)
{
	COMMTIMEOUTS cto;

	if (NULL == rsport)
	{
		return -1;
	}

	cto.ReadIntervalTimeout = 0;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = rxTimeout;
	cto.WriteTotalTimeoutConstant = txTimeout;
	cto.WriteTotalTimeoutMultiplier = 0;

	if (SetCommTimeouts(rsport, &cto))
		return 0;
	else
		return -1;
}

//0成功；非0失败。
int lib_rsport_write(HRSPORT rsport, const char* pbuff, int bytes_to_write)
{
	DWORD dwret;

	if (NULL == rsport || NULL == pbuff || bytes_to_write <= 0)
	{
		return -1;
	}

	if (!WriteFile(rsport, pbuff, (DWORD) bytes_to_write, &dwret, 0))
		return -1;

	return (int) dwret;
}

//0成功；非0失败。
int lib_rsport_read(HRSPORT rsport, char* pbuff, int bytes_to_read)
{
	DWORD dwret;

	if (NULL == rsport || NULL == pbuff || bytes_to_read <= 0)
	{
		return -1;
	}

	if (!ReadFile(rsport, pbuff, bytes_to_read, &dwret, 0))
		return -1;

	return (int) dwret;
}

//0成功；非0失败。
int lib_rsport_purge(HRSPORT rsport, int read, int write)
{
	int opt = 0;

	if (read)
		opt |= PURGE_RXCLEAR;
	if (write)
		opt |= PURGE_TXCLEAR;

	if (opt)
	{
		if (!PurgeComm(rsport, opt))
			return -1;
	}

	return 0;
}

#endif//OS_WINDOW

#ifdef OS_LINUX

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun
{
	int val; /* Value for SETVAL */
	struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* Array for GETALL, SETALL */
	struct seminfo *__buf; /* Buffer for IPC_INFO*/
};
#endif

void lib_get_float32(unsigned char *pbuff, float *out)
{
	unsigned char *ptr = (unsigned char *) out;

	*ptr++ = *pbuff++;
	*ptr++ = *pbuff++;
	*ptr++ = *pbuff++;
	*ptr++ = *pbuff++;
}

//process

int lib_process_create(PROS_HANDLE *phandle, char *pfilename, char *pcmdline)
{
	int i = 0;
	pid_t pid = 0;
	char *pTemp = NULL;
	char *argv[11] =
	{ NULL }; //最多允许pcmdline中有10个参数

	//把pcmdline中的参数解析到argv
	for (i = 0; i < 10, *pcmdline != 0; i++)
	{
		while (*pcmdline == 0x20)
		{
			pcmdline++; //除去参数i前可能的空格
		}

		if (*pcmdline == 0)
		{
			break; //命令行结束
		}

		//查找参数i
		pTemp = pcmdline;
		while (*pcmdline != 0 && *pcmdline != 0x20)
		{
			pcmdline++;
		}

		//查参数i结束
		if (*pcmdline == 0x20)
		{
			*pcmdline = 0;
			pcmdline++;
		}

		argv[i] = pTemp;
	}
	argv[i] = NULL;

	pid = fork();
	if (pid < 0)
	{
		return -1;
	}
	else if (0 == pid)
	{
		//child process
		if (execv((const char *) pfilename, (char * const *) argv) < 0)
		{
			exit(-1);
		}
	}
	else
	{
		//parent process
		*phandle = pid;
		return 0;
	}

	exit(0);
}

int lib_process_destroy(PROS_HANDLE handle, int iSignal)
{
	kill(handle, iSignal);	//杀掉所有子进程
	return 0;
}

int lib_processV2_create(PROS_HANDLE *phandle, void (*pfun)(char *pname), char *pname)
{
	pid_t pid = 0;

	pid = fork();
	if (pid < 0)
	{
		return -1;
	}
	else if (0 == pid)
	{
		//child process
		(*pfun)(pname);
		exit(0);
	}
	else
	{
		//parent process
		*phandle = pid;
		return 0;
	}

	return 0;
}

//thread

//0 success; -1 error
int lib_task_create(char *pname, int priority, int stackSize, TASK_HANDLE *phandle, LIB_TASK_ROUTINE routine, void *param)
{
	pthread_t t;

	if (pthread_create(&t, 0, routine, param) != 0)
		return -1;

	if (phandle)
	{
		*phandle = t;
		return 0;
	}
	else
	{
		pthread_detach(t);
		return -1;
	}
}

//0 success; -1 error
int lib_task_detach(TASK_HANDLE handle)
{
	return pthread_detach(handle);
}

//0 success; -1 error
int lib_task_join(TASK_HANDLE handle, void **pretcode)
{
	return pthread_join(handle, pretcode);
}

void lib_task_exit(unsigned int retval)
{
	pthread_exit((void *) retval);
}

//mutex

//0 success; other error
int lib_mutex_init(HMUTEX *mutex)
{
	pthread_mutexattr_t mt;

	pthread_mutexattr_init(&mt);
	pthread_mutexattr_settype(&mt, PTHREAD_MUTEX_RECURSIVE_NP);

	return pthread_mutex_init(mutex, &mt);
}

//0 success; other error
int lib_mutex_lock(HMUTEX *mutex)
{
	return pthread_mutex_lock(mutex);
}

//0 success; other error
int lib_mutex_unlock(HMUTEX *mutex)
{
	return pthread_mutex_unlock(mutex);
}

//0 success; other error
int lib_mutex_destroy(HMUTEX *mutex)
{
	return pthread_mutex_destroy(mutex);
}

//cond

//0 success; other error
int lib_cond_init(HCOND *cond)
{
	return pthread_cond_init(cond, 0);
}

//0 success; other error
int lib_cond_wait(HMUTEX *mutex, HCOND *cond, int milliseconds)
{
	int nRet = 0;

	if (milliseconds == -1)
	{
		nRet = pthread_cond_wait(cond, mutex);
	}
	else
	{
		struct timeval tv;
		struct timespec tm;

		gettimeofday(&tv, 0);

		tm.tv_sec = tv.tv_sec + milliseconds / 1000;
		tm.tv_nsec = (milliseconds % 1000) * 1000 + tv.tv_usec;
		tm.tv_sec += tm.tv_nsec / 1000000;
		tm.tv_nsec = (tm.tv_nsec % 1000000) * 1000;

		nRet = pthread_cond_timedwait(cond, mutex, &tm);
	}

	if (nRet != 0)
	{
		if (nRet == ETIMEDOUT)
		{
			nRet = -1;	//timeout
		}
		else
		{
			nRet = -2;	//error
		}
	}

	return nRet;
}

//0 success; other error
int lib_cond_signal(HCOND *cond)
{
	return pthread_cond_signal(cond);
}

//0 success; other error
int lib_cond_broadcast(HCOND *cond)
{
	return pthread_cond_signal(cond);
}

//0 success; other error
int lib_cond_destroy(HCOND *cond)
{
	return pthread_cond_destroy(cond);
}

//no name semaphore

//0 success; -1 error
int lib_noname_sem_create(HNSEM *sem, unsigned int value)
{
	return sem_init(sem, 0, value);
}

//0 success; -1 error
int lib_noname_sem_destroy(HNSEM *sem)
{
	return sem_destroy(sem);
}

//0 success; -1 error
int lib_noname_sem_wait(HNSEM *sem)
{
	return sem_wait(sem);
}

//0 success; -1 error
int lib_noname_sem_post(HNSEM *sem)
{
	return sem_post(sem);
}

//0 success; -1 error
int lib_noname_sem_getvalue(HNSEM *sem)
{
	int value = 0;

	sem_getvalue(sem, &value);
	return value;
}

//name semaphore

//0 success; -1 error
int lib_name_sem_create(char *pfilename, unsigned int max_val, unsigned int init_val, HSEM *phandle)
{
	HSEM hSemObj = NULL;
	key_t shmKey;
	union semun semopts;
	FILE *hFile = NULL;

	if (NULL == pfilename || NULL == phandle)
	{
		return -1;
	}

	if (lib_pathfile_exist(pfilename) != 0)
	{
		//文件不存在，则创建该文件。
		hFile = fopen(pfilename, "wb");
		if (NULL == hFile)
		{
			return -1;
		}
		fclose(hFile);
	}

	//获取IPC的key
	shmKey = ftok(pfilename, 2);
	if (shmKey == (key_t) - 1)
	{
		return -1;
	}

	//判断该信号量是否存在？
	hSemObj = semget(shmKey, 0, 0777);
	if (hSemObj != -1)
	{
		//存在，则删除
		if (lib_name_sem_destroy(hSemObj) == -1)
		{
			return -1;	//删除失败
		}
	}

	//创建信号量
	hSemObj = semget(shmKey, 1, IPC_CREAT | IPC_EXCL | 0777);
	if (-1 == hSemObj)
	{
		return -1;
	}

	//设置初始值
	semopts.val = init_val;
	if (semctl(hSemObj, 0, SETVAL, semopts) == -1)
	{
		return -1;
	}

	*phandle = hSemObj;

	return 0;
}

//-1失败; 0成功
int lib_name_sem_open(char *pfilename, HSEM *phandle)
{
	HSEM hSemObj = NULL;
	key_t shmKey;

	if ((NULL == pfilename) || (NULL == phandle))
	{
		return -1;
	}

	//获取IPC的key
	shmKey = ftok(pfilename, 2);
	if (shmKey == (key_t) - 1)
	{
		return -1;
	}

	hSemObj = semget(shmKey, 0, 0777);
	if (hSemObj == -1)
	{
		return -1;
	}

	*phandle = hSemObj;
	return 0;
}

//0 success; -1 error
int lib_name_sem_close(HSEM hSemObj)
{
	return 0;
}

//0 success; -1 error
int lib_name_sem_destroy(HSEM hSemObj)
{
	union semun semopts;

	if (semctl(hSemObj, 0, IPC_RMID, semopts) == -1)
		return -1;

	return 0;
}

//0 success; -1 error
int lib_name_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds)
{
	struct sembuf sem =
	{ 0, -1, IPC_NOWAIT | SEM_UNDO };
	unsigned int total = 0;

	if (INFINITE == dwMilliseconds)
	{
		//阻塞等待
		sem.sem_flg = SEM_UNDO;
		if (semop(hSemObj, &sem, 1) == 0)
		{
			return 0;	//成功
		}
		else
		{
			return -1;	//失败
		}
	}

	//非阻塞等待
	while (1)
	{
		if (semop(hSemObj, &sem, 1) == 0)
		{
			return 0;	//成功
		}

		if (total >= dwMilliseconds)
		{
			break;	//超时，退出
		}

		lib_sleep(5);	//睡眠5毫秒
		total += 5;
	}

	return -1;
}

//0 success; -1 error
int lib_beat_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds)
{
	struct sembuf sem =
	{ 0, -1, IPC_NOWAIT };
	unsigned int total = 0;

	//非阻塞等待
	while (1)
	{
		if (semop(hSemObj, &sem, 1) == 0)
		{
			return 0;	//成功
		}

		if (total >= dwMilliseconds)
		{
			break;	//超时，退出
		}

		lib_sleep(5);	//睡眠5毫秒
		total += 5;
	}

	return -1;
}

//0 success; -1 error
int lib_name_sem_post(HSEM hSemObj)
{
	struct sembuf sem =
	{ 0, 1, IPC_NOWAIT | SEM_UNDO };

	if (semop(hSemObj, &sem, 1) == -1)
		return -1;

	return 0;
}

//0 success; -1 error
int lib_beat_sem_post(HSEM hSemObj)
{
	union semun semopts;
	struct sembuf sem =
	{ 0, 1, IPC_NOWAIT };

	if (semctl(hSemObj, 0, GETVAL, semopts) > 0)
	{
		return 0;
	}

	if (semop(hSemObj, &sem, 1) == -1)
		return -1;

	return 0;
}

//0 success; -1 error
int lib_name_sem_getvalue(HSEM hSemObj)
{
	union semun semopts;
	return semctl(hSemObj, 0, GETVAL, semopts);
}

//0 success; -1 error
int lib_shm_create(char *pfilename, unsigned int max_size, SHM_HANDLE *phandle)
{
	SHM_HANDLE hMapObj;
	key_t shmKey;
	FILE *hFile = NULL;

	if ((NULL == pfilename) || (max_size <= 0) || (NULL == phandle))
	{
		return -1;
	}

	if (lib_pathfile_exist(pfilename) != 0)
	{
		//文件不存在，则创建该文件。
		hFile = fopen(pfilename, "wb");
		if (NULL == hFile)
		{
			return -1;
		}
		fclose(hFile);
	}

	shmKey = ftok(pfilename, 1);
	if (shmKey == (key_t) - 1)
	{
		return -1;
	}

	hMapObj = shmget(shmKey, 0, (0777));
	if (hMapObj != -1)
	{
		if (lib_shm_destroy(hMapObj) == -1)
		{
			return -1;	//删除失败
		}
	}

	hMapObj = shmget(shmKey, max_size, (IPC_CREAT | 0777));
	if (hMapObj == -1)
	{
		return -1;
	}

	*phandle = hMapObj;

	return 0;
}

//0 success; -1 error
int lib_shm_open(char *pfilename, SHM_HANDLE *phandle)
{
	SHM_HANDLE hMapObj;
	key_t shmKey;

	if ((NULL == pfilename) || (NULL == phandle))
	{
		return -1;
	}

	shmKey = ftok(pfilename, 1);
	if (shmKey == (key_t) - 1)
	{
		return -1;
	}

	hMapObj = shmget(shmKey, 0, 0777);
	if (hMapObj == -1)
	{
		return -1;
	}
	*phandle = hMapObj;

	return 0;
}

//NULL失败，其它成功
void* lib_shm_map(SHM_HANDLE hShmObj)
{
	void *pMapAddr;

	pMapAddr = shmat(hShmObj, NULL, 0);
	if (pMapAddr == (void *) -1)
	{
		return NULL;
	}

	return pMapAddr;
}

int lib_shm_unmap(void *hMapAddr)
{
	if (shmdt(hMapAddr) == 0)
		return 0;
	else
		return -1;
}

int lib_shm_close(SHM_HANDLE hMapObj)
{
	return 0;
}

int lib_shm_destroy(SHM_HANDLE hMapObj)
{
	if (shmctl(hMapObj, IPC_RMID, NULL) == 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

//dynamic linking loader

//0 success; -1 error
HANDLE lib_load_library(const char *filename)
{
	void *handle;

	if (filename == NULL)
		return NULL;

	handle = dlopen(filename, RTLD_LAZY);

	if (!handle)
		return NULL;

	return handle;
}

//0 success; -1 error
void *lib_get_addr(HANDLE handle, const char *module)
{
	void *ptr = NULL;

	if (handle == NULL || module == NULL)
		return NULL;

	ptr = (void *) dlsym(handle, module);

	if (ptr == NULL)
		return NULL;

	return ptr;
}

//0 success; -1 error
int lib_free_library(HANDLE handle)
{
	if (handle == NULL)
		return -1;

	if (dlclose(handle) != 0)
		return -1;
	else
		return 0;
}

//NULL无错误,否则有错误
char *lib_get_liberror()
{
	return dlerror();
}

//some useful functions

//0 success; other error
int lib_sleep(unsigned int milliseconds)
{
	int nRet;
	struct timeval tm;

	tm.tv_sec = milliseconds / 1000;
	tm.tv_usec = (milliseconds % 1000) * 1000;
	nRet = select(0, 0, 0, 0, &tm);

	return nRet;
}

//0 success; -1 error
int lib_getlocaltime(lib_systime *psystime)
{
	int nRet = 0;
	time_t t;
	struct tm *ptm = NULL;
	struct timeval tv;

	nRet = gettimeofday(&tv, 0);

	if (nRet != 0)
		return -1;

	t = tv.tv_sec;

	ptm = localtime(&t);

	if (ptm == NULL)
		return -1;

	psystime->wYear = ptm->tm_year + 1900;

	psystime->wMonth = ptm->tm_mon + 1;
	psystime->wDay = ptm->tm_mday;
	psystime->wDayOfWeek = ptm->tm_wday;
	psystime->wHour = ptm->tm_hour;
	psystime->wMinute = ptm->tm_min;
	psystime->wSecond = ptm->tm_sec;
	psystime->wMilliseconds = tv.tv_usec / 1000;

	return 0;
}

//0 success; -1 error
int lib_setlocaltime(const lib_systime *psystime)
{
	time_t t;
	struct tm mtm;
	struct timeval tv;

	mtm.tm_year = psystime->wYear - 1900;
	mtm.tm_mon = psystime->wMonth - 1;
	mtm.tm_mday = psystime->wDay;
	mtm.tm_wday = psystime->wDayOfWeek;
	mtm.tm_hour = psystime->wHour;
	mtm.tm_min = psystime->wMinute;
	mtm.tm_sec = psystime->wSecond;
	mtm.tm_isdst = 0;

	t = mktime(&mtm);
	if (t == -1)
		return -1;

	tv.tv_sec = t;
	tv.tv_usec = psystime->wMilliseconds * 1000;

	return settimeofday(&tv, 0);
}

int lib_getcurrenttime(time_t *pSeconds, unsigned short *pMilliSeconds)
{
	struct timeb timebuffer;

	if (NULL == pSeconds || NULL == pMilliSeconds)
	{
		return -1;
	}

	ftime(&timebuffer);
	*pSeconds = timebuffer.time;
	*pMilliSeconds = timebuffer.millitm;

	return 0;
}

int lib_time_init(lib_time_ms *ptTime)
{
	if (NULL == ptTime)
	{
		return -1;
	}

	//获取时钟频率
	ptTime->ticksPerSec = 0;
	ptTime->ticksPerSec = (unsigned int) sysconf(_SC_CLK_TCK);
	if (ptTime->ticksPerSec == (unsigned int) -1L || ptTime->ticksPerSec == 0)
	{
		return -1;	//失败
	}

	//初始化参数
	ptTime->lastCheckTime = 0;
	ptTime->wrapCount = 0;
	ptTime->initMsTime = _lib_time_get(ptTime);

	return 0;
}

double _lib_time_get(lib_time_ms *ptTime)
{
	double totalMsTime = 0.0;
	struct tms tmsTime;
	unsigned int ticks = 0;

	if (NULL == ptTime)
	{
		return 0.0;
	}

	//获取tick计数
	ticks = (unsigned int) times(&tmsTime);
	if (ticks == (unsigned int) -1)
	{
		return 0.0;
	}

	//如果时间低位越界，则进1到高位
	if (ticks < ptTime->lastCheckTime)
	{
		ptTime->wrapCount += 1;
	}

	//根据高位和低位tick数，计算总毫秒
	totalMsTime = (double) (((unsigned long long) (((unsigned long long) ptTime->wrapCount) << 32) + (unsigned long long) ticks) * 1000 / (unsigned long long) ptTime->ticksPerSec);
	ptTime->lastCheckTime = ticks;/* save for next check	*/

	return totalMsTime;
}

//0 success; -1 error
int lib_create_dir(const char *pdir)
{
	char *psrc = NULL;
	char *pdest = NULL;
	char tmpPath[32];
	char fulPath[256];
	int ret = 0;

	if (NULL == pdir || strlen(pdir) <= 1 || *pdir != '/')
	{
		return -1;
	}

	psrc = (char *) (pdir + 1);
	memset(fulPath, 0, 256);
	fulPath[0] = '/';
	while (1)
	{
		memset(tmpPath, 0, 32);
		pdest = tmpPath;
		while (*psrc != 0 && *psrc != '/')
		{
			*pdest++ = *psrc++;
		}

		if (strlen(tmpPath) > 0)
		{
			strcpy(fulPath + strlen(fulPath), tmpPath);
			if (lib_pathfile_exist(fulPath) != 0)
			{
				ret = mkdir(fulPath, S_IRWXU);
			}
		}

		if (0 == *psrc || ret != 0)
		{
			break;	//遍历结束或创建目录失败
		}
		else
		{
			//*psrc=='/'
			fulPath[strlen(fulPath)] = '/';
			psrc++;
		}
	}

	return ret;
}

//0 yes; -1 no
int lib_pathfile_exist(const char *pathfile)
{
	if (access(pathfile, F_OK) != 0)
	{
		return -1;
	}

	return 0;
}

//0 yes; -1 no
int lib_pathfile_exec(const char *pathfile)
{
	if (access(pathfile, X_OK) != 0)
	{
		return -1;
	}

	return 0;
}

//0 yes; -1 no
int lib_pathfile_rw(const char *pathfile)
{
	if (access(pathfile, R_OK | W_OK) != 0)
	{
		return -1;
	}

	return 0;
}

int lib_get_file_size(const char *file_name)
{
	struct stat file_desc;

	if (stat(file_name, &file_desc) == 0)
	{
		return file_desc.st_size;
	}

	return -1;
}

int lib_get_filenum_of_dir(const char *pathfile)
{
	return 0;
}

int lib_get_filelist_of_dir(const char *pathfile, SHR_FILE_ATTRI *pfilelist, int maxnum)
{
	return 0;
}

//非NULL成功；NULL失败
HRSPORT lib_rsport_open(unsigned int nr)
{
	HRSPORT hCom = 0;
	char port_name[32];

	if (nr <= 0)
	{
		return NULL;
	}

	sprintf(port_name, "/dev/ttyS%d", nr - 1);
	hCom = open(port_name, O_RDWR | O_NOCTTY);

	if (hCom < 0)
	{
		hCom = NULL;
	}

	return hCom;
}

//0成功；非0失败。
int lib_rsport_close(HRSPORT rsport)
{
//	if (NULL==rsport)
//	{
//		return -1;
//	}

	if (close(rsport) == 0)
		return 0;
	else
		return -1;
}

const speed_t baudratemap[] =
{ B110, B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, };

const int databit[] =
{ CS5, CS6, CS7, CS8, };

//0成功；非0失败。
int lib_rsport_setup(HRSPORT rsport, const rsport_attr* pattr)
{
	struct termios options;

	if (tcgetattr(rsport, &options) < 0 || cfsetispeed(&options, (speed_t) baudratemap[pattr->baudrate]) < 0 || cfsetospeed(&options, (speed_t) baudratemap[pattr->baudrate]) < 0)
	{
		return -1;
	}

	//数据位
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= databit[pattr->databit];

	/* 设置停止位*/
	options.c_cflag &= ~CSTOPB;
	switch (pattr->stopbit)
	{
	case rsstopbit_1:
		options.c_cflag &= ~CSTOPB;
		break;
	case rsstopbit_2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		return -1;
		break;
	}

	//校验类型
	switch (pattr->parity)
	{
	case rsparity_none:
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~(INPCK | ICRNL);
		break;
	case rsparity_odd:
		options.c_cflag |= PARODD;
		options.c_cflag |= PARENB;
		options.c_iflag |= INPCK;
		break;
	case rsparity_even:
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case rsparity_space: /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~(INPCK | ICRNL);
		break;
	default:
		return -1;
		break;
	}

	/* set the input options */
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	options.c_iflag &= ~(INLCR | IGNCR | ICRNL);

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* set the output options */
	options.c_oflag &= ~OPOST;

	/* set local mode and enable the receiver */
	options.c_cflag |= (CLOCAL | CREAD);

	//strange! tcgetattr failed on linux with errno set to 5(I/O error)
	tcsetattr(rsport, TCSANOW, &options);

	return 0;
}

//0成功；非0失败。
int lib_rsport_set_timeout( HRSPORT rsport, unsigned int rxTimeout, unsigned int txTimeout)
{
	struct termios options;

	tcgetattr(rsport, &options);

	options.c_cc[VTIME] = rxTimeout / 100;
	options.c_cc[VMIN] = 0;

	tcsetattr(rsport, TCSANOW, &options);

	return 0;
}

//0成功；非0失败。
int lib_rsport_write(HRSPORT rsport, const char* pbuff, int bytes_to_write)
{
	return write(rsport, pbuff, bytes_to_write);
}

//0成功；非0失败。
int lib_rsport_read(HRSPORT rsport, char* pbuff, int bytes_to_read)
{
	return read(rsport, pbuff, bytes_to_read);
}

//0成功；非0失败。
int lib_rsport_purge(HRSPORT rsport, int read, int write)
{
	int opt = 0;

	if (read)
		opt = TCIFLUSH;
	else
	{
		if (write)
			opt = TCIOFLUSH;
		else
			opt = TCOFLUSH;
	}

	if (opt)
	{
		return tcflush(rsport, opt);
	}

	return 0;
}

#endif//OS_LINUX
#ifdef OS_MAC

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun
{
	int val; /* Value for SETVAL */
	struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* Array for GETALL, SETALL */
	struct seminfo *__buf; /* Buffer for IPC_INFO*/
};
#endif

void lib_get_float32(unsigned char *pbuff, float *out)
{
	unsigned char *ptr = (unsigned char *) out;

	*ptr++ = *pbuff++;
	*ptr++ = *pbuff++;
	*ptr++ = *pbuff++;
	*ptr++ = *pbuff++;
}

//process

int lib_process_create(PROS_HANDLE *phandle, char *pfilename, char *pcmdline)
{
	int i = 0;
	pid_t pid = 0;
	char *pTemp = NULL;
	char *argv[11] =
	{ NULL };	//最多允许pcmdline中有10个参数

	//把pcmdline中的参数解析到argv
	for (i = 0; i < 10, *pcmdline != 0; i++)
	{
		while (*pcmdline == 0x20)
		{
			pcmdline++;	//除去参数i前可能的空格
		}

		if (*pcmdline == 0)
		{
			break;	//命令行结束
		}

		//查找参数i
		pTemp = pcmdline;
		while (*pcmdline != 0 && *pcmdline != 0x20)
		{
			pcmdline++;
		}

		//查参数i结束
		if (*pcmdline == 0x20)
		{
			*pcmdline = 0;
			pcmdline++;
		}

		argv[i] = pTemp;
	}
	argv[i] = NULL;

	pid = fork();
	if (pid < 0)
	{
		return -1;
	}
	else if (0 == pid)
	{
		//child process
		if (execv((const char *) pfilename, (char * const *) argv) < 0)
		{
			exit(-1);
		}
	}
	else
	{
		//parent process
		*phandle = pid;
		return 0;
	}

	exit(0);
}

int lib_process_destroy(PROS_HANDLE handle, int iSignal)
{
	kill(handle, iSignal);	//杀掉所有子进程
	return 0;
}

int lib_processV2_create(PROS_HANDLE *phandle, void (*pfun)(char *pname), char *pname)
{
	pid_t pid = 0;

	pid = fork();
	if (pid < 0)
	{
		return -1;
	}
	else if (0 == pid)
	{
		//child process
		(*pfun)(pname);
		exit(0);
	}
	else
	{
		//parent process
		*phandle = pid;
		return 0;
	}

	return 0;
}

//thread

//0 success; -1 error
int lib_task_create(char *pname, int priority, int stackSize, TASK_HANDLE *phandle, LIB_TASK_ROUTINE routine, void *param)
{
	pthread_t t;

	if (pthread_create(&t, 0, routine, param) != 0)
		return -1;

	if (phandle)
	{
		*phandle = t;
		return 0;
	}
	else
	{
		pthread_detach(t);
		return -1;
	}
}

//0 success; -1 error
int lib_task_detach(TASK_HANDLE handle)
{
	return pthread_detach(handle);
}

//0 success; -1 error
int lib_task_join(TASK_HANDLE handle, void **pretcode)
{
	return pthread_join(handle, pretcode);
}

void lib_task_exit(unsigned int retval)
{
	pthread_exit((void *) retval);
}

//mutex

//0 success; other error
int lib_mutex_init(HMUTEX *mutex)
{
	pthread_mutexattr_t mt;

	pthread_mutexattr_init(&mt);
	pthread_mutexattr_settype(&mt, PTHREAD_MUTEX_RECURSIVE_NP);

	return pthread_mutex_init(mutex, &mt);
}

//0 success; other error
int lib_mutex_lock(HMUTEX *mutex)
{
	return pthread_mutex_lock(mutex);
}

//0 success; other error
int lib_mutex_unlock(HMUTEX *mutex)
{
	return pthread_mutex_unlock(mutex);
}

//0 success; other error
int lib_mutex_destroy(HMUTEX *mutex)
{
	return pthread_mutex_destroy(mutex);
}

//cond

//0 success; other error
int lib_cond_init(HCOND *cond)
{
	return pthread_cond_init(cond, 0);
}

//0 success; other error
int lib_cond_wait(HMUTEX *mutex, HCOND *cond, int milliseconds)
{
	int nRet = 0;

	if (milliseconds == -1)
	{
		nRet = pthread_cond_wait(cond, mutex);
	}
	else
	{
		struct timeval tv;
		struct timespec tm;

		gettimeofday(&tv, 0);

		tm.tv_sec = tv.tv_sec + milliseconds / 1000;
		tm.tv_nsec = (milliseconds % 1000) * 1000 + tv.tv_usec;
		tm.tv_sec += tm.tv_nsec / 1000000;
		tm.tv_nsec = (tm.tv_nsec % 1000000) * 1000;

		nRet = pthread_cond_timedwait(cond, mutex, &tm);
	}

	if (nRet != 0)
	{
		if (nRet == ETIMEDOUT)
		{
			nRet = -1;	//timeout
		}
		else
		{
			nRet = -2;	//error
		}
	}

	return nRet;
}

//0 success; other error
int lib_cond_signal(HCOND *cond)
{
	return pthread_cond_signal(cond);
}

//0 success; other error
int lib_cond_broadcast(HCOND *cond)
{
	return pthread_cond_signal(cond);
}

//0 success; other error
int lib_cond_destroy(HCOND *cond)
{
	return pthread_cond_destroy(cond);
}

//no name semaphore

//0 success; -1 error
int lib_noname_sem_create(HNSEM *sem, unsigned int value)
{
	return sem_init(sem, 0, value);
}

//0 success; -1 error
int lib_noname_sem_destroy(HNSEM *sem)
{
	return sem_destroy(sem);
}

//0 success; -1 error
int lib_noname_sem_wait(HNSEM *sem)
{
	return sem_wait(sem);
}

//0 success; -1 error
int lib_noname_sem_post(HNSEM *sem)
{
	return sem_post(sem);
}

//0 success; -1 error
int lib_noname_sem_getvalue(HNSEM *sem)
{
	int value = 0;

	sem_getvalue(sem, &value);
	return value;
}

//name semaphore

//0 success; -1 error
int lib_name_sem_create(char *pfilename, unsigned int max_val, unsigned int init_val, HSEM *phandle)
{
	HSEM hSemObj = NULL;
	key_t shmKey;
	union semun semopts;
	FILE *hFile = NULL;

	if (NULL == pfilename || NULL == phandle)
	{
		return -1;
	}

	if (lib_pathfile_exist(pfilename) != 0)
	{
		//文件不存在，则创建该文件。
		hFile = fopen(pfilename, "wb");
		if (NULL == hFile)
		{
			return -1;
		}
		fclose(hFile);
	}

	//获取IPC的key
	shmKey = ftok(pfilename, 2);
	if (shmKey == (key_t) - 1)
	{
		return -1;
	}

	//判断该信号量是否存在？
	hSemObj = semget(shmKey, 0, 0777);
	if (hSemObj != -1)
	{
		//存在，则删除
		if (lib_name_sem_destroy(hSemObj) == -1)
		{
			return -1;	//删除失败
		}
	}

	//创建信号量
	hSemObj = semget(shmKey, 1, IPC_CREAT | IPC_EXCL | 0777);
	if (-1 == hSemObj)
	{
		return -1;
	}

	//设置初始值
	semopts.val = init_val;
	if (semctl(hSemObj, 0, SETVAL, semopts) == -1)
	{
		return -1;
	}

	*phandle = hSemObj;

	return 0;
}

//-1失败; 0成功
int lib_name_sem_open(char *pfilename, HSEM *phandle)
{
	HSEM hSemObj = NULL;
	key_t shmKey;

	if ((NULL == pfilename) || (NULL == phandle))
	{
		return -1;
	}

	//获取IPC的key
	shmKey = ftok(pfilename, 2);
	if (shmKey == (key_t) - 1)
	{
		return -1;
	}

	hSemObj = semget(shmKey, 0, 0777);
	if (hSemObj == -1)
	{
		return -1;
	}

	*phandle = hSemObj;
	return 0;
}

//0 success; -1 error
int lib_name_sem_close(HSEM hSemObj)
{
	return 0;
}

//0 success; -1 error
int lib_name_sem_destroy(HSEM hSemObj)
{
	union semun semopts;

	if (semctl(hSemObj, 0, IPC_RMID, semopts) == -1)
		return -1;

	return 0;
}

//0 success; -1 error
int lib_name_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds)
{
	struct sembuf sem =
	{ 0, -1, IPC_NOWAIT | SEM_UNDO };
	unsigned int total = 0;

	if (INFINITE == dwMilliseconds)
	{
		//阻塞等待
		sem.sem_flg = SEM_UNDO;
		if (semop(hSemObj, &sem, 1) == 0)
		{
			return 0;	//成功
		}
		else
		{
			return -1;	//失败
		}
	}

	//非阻塞等待
	while (1)
	{
		if (semop(hSemObj, &sem, 1) == 0)
		{
			return 0;	//成功
		}

		if (total >= dwMilliseconds)
		{
			break;	//超时，退出
		}

		lib_sleep(5);	//睡眠5毫秒
		total += 5;
	}

	return -1;
}

//0 success; -1 error
int lib_beat_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds)
{
	struct sembuf sem =
	{ 0, -1, IPC_NOWAIT };
	unsigned int total = 0;

	//非阻塞等待
	while (1)
	{
		if (semop(hSemObj, &sem, 1) == 0)
		{
			return 0;	//成功
		}

		if (total >= dwMilliseconds)
		{
			break;	//超时，退出
		}

		lib_sleep(5);	//睡眠5毫秒
		total += 5;
	}

	return -1;
}

//0 success; -1 error
int lib_name_sem_post(HSEM hSemObj)
{
	struct sembuf sem =
	{ 0, 1, IPC_NOWAIT | SEM_UNDO };

	if (semop(hSemObj, &sem, 1) == -1)
		return -1;

	return 0;
}

//0 success; -1 error
int lib_beat_sem_post(HSEM hSemObj)
{
	union semun semopts;
	struct sembuf sem =
	{ 0, 1, IPC_NOWAIT };

	if (semctl(hSemObj, 0, GETVAL, semopts) > 0)
	{
		return 0;
	}

	if (semop(hSemObj, &sem, 1) == -1)
		return -1;

	return 0;
}

//0 success; -1 error
int lib_name_sem_getvalue(HSEM hSemObj)
{
	union semun semopts;
	return semctl(hSemObj, 0, GETVAL, semopts);
}

//0 success; -1 error
int lib_shm_create(char *pfilename, unsigned int max_size, SHM_HANDLE *phandle)
{
	SHM_HANDLE hMapObj;
	key_t shmKey;
	FILE *hFile = NULL;

	if ((NULL == pfilename) || (max_size <= 0) || (NULL == phandle))
	{
		return -1;
	}

	if (lib_pathfile_exist(pfilename) != 0)
	{
		//文件不存在，则创建该文件。
		hFile = fopen(pfilename, "wb");
		if (NULL == hFile)
		{
			return -1;
		}
		fclose(hFile);
	}

	shmKey = ftok(pfilename, 1);
	if (shmKey == (key_t) - 1)
	{
		return -1;
	}

	hMapObj = shmget(shmKey, 0, (0777));
	if (hMapObj != -1)
	{
		if (lib_shm_destroy(hMapObj) == -1)
		{
			return -1;	//删除失败
		}
	}

	hMapObj = shmget(shmKey, max_size, (IPC_CREAT | 0777));
	if (hMapObj == -1)
	{
		return -1;
	}

	*phandle = hMapObj;

	return 0;
}

//0 success; -1 error
int lib_shm_open(char *pfilename, SHM_HANDLE *phandle)
{
	SHM_HANDLE hMapObj;
	key_t shmKey;

	if ((NULL == pfilename) || (NULL == phandle))
	{
		return -1;
	}

	shmKey = ftok(pfilename, 1);
	if (shmKey == (key_t) - 1)
	{
		return -1;
	}

	hMapObj = shmget(shmKey, 0, 0777);
	if (hMapObj == -1)
	{
		return -1;
	}
	*phandle = hMapObj;

	return 0;
}

//NULL失败，其它成功
void* lib_shm_map(SHM_HANDLE hShmObj)
{
	void *pMapAddr;

	pMapAddr = shmat(hShmObj, NULL, 0);
	if (pMapAddr == (void *) -1)
	{
		return NULL;
	}

	return pMapAddr;
}

int lib_shm_unmap(void *hMapAddr)
{
	if (shmdt(hMapAddr) == 0)
		return 0;
	else
		return -1;
}

int lib_shm_close(SHM_HANDLE hMapObj)
{
	return 0;
}

int lib_shm_destroy(SHM_HANDLE hMapObj)
{
	if (shmctl(hMapObj, IPC_RMID, NULL) == 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

//dynamic linking loader

//0 success; -1 error
HANDLE lib_load_library(const char *filename)
{
	void *handle;

	if (filename == NULL)
		return NULL;

	handle = dlopen(filename, RTLD_LAZY);

	if (!handle)
		return NULL;

	return handle;
}

//0 success; -1 error
void *lib_get_addr(HANDLE handle, const char *module)
{
	void *ptr = NULL;

	if (handle == NULL || module == NULL)
		return NULL;

	ptr = (void *) dlsym(handle, module);

	if (ptr == NULL)
		return NULL;

	return ptr;
}

//0 success; -1 error
int lib_free_library(HANDLE handle)
{
	if (handle == NULL)
		return -1;

	if (dlclose(handle) != 0)
		return -1;
	else
		return 0;
}

//NULL无错误,否则有错误
char *lib_get_liberror()
{
	return dlerror();
}

//some useful functions

//0 success; other error
int lib_sleep(unsigned int milliseconds)
{
	int nRet;
	struct timeval tm;

	tm.tv_sec = milliseconds / 1000;
	tm.tv_usec = (milliseconds % 1000) * 1000;
	nRet = select(0, 0, 0, 0, &tm);

	return nRet;
}

//0 success; -1 error
int lib_getlocaltime(lib_systime *psystime)
{
	int nRet = 0;
	time_t t;
	struct tm *ptm = NULL;
	struct timeval tv;

	nRet = gettimeofday(&tv, 0);

	if (nRet != 0)
		return -1;

	t = tv.tv_sec;

	ptm = localtime(&t);

	if (ptm == NULL)
		return -1;

	psystime->wYear = ptm->tm_year + 1900;

	psystime->wMonth = ptm->tm_mon + 1;
	psystime->wDay = ptm->tm_mday;
	psystime->wDayOfWeek = ptm->tm_wday;
	psystime->wHour = ptm->tm_hour;
	psystime->wMinute = ptm->tm_min;
	psystime->wSecond = ptm->tm_sec;
	psystime->wMilliseconds = tv.tv_usec / 1000;

	return 0;
}

//0 success; -1 error
int lib_setlocaltime(const lib_systime *psystime)
{
	time_t t;
	struct tm mtm;
	struct timeval tv;

	mtm.tm_year = psystime->wYear - 1900;
	mtm.tm_mon = psystime->wMonth - 1;
	mtm.tm_mday = psystime->wDay;
	mtm.tm_wday = psystime->wDayOfWeek;
	mtm.tm_hour = psystime->wHour;
	mtm.tm_min = psystime->wMinute;
	mtm.tm_sec = psystime->wSecond;
	mtm.tm_isdst = 0;

	t = mktime(&mtm);
	if (t == -1)
		return -1;

	tv.tv_sec = t;
	tv.tv_usec = psystime->wMilliseconds * 1000;

	return settimeofday(&tv, 0);
}

int lib_getcurrenttime(time_t *pSeconds, unsigned short *pMilliSeconds)
{
	struct timeb timebuffer;

	if (NULL == pSeconds || NULL == pMilliSeconds)
	{
		return -1;
	}

	ftime(&timebuffer);
	*pSeconds = timebuffer.time;
	*pMilliSeconds = timebuffer.millitm;

	return 0;
}

int lib_time_init(lib_time_ms *ptTime)
{
	if (NULL == ptTime)
	{
		return -1;
	}

	//获取时钟频率
	ptTime->ticksPerSec = 0;
	ptTime->ticksPerSec = (unsigned int) sysconf(_SC_CLK_TCK);
	if (ptTime->ticksPerSec == (unsigned int) -1L || ptTime->ticksPerSec == 0)
	{
		return -1;	//失败
	}

	//初始化参数
	ptTime->lastCheckTime = 0;
	ptTime->wrapCount = 0;
	ptTime->initMsTime = _lib_time_get(ptTime);

	return 0;
}

double _lib_time_get(lib_time_ms *ptTime)
{
	double totalMsTime = 0.0;
	struct tms tmsTime;
	unsigned int ticks = 0;

	if (NULL == ptTime)
	{
		return 0.0;
	}

	//获取tick计数
	ticks = (unsigned int) times(&tmsTime);
	if (ticks == (unsigned int) -1)
	{
		return 0.0;
	}

	//如果时间低位越界，则进1到高位
	if (ticks < ptTime->lastCheckTime)
	{
		ptTime->wrapCount += 1;
	}

	//根据高位和低位tick数，计算总毫秒
	totalMsTime = (double) (((unsigned long long) (((unsigned long long) ptTime->wrapCount) << 32) + (unsigned long long) ticks) * 1000 / (unsigned long long) ptTime->ticksPerSec);
	ptTime->lastCheckTime = ticks;/* save for next check	*/

	return totalMsTime;
}

//0 success; -1 error
int lib_create_dir(const char *pdir)
{
	char *psrc = NULL;
	char *pdest = NULL;
	char tmpPath[32];
	char fulPath[256];
	int ret = 0;

	if (NULL == pdir || strlen(pdir) <= 1 || *pdir != '/')
	{
		return -1;
	}

	psrc = (char *) (pdir + 1);
	memset(fulPath, 0, 256);
	fulPath[0] = '/';
	while (1)
	{
		memset(tmpPath, 0, 32);
		pdest = tmpPath;
		while (*psrc != 0 && *psrc != '/')
		{
			*pdest++ = *psrc++;
		}

		if (strlen(tmpPath) > 0)
		{
			strcpy(fulPath + strlen(fulPath), tmpPath);
			if (lib_pathfile_exist(fulPath) != 0)
			{
				ret = mkdir(fulPath, S_IRWXU);
			}
		}

		if (0 == *psrc || ret != 0)
		{
			break;	//遍历结束或创建目录失败
		}
		else
		{
			//*psrc=='/'
			fulPath[strlen(fulPath)] = '/';
			psrc++;
		}
	}

	return ret;
}

//0 yes; -1 no
int lib_pathfile_exist(const char *pathfile)
{
	if (access(pathfile, F_OK) != 0)
	{
		return -1;
	}

	return 0;
}

//0 yes; -1 no
int lib_pathfile_exec(const char *pathfile)
{
	if (access(pathfile, X_OK) != 0)
	{
		return -1;
	}

	return 0;
}

//0 yes; -1 no
int lib_pathfile_rw(const char *pathfile)
{
	if (access(pathfile, R_OK | W_OK) != 0)
	{
		return -1;
	}

	return 0;
}

int lib_get_file_size(const char *file_name)
{
	struct stat file_desc;

	if (stat(file_name, &file_desc) == 0)
	{
		return file_desc.st_size;
	}

	return -1;
}

int lib_get_filenum_of_dir(const char *pathfile)
{
	return 0;
}

int lib_get_filelist_of_dir(const char *pathfile, SHR_FILE_ATTRI *pfilelist, int maxnum)
{
	return 0;
}

//非NULL成功；NULL失败
HRSPORT lib_rsport_open(unsigned int nr)
{
	HRSPORT hCom = 0;
	char port_name[32];

	if (nr <= 0)
	{
		return NULL;
	}

	sprintf(port_name, "/dev/ttyS%d", nr - 1);
	hCom = open(port_name, O_RDWR | O_NOCTTY);

	if (hCom < 0)
	{
		hCom = NULL;
	}

	return hCom;
}

//0成功；非0失败。
int lib_rsport_close(HRSPORT rsport)
{
//	if (NULL==rsport)
//	{
//		return -1;
//	}

	if (close(rsport) == 0)
		return 0;
	else
		return -1;
}

const speed_t baudratemap[] =
{ B110, B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, };

const int databit[] =
{ CS5, CS6, CS7, CS8, };

//0成功；非0失败。
int lib_rsport_setup(HRSPORT rsport, const rsport_attr* pattr)
{
	struct termios options;

	if (tcgetattr(rsport, &options) < 0 || cfsetispeed(&options, (speed_t) baudratemap[pattr->baudrate]) < 0 || cfsetospeed(&options, (speed_t) baudratemap[pattr->baudrate]) < 0)
	{
		return -1;
	}

	//数据位
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= databit[pattr->databit];

	/* 设置停止位*/
	options.c_cflag &= ~CSTOPB;
	switch (pattr->stopbit)
	{
	case rsstopbit_1:
		options.c_cflag &= ~CSTOPB;
		break;
	case rsstopbit_2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		return -1;
		break;
	}

	//校验类型
	switch (pattr->parity)
	{
	case rsparity_none:
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~(INPCK | ICRNL);
		break;
	case rsparity_odd:
		options.c_cflag |= PARODD;
		options.c_cflag |= PARENB;
		options.c_iflag |= INPCK;
		break;
	case rsparity_even:
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case rsparity_space: /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~(INPCK | ICRNL);
		break;
	default:
		return -1;
		break;
	}

	/* set the input options */
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	options.c_iflag &= ~(INLCR | IGNCR | ICRNL);

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* set the output options */
	options.c_oflag &= ~OPOST;

	/* set local mode and enable the receiver */
	options.c_cflag |= (CLOCAL | CREAD);

	//strange! tcgetattr failed on linux with errno set to 5(I/O error)
	tcsetattr(rsport, TCSANOW, &options);

	return 0;
}

//0成功；非0失败。
int lib_rsport_set_timeout( HRSPORT rsport, unsigned int rxTimeout, unsigned int txTimeout)
{
	struct termios options;

	tcgetattr(rsport, &options);

	options.c_cc[VTIME] = rxTimeout / 100;
	options.c_cc[VMIN] = 0;

	tcsetattr(rsport, TCSANOW, &options);

	return 0;
}

//0成功；非0失败。
int lib_rsport_write(HRSPORT rsport, const char* pbuff, int bytes_to_write)
{
	return write(rsport, pbuff, bytes_to_write);
}

//0成功；非0失败。
int lib_rsport_read(HRSPORT rsport, char* pbuff, int bytes_to_read)
{
	return read(rsport, pbuff, bytes_to_read);
}

//0成功；非0失败。
int lib_rsport_purge(HRSPORT rsport, int read, int write)
{
	int opt = 0;

	if (read)
		opt = TCIFLUSH;
	else
	{
		if (write)
			opt = TCIOFLUSH;
		else
			opt = TCOFLUSH;
	}

	if (opt)
	{
		return tcflush(rsport, opt);
	}

	return 0;
}

#endif//OS_MAC

int lib_modify_time(lib_systime *psystime, unsigned short wMilliseconds, char op)
{
	time_t t;
	int nTempValue = 0;
	struct tm mtm;
	struct timeval tv1;
	struct tm *ptm = NULL;
	unsigned short seconds = 0;

	if (psystime == NULL)
	{
		return -1;
	}

	mtm.tm_year = psystime->wYear - 1900;
	mtm.tm_mon = psystime->wMonth - 1;
	mtm.tm_mday = psystime->wDay;
	mtm.tm_wday = psystime->wDayOfWeek;
	mtm.tm_hour = psystime->wHour;
	mtm.tm_min = psystime->wMinute;
	mtm.tm_sec = psystime->wSecond;
	mtm.tm_isdst = 0;

	t = mktime(&mtm);

	tv1.tv_sec = t;	//秒
	tv1.tv_usec = psystime->wMilliseconds * 1000;	//微秒

	if (op == '+')
	{
		tv1.tv_usec += wMilliseconds * 1000;	//微秒

		tv1.tv_sec += tv1.tv_usec / 1000000;	//秒
		tv1.tv_usec = tv1.tv_usec % 1000000;	//微秒
	}
	else if (op == '-')
	{
		seconds = wMilliseconds / 1000;			//秒
		wMilliseconds = wMilliseconds % 1000;			//毫秒

		tv1.tv_sec -= seconds;			//秒

		nTempValue = wMilliseconds * 1000;			//微秒
		if (tv1.tv_usec >= nTempValue)
		{
			tv1.tv_usec -= nTempValue;
		}
		else
		{
			tv1.tv_sec -= 1;
			tv1.tv_usec += 1000000;
			tv1.tv_usec -= nTempValue;
		}
	}
	else
	{
		return -1;
	}

	t = tv1.tv_sec;
	ptm = localtime(&t);

	if (ptm == NULL)
		return -1;

	psystime->wYear = ptm->tm_year + 1900;

	psystime->wMonth = ptm->tm_mon + 1;
	psystime->wDay = ptm->tm_mday;
	psystime->wDayOfWeek = ptm->tm_wday;
	psystime->wHour = ptm->tm_hour;
	psystime->wMinute = ptm->tm_min;
	psystime->wSecond = ptm->tm_sec;
	psystime->wMilliseconds = tv1.tv_usec / 1000;

	return 0;
}

time_t shrtime_to_timet(lib_systime *psystime)
{
	struct tm tm_systime;

	if (psystime == NULL)
		return 0;

	tm_systime.tm_year = psystime->wYear - 1900;
	tm_systime.tm_mon = psystime->wMonth - 1;
	tm_systime.tm_mday = psystime->wDay;
	tm_systime.tm_hour = psystime->wHour;
	tm_systime.tm_min = psystime->wMinute;
	tm_systime.tm_sec = psystime->wSecond;

	return mktime(&tm_systime);
}

double lib_time_get(lib_time_ms *ptTime)
{
	return (_lib_time_get(ptTime) - ptTime->initMsTime);	//获取相对时间：毫秒
}
