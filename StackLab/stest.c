/* Implementation of testing code for stack code */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>

/* Our program needs to use regular malloc/free */
#define INTERNAL 1
#include "harness.h"

#include "stack.h"

#include "report.h"
#include "console.h"

/***** Settable parameters *****/

/*
  How large is a stack before it's considered big.
  This affects how it gets printed
  and whether cautious mode is used when freeing the list
*/
#define BIG_STACK 30

int big_stack_size = BIG_STACK;

/******* Global variables ******/

/* Queue being tested */
stack *s = NULL;
/* Number of elements in stack */
size_t scnt = 0;

/* How many times can stack operations fail */
int fail_limit = BIG_STACK;
int fail_count = 0;

/****** Forward declarations ******/
static bool show_stack(int vlevel);
bool do_new(int argc, char *argv[]);
bool do_free(int argc, char *argv[]);
bool do_push(int argc, char *argv[]);

bool do_pop(int argc, char *argv[]);

bool do_reverse(int argc, char *argv[]);
bool do_size(int argc, char *argv[]);
bool do_show(int argc, char *argv[]);

static void stack_init();

static void console_init() {
    add_cmd("new", do_new,
	    "                | Create new stack");
    add_cmd("free", do_free,
	    "                | Delete stack");
    add_cmd("push", do_push,
	    " v [n]          | Insert v at top of stack n times (default: n == 1)");
    add_cmd("pop", do_pop,
	    " [v]            | Remove from top of stack.  Optionally compare to expected value v");
    add_cmd("reverse", do_reverse,
	    "                | Reverse stack");
    add_cmd("size", do_size,
	    " [n]            | Compute stack size n times (default: n == 1)");
    add_cmd("show", do_show,
	    "                | Show stack contents");
    add_param("malloc", &fail_probability, "Malloc failure probability percent", NULL);
    add_param("fail", &fail_limit, "Number of times allow stack operations to return false", NULL);
}

bool do_new(int argc, char *argv[])
{
    bool ok = true;
    if (s != NULL) {
	report(3, "Freeing old stack");
	ok = do_free(argc, argv);
    }
    error_check();
    if (exception_setup(true))
	s = s_new();
    exception_cancel();
    scnt = 0;
    show_stack(3);
    return ok && !error_check();
}

bool do_free(int argc, char *argv[])
{
    bool ok = true;
    if (s == NULL)
	report(3, "Warning: Calling free on null stack");
    error_check();
    if (scnt > big_stack_size)
	set_cautious_mode(false);
    if (exception_setup(true))
	s_free(s);
    exception_cancel();
    set_cautious_mode(true);
    s = NULL;
    scnt = 0;
    show_stack(3);
    size_t bcnt = allocation_check();
    if (bcnt > 0) {
	report(1, "ERROR: Freed stack, but %lu blocks are still allocated", bcnt);
	ok = false;
    }
    return ok && !error_check();
}

bool do_push(int argc, char *argv[])
{
    int val;
    int reps = 1;
    int r;
    bool ok = true;
    if (argc != 2 && argc != 3) {
	report(1, "%s needs 1-2 arguments", argv[0]);
	return false;
    }
    if (!get_int(argv[1], &val)) {
	report(1, "Invalid insertion value '%s'", argv[1]);
	return false;
    }
    if (argc == 3) {
	if (!get_int(argv[2], &reps)) {
	    report(1, "Invalid number of insertions '%s'", argv[2]);
	    return false;
	}
    }
    if (s == NULL)
	report(3, "Warning: Calling insert top on null stack");
    error_check();
    if (exception_setup(true)) {
	for (r = 0; ok && r < reps; r++) {
	    bool rval = s_push(s, val);
	    if (rval) {
		scnt++;
	    } else {
		fail_count++;
		if (fail_count < fail_limit)
		    report(2, "Insertion of %d failed", val);
		else {
		    report(1, "ERROR: Insertion of %d failed (%d failures total)", val, fail_count);
		    ok = false;
		}
	    }
	    ok = ok &&!error_check();
	}
    }
    exception_cancel();
    show_stack(3);
    return ok;
}



bool do_pop(int argc, char *argv[])
{
    int val, ival, eval;
    bool check = argc > 1;
    bool ok = true;
    val = ival = random();
    if (check) {
	if (!get_int(argv[1], &eval)) {
	    report(1, "Invalid comparison value '%s'", argv[1]);
	    return false;
	}
    }
    if (s == NULL)
	report(3, "Warning: Calling remove top on null stack");
    else if (s->top == NULL)
	report(3, "Warning: Calling remove top on empty stack");
    error_check();
    bool rval = false;
    if (exception_setup(true))
	rval = s_pop(s, &val);
    exception_cancel();
    if (rval) {
	
	
	scnt--;
    } else {
	fail_count++;
	if (!check && fail_count < fail_limit)  {
	    report(2, "Removal from stack failed");
	} else {
	    report(1, "ERROR:  Removal from stack failed (%d failures total)", fail_count);
	    ok = false;
	}
    }
    if (ok && check && val != eval) {
	report(1, "ERROR:  Removed value %d != expected value %d", val, eval);
	ok = false;
    }
    show_stack(3);
    return ok && !error_check();
}



