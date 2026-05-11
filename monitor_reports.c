#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

#define PID_FILE ".monitor_pid"

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    const char *msg = "\n[MONITOR] Semnal SIGINT primit. Se inchide programul...\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    keep_running = 0; 
}

void handle_sigusr1(int sig) {
    const char *msg = "[MONITOR] Notificare: Un raport nou a fost adaugat in sistem!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main() {
    struct sigaction sa_int;
    memset(&sa_int, 0, sizeof(sa_int));
    sa_int.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa_int, NULL);

    struct sigaction sa_usr1;
    memset(&sa_usr1, 0, sizeof(sa_usr1));
    sa_usr1.sa_handler = handle_sigusr1;
    sigaction(SIGUSR1, &sa_usr1, NULL);

    int fd = open(PID_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        const char *err = "Eroare la open .monitor_pid\n";
        write(STDERR_FILENO, err, strlen(err));
        exit(EXIT_FAILURE);
    }

    char pid_str[32];
    int len = snprintf(pid_str, sizeof(pid_str), "%d\n", getpid());
    write(fd, pid_str, len);
    close(fd);

    printf("[MONITOR] Pornit cu PID: %d. Astept semnale...\n", getpid());

    while (keep_running) {
        pause(); 
    }

    unlink(PID_FILE);
    const char *exit_msg = "[MONITOR] Fisier .monitor_pid sters. Iesire.\n";
    write(STDOUT_FILENO, exit_msg, strlen(exit_msg));

    return 0;
}