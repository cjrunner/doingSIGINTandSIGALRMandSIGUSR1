//
//  doingSigIntANDSigAlrm.hpp
//  doingSIGINTandSIGALRM
//
//  Created by Clifford Campo on 9/29/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//

#ifndef doingSigIntANDSigAlrm_hpp
#define doingSigIntANDSigAlrm_hpp



#include <iostream>
#include <queue>
#include <chrono>
#include <exception>
#include <ctime>
#include <locale>
#include <sstream> //StringStream
#include <stdio.h>
#include <signal.h> 
#include <setjmp.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#define SIGINTSETJUMP   1
#define SIGALRMSETJUMP  2
#define SIGUSR1SETJUMP  3
using namespace  std;
typedef struct  {
    unsigned int entryNumber;
    int  mysignal;
    int  myprocessid;
    std::chrono::system_clock::time_point timePoint; //Note that time point is 12 hex characters, or 48-bits, long! \
    representing 2^48 microseconds, since ratio looks like its defining 1 second = 1000000:\
    __d_    std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1, 1000000> > >::duration
} QE;
template <class T, class Container = deque<T> > class myqueue;
class MySig {
private:
    
    unsigned char MySigstart = 'S'; 
    unsigned char rest[14] = {'T','A','R','T',' ','O','F',' ','m','y','S','I','G','\0'};
    static const int ONE = 1;
    static const int SLEEPTIME = 10;
    static const int TIMESIZE = 32;
    
public:
    QE oneQueueElement;
    struct std::tm tt;
    long setJumpValue;
    bool dojustonce;
    string timeReturned;
    char work[32];
    ostringstream oss;
//    string oss;
    char  c;
    siginfo_t sigINFO;
//    char buf[sizeof "2011-10-08T07:07:09Z"];
    unsigned char *ptrqstart;
    const int ALARMTIME = 5;
    sigset_t mask, oldmask; //Signal masks
    sig_t WAITMASK;
    int sizeofbuffer;
//    const sigset_t *ptr_sigset;
    sigset_t sigalrm = SIGALRM;
    sigset_t sigusr1 = SIGUSR1;
    char mytime[TIMESIZE];
//    sigset_t sigset; // = sigusr1 || sigalrm;
    int theSignal;
    int *ptr_theSignal;
    unsigned int whileLoopCounter = 0;
    const string format = "%Y-%m-%dT%H:%M:%S %Z";
//    const time_put<char>& tpC;
//    int waitFlag=0;
    int rc;
    size_t rcsz;
//    pid_t waitPID;
    std::string timePointAsString (const std::chrono::system_clock::time_point& );
//    int qvalue;
    int envVariableCounter=0;
    string formattedTime;
    tm* nowTM;
    std::time_t t_t;
    locale locC;
    MySig(void) noexcept(true);
    ~MySig(void);
    unsigned char *getQstart(void);
    static void sigalarm_handler(int);   //This SIGALRM handler
    static void INThandler(int);               /* the Ctrl-C handler       */
    static void sigusr1_handler(int); // the SIGSUR1 signal handler

    queue<QE> *ptrq; //Points to `q`
    unsigned char qs[8] = {'q',' ','S','T','A','R','T','-'};
    unsigned char qsstart = '>';
    queue<QE> q; //Queue is a queue of elements of type QE which is a struct as defined via the above `typedef struct` statement;
    unsigned char qsend = '<';
    unsigned char qendc[13] = {'-','E','N','D',' ','O','F',' ','m','y','S','I','G'};
    unsigned char qend = '\0';
}; 
#endif /* doingSigIntANDSigAlrm_hpp */
