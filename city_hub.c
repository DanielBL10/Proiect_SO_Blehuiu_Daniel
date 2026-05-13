#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
 
void start_monitor_logic() {
    pid_t hub_mon = fork();
    if (hub_mon < 0) return;

    if (hub_mon == 0) { int p[2];

    if (pipe(p) < 0) {
        exit(1); 
    }
    
    pid_t mon = fork();

    if (mon == 0) {  
    close(p[0]); 
    dup2(p[1], STDOUT_FILENO);
    close(p[1]);
    execlp("./monitor_reports", "monitor_reports", NULL);
    exit(1); 
}
    close(p[1]); char buf[512];
    int n;

    while ((n = read(p[0], buf, sizeof(buf) - 1)) > 0) {
    buf[n] = 0;
    printf("\n[HUB_MON] %s", buf);

    if (strstr(buf, "ERR:") || strstr(buf, "END:")) break;
    }

    close(p[0]);
    waitpid(mon, NULL, 0);
    printf("\n[HUB_MON] Procesul monitor s-a terminat.\n");
    exit(0);
    }
    printf("[HUB] hub_mon pornit în fundal (PID: %d).\n", hub_mon);
    }
