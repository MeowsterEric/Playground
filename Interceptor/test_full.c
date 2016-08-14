#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/syscall.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "interceptor.h"

#define MAX_SYSCALLS 337
#define TRUE 1
#define FALSE 0

static int last_child;

int vsyscall_arg(int sno, int n, ...) {

	va_list va;
	long args[6];
	int i, ret;

	va_start(va, n);
	for (i = 0; i < n; i++) {
		args[i] = va_arg(va, long);
	}
	va_end(va);

	ret = syscall(sno, args[0], args[1], args[2]);
	if (ret) ret = -errno;
	return ret;
}

void on_quit(int signo, siginfo_t *si, void *ignored) {
	// printf("Terminating %d.\n", getpid());
	exit(0);
}

#define test(s, a, t) \
({\
	int i;\
	char dummy[1024];\
	\
	sprintf(dummy, s, a);\
	printf("\ttest: %s", dummy); \
	for(i=0; i<40-strlen(dummy); i++)\
		putchar('.');\
	if (!(t))\
		printf("failed %-18s\n", strerror(errno));\
	else\
		printf("passed %-18s\n", "");\
	fflush(stdout);\
})

#define subtest(s) puts("SUBTEST: "s)

void clear_log() {
	system("dmesg -c &> /dev/null");
}

/**
 * Check if the log contains what is expected - if log_message was done properly
 */
int find_log(long pid, long sno, long *args) {
	char message[1024], command[1024], output[1024];
	FILE *fp;

	sprintf(message, "[%lx]%lx(%lx,%lx,%lx,%lx,%lx,%lx)",
	        (long)getpid(), sno, args[0], args[1], args[2], args[3], args[4], args[5]);
	sprintf(command, "dmesg | grep \"\\[%lx\\]%lx(%lx,%lx,%lx,%lx,%lx,%lx)\" 2>&1",
	        (long)getpid(), sno, args[0], args[1], args[2], args[3], args[4], args[5]);

	fp = popen(command, "r");
	if (!fp)  return -1;

	while (fgets(output, sizeof(output) - 1, fp) != NULL) {
		if (strstr(output, message)) {
			pclose(fp);
			return 0;
		}
	}

	pclose(fp);
	return -1;
}

/**
 * Check if a syscall gets logged properly when it's been already intercepted
 */
int test_monitor(int sysno, int should_log) {
	int found_log;
	do_monitor1(sysno, &found_log);
	test("%d monitor", sysno, found_log == should_log);
}

/**
 * Invokes a syscall from current thread.
 */
int do_monitor1(int sysno, int* found_log) {
	int sno, ret, i;
	long args[6];

	sno = sysno;
	for (i = 0; i < 6; i++) {
		args[i] = rand();
	}

	ret = syscall(sno, args[0], args[1], args[2], args[3], args[4], args[5]);
	if (ret) ret = -errno;

	if (found_log)
		*found_log = find_log(getpid(), (long)sno, args) == 0;

	//printf("[%x]%lx(%lx,%lx,%lx,%lx,%lx,%lx)\n", getpid(), (long)sysno,
	//	args[0], args[1], args[2], args[3], args[4], args[5]);

	return ret;
}


/**
 * Test invoking syscall from current process and child process.
 */
void test_monitor2(sysc, should_log_parent, should_log_child) {
#define BEQ(a,b) (!(a)==!(b))
	int found_log_child, found_log_parent, child;
	switch (child = fork()) {
	case -1:
		assert(0);
	case 0:
		do_monitor1(sysc, &found_log_child);
		test("%d child monitor", sysc, BEQ(found_log_child, should_log_child));
		exit(0);
	default:
		do_monitor1(sysc, &found_log_parent);
		waitpid(child, NULL, 0);
		// printf("parent=%d, child=%d\n", getpid(), child);
		test("%d parent monitor", sysc, BEQ(found_log_parent, should_log_parent));
#undef BEQ
	}
}

int do_intercept(int syscall, int status) {
	test("%d intercept", syscall, vsyscall_arg(MY_CUSTOM_SYSCALL, 3, REQUEST_SYSCALL_INTERCEPT, syscall, getpid()) == status);
	return 0;
}


int do_release(int syscall, int status) {
	test("%d release", syscall, vsyscall_arg(MY_CUSTOM_SYSCALL, 3, REQUEST_SYSCALL_RELEASE, syscall, getpid()) == status);
	return 0;
}


