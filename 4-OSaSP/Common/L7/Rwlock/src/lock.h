#pragma once

// Устанавливает определенную ожидающую блокировку на файл
void lock(int fd, short type);

// Устанавливает определенную ожидающую блокировку на запись в файле
void rec_lock(int fd, int recno, short type);

#define rlock(fd) lock(fd, F_RDLCK)
#define ulock(fd) lock(fd, F_UNLCK)

#define rec_rlock(fd, recno) rec_lock(fd, recno, F_RDLCK)
#define rec_wlock(fd, recno) rec_lock(fd, recno, F_WRLCK)
#define rec_ulock(fd, recno) rec_lock(fd, recno, F_UNLCK)