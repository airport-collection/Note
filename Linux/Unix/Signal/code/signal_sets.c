#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <signal.h>
#include <string.h>

static void printSigset(FILE *of, const char *prefix, const sigset_t *sigset)
{
    int cnt = 0;
    for(int sig = 1; sig < NSIG; ++sig)
    {
        if(sigismember(sigset, sig))
        {
            ++cnt;
            fprintf(of, "%s %d (%s)\n", prefix, sig, strsignal(sig));
        }
    }
    if(cnt == 0)
        fprintf(of, "%s <empty signal set>.\n", prefix);
}

static void printSigMask()
{
    sigset_t oldset;
    int res = sigprocmask(SIG_SETMASK, NULL, &oldset);
    if(res == -1)
    {
        fprintf(stderr, "get signal mask failed!\n");
        exit(1);
    }
    FILE *fstdout = fdopen(1, "w");
    printSigset(fstdout, "", &oldset);
    
}

static void printPendingSigs()
{
    sigset_t pSet;
    if(sigpending(&pSet) < 0)
    {
        fprintf(stderr, "calliing sigpending error!");
        exit(1);
    }
    FILE *fstdout = fdopen(1, "w");
    printSigset(fstdout, "", &pSet);
}

static void testSigMask()
{
    sigset_t blockSet, prevSet;
    sigemptyset(&blockSet);
    sigaddset(&blockSet, SIGBUS);
    sigaddset(&blockSet, SIGILL);
    if(sigprocmask(SIG_BLOCK, &blockSet, &prevSet) == -1)
    {
        fprintf(stderr, "set the signal mask failed!\n");
        exit(1);
    }
    printSigMask();
    if(sigprocmask(SIG_SETMASK, &prevSet, NULL) == -1)
    {
        fprintf(stderr, "reset the signal mask failed!\n");
        exit(1);
    } 
}

static void testPendingSigs()
{
    printPendingSigs(); 
}

int main(int argc, char**argv)
{
    testPendingSigs();
    exit(0);
}