int do_start(int syscall, int pid, int status) {
	int ret;
	if (pid == -1)
		pid = getpid();
	ret = vsyscall_arg(MY_CUSTOM_SYSCALL, 3, REQUEST_START_MONITORING, syscall, pid);
	test("%d start", syscall, ret == status);
	// printf("return value=%d", ret);
	return 0;
}

int do_stop(int syscall, int pid, int status) {
	if (pid == -1)
		pid = getpid();
	test("%d stop", syscall, vsyscall_arg(MY_CUSTOM_SYSCALL, 3, REQUEST_STOP_MONITORING, syscall, pid) == status);
	return 0;
}


/**
 * Run the tester as a non-root user, and basically run do_nonroot
 */
void do_as_guest(const char *str, int args1, int args2) {

	char cmd[1024];
	char cmd2[1024];
	char* exec[] = {"bash", "-c", cmd2, NULL};

	sprintf(cmd, str, args1, args2);
	sprintf(cmd2, "su nobody -c '%s' ", cmd);
	switch ((last_child = fork()))  {
	case -1:
		assert(0);
	case 0:
		execvp("/bin/bash", exec);
		assert(0);
	default:
		waitpid(last_child, NULL, 0);
	}
}

void test_B(int, int);

/* note this test is run after all processes have been intercepted*/
int do_nonroot(int syscall) {
	do_intercept(syscall, -EPERM);
	do_release(syscall, -EPERM);
	do_start(syscall, 0, -EPERM);
	do_stop(syscall, 0, -EPERM);
	do_start(syscall, 1, -EPERM);
	do_stop(syscall, 1, -EPERM);
	do_start(syscall, getpid(), 0);
	do_start(syscall, getpid(), -EBUSY);
	test_monitor(syscall, TRUE);
	do_stop(syscall, getpid(), 0);
	do_stop(syscall, getpid(), -EINVAL);

	puts("----- START OF test_B -----");
	test_B(syscall, FALSE);
	puts("----- END OF test_B -----");

	return 0;
}



void test_A(int syscall) {
	// test negative syscall number
	do_intercept(-1, -EINVAL);
	do_release(-1, -EINVAL);
	do_start(-1, -1, -EINVAL);
	do_stop(-1, -1, -EINVAL);
	// test the last syscall
	do_intercept(MAX_SYSCALLS - 1, 0);
	do_start(MAX_SYSCALLS - 1, -1, 0);
	do_stop(MAX_SYSCALLS - 1, -1, 0);
	do_release(MAX_SYSCALLS - 1, 0);
	// test greater than NR_syscalls
	do_intercept(MAX_SYSCALLS, -EINVAL);
	do_release(MAX_SYSCALLS, -EINVAL);
	do_start(MAX_SYSCALLS, -1, -EINVAL);
	do_stop(MAX_SYSCALLS, -1, -EINVAL);
	do_intercept(MAX_SYSCALLS + 1, -EINVAL);
	do_release(MAX_SYSCALLS + 1, -EINVAL);
	do_start(MAX_SYSCALLS + 1, -1, -EINVAL);
	do_stop(MAX_SYSCALLS + 1, -1, -EINVAL);
	// test MY_CUSTOM_SYSCALL
	do_intercept(MY_CUSTOM_SYSCALL, -EINVAL);
	do_release(MY_CUSTOM_SYSCALL, -EINVAL);
	do_start(MY_CUSTOM_SYSCALL, -1, -EINVAL);
	do_stop(MY_CUSTOM_SYSCALL, -1, -EINVAL);

	// test on pids
	do_intercept(syscall, 0);
	do_start(syscall, -2, -EINVAL);
	do_stop(syscall, -2, -EINVAL);
	do_release(syscall, 0);
}

void test_B(int syscall, int iamroot) {
#define subtestroot(s) printf((s" as %s\n"), (iamroot)?"root":"nonroot")

	subtestroot("EPERM: intercept");
	do_intercept(syscall, iamroot ? 0 : -EPERM);
	subtestroot("EPERM: monitor self");
	do_start(syscall, -1, 0);
	do_stop(syscall, -1, 0);
	subtestroot("EPERM: monitor all processes");
	do_start(syscall, 0, iamroot ? 0 : -EPERM);
	do_stop(syscall, 0, iamroot ? 0 : -EPERM);
	subtestroot("EPERM: monitor init process");
	do_start(syscall, 1, iamroot ? 0 : -EPERM);
	do_stop(syscall, 1, iamroot ? 0 : -EPERM);
	subtestroot("EPERM: release");
	do_release(syscall, iamroot ? 0 : -EPERM);

#undef subtestroot
}

