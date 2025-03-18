#define readline(prompt)        /* Kullanıcıdan düzenlenebilir bir satır okur: char *line = readline("> "); */
#define rl_clear_history()      /* Readline geçmişini temizler: rl_clear_history(); */
#define rl_on_new_line()        /* Yeni bir satır başlatıldığını bildirir: rl_on_new_line(); */
#define rl_replace_line(text, clear) /* Mevcut satırı yeni bir metinle değiştirir: rl_replace_line("Yeni", 0); */
#define rl_redisplay()          /* Readline ekranını yeniden çizer: rl_redisplay(); */
#define add_history(cmd)        /* Girdiyi geçmişe ekler: add_history("komut"); */
#define printf(fmt, ...)        /* Biçimlendirilmiş çıktı yazdırır: printf("Merhaba\n"); */
#define malloc(size)            /* Bellek ayırır: int *p = malloc(10 * sizeof(int)); */
#define free(ptr)               /* Belleği serbest bırakır: free(p); */
#define write(fd, buf, len)     /* Dosya tanımlayıcısına veri yazar: write(1, "Selam\n", 6); */
#define access(path, mode)      /* Dosya erişim izinlerini kontrol eder: int x = access("dosya.txt", R_OK); */
#define open(path, flags)       /* Dosya açar ve tanımlayıcı döndürür: int fd = open("dosya.txt", O_RDONLY); */
#define read(fd, buf, len)      /* Dosyadan veri okur: char buf[10]; read(fd, buf, 10); */
#define close(fd)               /* Dosya tanımlayıcısını kapatır: close(fd); */
#define fork()                  /* Yeni bir süreç oluşturur: pid_t pid = fork(); */
#define wait(status)            /* Çocuk sürecin bitmesini bekler: wait(NULL); */
#define waitpid(pid, status, opts) /* Belirli bir çocuk süreci bekler: waitpid(pid, NULL, 0); */
#define wait3(status, opts, rusage) /* Çocuk süreç durumunu ve kaynak kullanımını döndürür: wait3(NULL, 0, NULL); */
#define wait4(pid, status, opts, rusage) /* Belirli bir çocuk için kaynak kullanımı ile bekler: wait4(pid, NULL, 0, NULL); */
#define signal(sig, handler)    /* Sinyal işleyicisi atar: signal(SIGINT, SIG_IGN); */
#define sigaction(sig, act, oldact) /* Sinyal işleme için gelişmiş yapı tanımlar: struct sigaction sa; sigaction(SIGINT, &sa, NULL); */
#define sigemptyset(set)        /* Sinyal kümesini boşaltır: sigset_t set; sigemptyset(&set); */
#define sigaddset(set, sig)     /* Sinyal kümesine sinyal ekler: sigaddset(&set, SIGINT); */
#define kill(pid, sig)          /* Bir sürece sinyal gönderir: kill(pid, SIGTERM); */
#define exit(status)            /* Programı sonlandırır: exit(0); */
#define getcwd(buf, size)       /* Mevcut çalışma dizinini alır: char buf[256]; getcwd(buf, 256); */
#define chdir(path)             /* Çalışma dizinini değiştirir: chdir("/tmp"); */
#define stat(path, buf)         /* Dosya bilgilerini alır: struct stat st; stat("dosya.txt", &st); */
#define lstat(path, buf)        /* Sembolik bağlar için dosya bilgisi alır: lstat("bag.txt", &st); */
#define fstat(fd, buf)          /* Açık dosya tanımlayıcısından bilgi alır: fstat(fd, &st); */
#define unlink(path)            /* Dosyayı siler: unlink("dosya.txt"); */
#define execve(path, argv, envp) /* Yeni bir program çalıştırır: char *args[] = {"ls", NULL}; execve("/bin/ls", args, NULL); */
#define dup(fd)                 /* Dosya tanımlayıcısını kopyalar: int newfd = dup(fd); */
#define dup2(oldfd, newfd)      /* Dosya tanımlayıcısını belirli bir hedefe kopyalar: dup2(fd, 1); */
#define pipe(fds)               /* Süreçler arası boru oluşturur: int fd[2]; pipe(fd); */
#define opendir(path)           /* Dizin açar: DIR *dir = opendir("/tmp"); */
#define readdir(dir)            /* Dizindeki bir girdiyi okur: struct dirent *entry = readdir(dir); */
#define closedir(dir)           /* Dizini kapatır: closedir(dir); */
#define strerror(errnum)        /* Hata kodunu metne çevirir: char *err = strerror(errno); */
#define perror(msg)             /* Hata mesajını yazdırır: perror("Hata"); */
#define isatty(fd)              /* Dosya tanımlayıcısının terminal olup olmadığını kontrol eder: int x = isatty(0); */
#define ttyname(fd)             /* Terminal adını döndürür: char *name = ttyname(0); */
#define ttyslot()               /* Kullanıcının terminal slotunu döndürür: int slot = ttyslot(); */
#define ioctl(fd, cmd, arg)     /* Cihaz kontrolü yapar: ioctl(fd, TIOCGWINSZ, &winsize); */
#define getenv(name)            /* Çevre değişkenini alır: char *path = getenv("PATH"); */
#define tcsetattr(fd, opt, termios_p) /* Terminal özniteliklerini ayarlar: struct termios t; tcsetattr(0, TCSANOW, &t); */
#define tcgetattr(fd, termios_p) /* Terminal özniteliklerini alır: tcgetattr(0, &t); */
#define tgetent(bp, name)       /* Termcap veritabanından giriş alır: char bp[1024]; tgetent(bp, "xterm"); */
#define tgetflag(id)            /* Termcap bayrağını alır: int flag = tgetflag("am"); */
#define tgetnum(id)             /* Termcap sayısal değerini alır: int num = tgetnum("co"); */
#define tgetstr(id, area)       /* Termcap dizesini alır: char *str = tgetstr("cl", NULL); */
#define tgoto(cap, col, row)    /* Termcap hareket komutu oluşturur: char *cmd = tgoto(tgetstr("cm", NULL), 10, 10); */
#define tputs(str, affcnt, putc) /* Termcap komutunu terminale uygular: tputs(cmd, 1, putchar); */