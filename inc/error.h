#ifndef ERROR_H
#define ERROR_H
void    err_dump(const char *, ...);            /* {App misc_source} */
void    err_msg(const char *, ...);
void    err_quit(const char *, ...);
void    err_exit(int, const char *, ...);
void    err_ret(const char *, ...);
void    err_sys(const char *, ...);
#endif