void test_CDE(int syscall) {
	//C -EINVAL
	subtest("Cannot de-intercept a system call that has not been intercepted yet.");
	do_release(syscall, -EINVAL);
	subtest("Cannot stop monitoring a pid for a syscall that has not been intercepted.");
	do_stop(syscall, -1, -EINVAL);
	do_stop(syscall, 0, -EINVAL);
	subtest("Cannot start monitoring a pid for that has not been intercepted yet.");
	do_start(syscall, -1, -EINVAL);
	do_start(syscall, 0, -EINVAL);

	do_intercept(syscall, 0);

	subtest("Cannot start monitoring a pid that is invalid.");
	do_start(syscall, 1234567, -EINVAL);
	do_start(syscall, -2, -EINVAL);
	subtest("Cannot stop monitoring for a pid that is not being monitored.");
	do_stop(syscall, 0, -EINVAL);
	do_stop(syscall, -1, -EINVAL);
	subtest("Cannot stop monitoring for a pid that is blacklisted.");
	do_start(syscall, 0, 0);
	do_stop(syscall, -1, 0);
	do_stop(syscall, -1, -EINVAL);
	do_stop(syscall, 0, 0);
	subtest("EINVAL: stop all -> stop all");
	do_start(syscall, 0, 0);
	do_stop(syscall, 0, 0);
	do_stop(syscall, 0, -EINVAL);
	subtest("EINVAL: start one pid -> stop the same pid -> stop all");
	do_start(syscall, -1, 0);
	do_stop(syscall, -1, 0);
	do_stop(syscall, 0, -EINVAL);


	do_release(syscall, 0);

	//D -EBUSY
	do_intercept(syscall, 0);

	subtest("EBUSY: intercepting a system call that is already intercepted");
	do_intercept(syscall, -EBUSY);
	subtest("EBUSY: monitoring a pid that is being whitelisted");
	do_start(syscall, -1, 0);
	do_start(syscall, -1, -EBUSY);
	do_stop(syscall, -1, 0);
	subtest("EBUSY: monitoring a pid that is being monitored globally");
	do_start(syscall, 0, 0);
	do_start(syscall, -1, -EBUSY);
	do_stop(syscall, 0, 0);
	subtest("EBUSY: monitoring all pids when all pids are being monitored");
	do_start(syscall, 0, 0);
	do_start(syscall, 0, -EBUSY);
	do_stop(syscall, 0, 0);
	subtest("OK: monitoring all pids when some pids are being monitored");
	do_start(syscall, -1, 0);
	do_start(syscall, 0, 0);
	do_stop(syscall, -1, 0);
	do_stop(syscall, 0, 0);
	//E -ENOMEM
	//If a pid cannot be added to a monitored list, due to no memory being available, an -ENOMEM error code should be returned.
	// int child;
	// int res = 0;
	// while (!res) {
	// 	switch (child = fork()) {
	// 	case -1://error
	// 		assert(0);
	// 	case 0: //child
	// 		pause();
	// 	default: //parent
	// 		res = vsyscall_arg(MY_CUSTOM_SYSCALL, 3, REQUEST_START_MONITORING, syscall, child);
	// 		// printf("start monitoring %d: %d\n", child, res);
	// 		break;
	// 	}
	// }
	// test("%d no memory", syscall, res == -ENOMEM);
	// signal(SIGQUIT, SIG_IGN);
	// kill(-getpid(), SIGQUIT);

	do_release(syscall, 0);
}

