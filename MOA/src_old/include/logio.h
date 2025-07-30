#ifndef _LOGIO_H
#define _LOGIO_H
typedef HANDLE LOGFILE;
extern short Log_write_completed;

extern long LogWrite(LOGFILE fd, void *buffer, size_t toWrite);
extern long LogRead(LOGFILE fd, void *buffer, size_t toRead);
extern unsigned LogWriteEx(LOGFILE fd, void *buffer, size_t toWrite);
extern void isLogCompleted(LOGFILE fd);
extern long LogSeek(LOGFILE fd, long position, int origin);
#endif