bool do_reverse(int argc, char *argv[])
{
    if (s == NULL)
	report(3, "Warning: Calling reverse on null stack");
    error_check();
    if (exception_setup(true))
	s_reverse(s);
    exception_cancel();
    show_stack(3);
    return !error_check();
}

bool do_size(int argc, char *argv[])
{

    int reps = 1;
    int r;
    bool ok = true;
    if (argc != 1 && argc != 2) {
	report(1, "%s needs 0-1 arguments", argv[0]);
	return false;
    }
    if (argc == 2) {
	if (!get_int(argv[1], &reps)) {
	    report(1, "Invalid number of calls to size '%s'", argv[2]);
	}
    }
    int cnt = 0;
    if (s == NULL)
	report(3, "Warning: Calling size on null stack");
    error_check();
    if (exception_setup(true)) {
	for (r = 0; ok && r < reps; r++) {
	    cnt = s_size(s);
	    ok = ok && !error_check();
	}
    }
    exception_cancel();
    if (ok) {
	if (scnt == cnt) {
	    report(2, "Stack size = %d", cnt);
	} else {
	    report(1, "ERROR:  Computed stack size as %d, but correct value is %d",
		   cnt, (int) scnt);
	    ok = false;
	}
    }
    show_stack(3);
    
    return ok && !error_check();
}

static bool show_stack(int vlevel)
{
    bool ok = true;
    if (verblevel < vlevel)
	return true;
    int cnt = 0;
    if (s == NULL) {
	report(vlevel, "s = NULL");
	return true;
    }
    report_noreturn(vlevel, "s = [");
    s_node *e = s->top;
    if (exception_setup(true)) {
	while (ok && e && cnt < scnt) {
	    if (cnt < big_stack_size)
		report_noreturn(vlevel, cnt == 0 ? "%d" : " %d", e->value);
	    e = e->next;
	    cnt++;
	    ok = ok && !error_check();
	}
    }
    exception_cancel();
    if (!ok) {
	report(vlevel, " ... ]");
	return false;
    }
    if (e == NULL) {
	if (cnt <= big_stack_size)
	    report(vlevel, "]");
	else
	    report(vlevel, " ... ]");
    } else {
	report(vlevel, " ... ]");
	report(vlevel, "ERROR:  Either list has cycle, or stack has more than %d elements",
	       scnt);
	ok = false;
    }
    return ok;
}

bool do_show(int argc, char *argv[])
{
    return show_stack(0);
}

/* Signal handlers */
void sigsegvhandler(int sig) {
    trigger_exception("Segmentation fault occurred.  You dereferenced a NULL or invalid pointer");
}

void sigalrmhandler(int sig) {
    trigger_exception("Time limit exceeded.  Either you are in an infinite loop, or your code is too inefficient");
}


static void stack_init() {
    fail_count = 0;
    s = NULL;
    signal(SIGSEGV, sigsegvhandler);
    signal(SIGALRM, sigalrmhandler);
}


static bool stack_quit(int argc, char *argv[]) {
    report(3, "Freeing stack");
    if (scnt > big_stack_size)
	set_cautious_mode(false);
    if (exception_setup(true))
	s_free(s);
    exception_cancel();
    set_cautious_mode(true);
    size_t bcnt = allocation_check();
    if (bcnt > 0) {
	report(1, "ERROR: Freed stack, but %lu blocks are still allocated", bcnt);
	return false;
    }
    return true;
}


static void usage(char *cmd) {
    printf("Usage: %s [-h] [-f IFILE][-v VLEVEL][-l LFILE]\n",  cmd);
    printf("\t-h         Print this information\n");
    printf("\t-f IFILE   Read commands from IFILE\n");
    printf("\t-v VLEVEL  Set verbosity level\n");
    printf("\t-l LFILE   Echo results to LFILE\n");
    exit(0);
}

#define BUFSIZE 256

int main(int argc, char *argv[]) {
    /* To hold input file name */
    char buf[BUFSIZE];
    char *infile_name = NULL;
    char lbuf[BUFSIZE];
    char *logfile_name = NULL;
    int level = 4;
    int c;

    while ((c = getopt(argc, argv, "hv:f:l:")) != -1) {
	switch(c) {
	case 'h':
	    usage(argv[0]);
	    break;
	case 'f':
	    infile_name = strncpy(buf, optarg, BUFSIZE-1);
	    buf[BUFSIZE-1] = '\0';
	    break;
	case 'v':
	    level = atoi(optarg);
	    break;
	case 'l':
	    logfile_name = strncpy(lbuf, optarg, BUFSIZE-1);
	    lbuf[BUFSIZE-1] = '\0';
	    break;
	default:
	    printf("Unknown option '%c'\n", c);
	    usage(argv[0]);
	    break;
	}
    }
    stack_init();
    init_cmd();
    console_init();
    set_verblevel(level);
    if (level > 1) {
	set_echo(true);
    }
    if (logfile_name)
	set_logfile(logfile_name);
    add_quit_helper(stack_quit);
    bool ok = true;
    ok = ok && run_console(infile_name);
    ok = ok && finish_cmd();
    return ok ? 0 : 1;
}