void test_PASS(int syscall) {
	int child;

	subtest("intercept");
	do_intercept(syscall, 0);
	do_release(syscall, 0);

	subtest("monitor");
	do_intercept(syscall, 0);
	test_monitor(syscall, FALSE);
	do_start(syscall, -1, 0);
	test_monitor(syscall, TRUE);
	do_stop(syscall, -1, 0);
	test_monitor(syscall, FALSE);
	do_release(syscall, 0);

	subtest("monitor & kill monitored process");
	do_intercept(syscall, 0);
	switch (child = fork()) {
	case -1:
		assert(0);
	case 0:
		// monitor the child process then exit
		do_start(syscall, -1, 0);
		test_monitor(syscall, TRUE);
		exit(0);
	default:
		waitpid(child, NULL, 0);
		// child should have been unmonitored automatically
		// there is no way to start process with specific pid...
		do_start(syscall, child, -EINVAL);
		do_stop(syscall, child, -EINVAL);
	}
	do_release(syscall, 0);

	subtest("monitor all pids");
	do_intercept(syscall, 0);
	test_monitor2(syscall, FALSE, FALSE);
	do_start(syscall, 0, 0);
	test_monitor2(syscall, TRUE, TRUE);

	subtest("stop monitor current pid");
	do_stop(syscall, -1, 0);
	test_monitor2(syscall, FALSE, TRUE);

	subtest("monitor all -> stop one -> monitor all");
	do_start(syscall, 0, 0);
	do_stop(syscall, -1, 0);
	do_start(syscall, 0, 0);
	do_stop(syscall, 0, 0);

	subtest("monitor one -> stop all");
	do_start(syscall, -1, 0);
	do_stop(syscall, 0, 0);

	subtest("monitor all -> stop one -> stop all");
	do_start(syscall, 0, 0);
	do_stop(syscall, -1, 0);
	do_stop(syscall, 0, 0);

	subtest("reset");
	do_start(syscall, -1, 0);
	test_monitor2(syscall, TRUE, FALSE);
	do_stop(syscall, 0, 0);
	test_monitor2(syscall, FALSE, FALSE);
	do_release(syscall, 0);
}

void test_syscall(int syscall) {

	//clear_log();
	do_intercept(syscall, 0);
	do_as_guest("./test_full nonroot %d", syscall, 0);
	do_start(syscall, -2, -EINVAL);
	do_start(syscall, 0, 0);
	do_stop(syscall, 0, 0);
	do_start(syscall, 1, 0);
	do_as_guest("./test_full stop %d 1 %d", syscall, -EPERM);
	do_stop(syscall, 1, 0);
	do_as_guest("./test_full start %d -1 %d", syscall, 0);
	do_stop(syscall, last_child, -EINVAL);
	do_release(syscall, 0);

	puts("----- START OF test_PASS -----");
	test_PASS(syscall);
	puts("----- END OF test_PASS -----");
	puts("----- START OF test_A -----");
	test_A(syscall);
	puts("----- END OF test_A -----");
	puts("----- START OF test_B -----");
	test_B(syscall, TRUE);
	puts("----- END OF test_B -----");
	puts("----- START OF test_CDE ----");
	test_CDE(syscall);
	puts("----- END OF test_CDE ----");
}
int main(int argc, char **argv) {

	srand(time(NULL));

	if (argc > 1 && strcmp(argv[1], "intercept") == 0)
		return do_intercept(atoi(argv[2]), atoi(argv[3]));

	if (argc > 1 && strcmp(argv[1], "release") == 0)
		return do_release(atoi(argv[2]), atoi(argv[3]));

	if (argc > 1 && strcmp(argv[1], "start") == 0)
		return do_start(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

	if (argc > 1 && strcmp(argv[1], "stop") == 0)
		return do_stop(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

	if (argc > 1 && strcmp(argv[1], "monitor") == 0)
		return test_monitor(atoi(argv[2]), TRUE);

	if (argc > 1 && strcmp(argv[1], "nonroot") == 0)
		return do_nonroot(atoi(argv[2]));

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = on_quit;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("Cannot register signal handler");

	test("insmod interceptor.ko %s", "", system("insmod interceptor.ko") == 0);
	test("bad MY_SYSCALL args%s", "",  vsyscall_arg(MY_CUSTOM_SYSCALL, 3, 100, 0, 0) == -EINVAL);
	do_intercept(MY_CUSTOM_SYSCALL, -EINVAL);
	do_release(MY_CUSTOM_SYSCALL, -EINVAL);
	do_intercept(-1, -EINVAL);
	do_release(-1, -EINVAL);
	do_intercept(__NR_exit, 0);
	do_release(__NR_exit, 0);

	test_syscall(SYS_open);
	/* The above line of code tests SYS_open.
	   Feel free to add more tests here for other system calls,
	   once you get everything to work; check Linux documentation
	   for other syscall number definitions.  */

	do_intercept(SYS_open, 0);
	do_start(SYS_open, -1, 0);
	test_monitor(SYS_open, TRUE);
	test("rmmod interceptor.ko %s", "", system("rmmod interceptor") == 0);
	test_monitor(SYS_open, FALSE);
	return 0;
}
