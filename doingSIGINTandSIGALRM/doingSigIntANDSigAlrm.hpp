//
//  doingSigIntANDSigAlrm.hpp
//  doingSIGINTandSIGALRM
//
//  Created by Clifford Campo on 9/29/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//

#ifndef doingSigIntANDSigAlrm_hpp
#define doingSigIntANDSigAlrm_hpp

#include <atomic>
#include <iostream>
#include <queue>
#include <chrono>
#include <exception>
#include <ctime>
#include <csignal>
#include <map>
#include <list>
#include <locale>
#include <sstream> //StringStream
#include <setjmp.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include "signalDescription.hpp"  //The base class which from which MySig derives its
#define zERO 0
#define SIGUNKNOWNJUMP  zERO
#define SIGINTSETJUMP   1
#define SIGALRMSETJUMP  2
#define SIGUSR1SETJUMP  3
#define SIZECIRCULARBUFFER  NUMBER_OF_ELEMENTS_IN_CIRCULAR_ARRAY
#define ONE  1
#define ZERO zERO
#define SLEEPTIME  10
#define TIMESIZE   32
using namespace  std;
typedef struct sigmap {
    int8_t signumber;
    const char *signame;
    const char *sigaction;
    const char *signalCause;
} sigmap;
typedef struct twoUINT8s {
    atomic<uint8_t> atomic_flag;
    uint8_t siteid; //Currently, siteid has a max value of 28-decimal, so 8 bits should be sufficient to hold siteid
} twoINT8s;
typedef struct circularArray {
#define NUMBER_OF_ELEMENTS_IN_CIRCULAR_ARRAY 28
#define MAX_INDEX_NUMBER NUMBER_OF_ELEMENTS_IN_CIRCULAR_ARRAY-1
#define FLAGS_NOTINUSE      0b00000000
#define FLAGS_INUSE         0b10000000
#define FLAGS_SIGNUMBERMASK 0b10011111
#define VLAGS_UNKNOWNSIG    0b01000000
    char c1 = '\0';
    char c2 = '\0';
    union u2x8 {
        u_int16_t flag;
        twoINT8s int8s; //see above struct, named twoINT8s, for definition of twoINT8s
    } u2x8;
//    int8_t entryNumber;
//    int8_t signal; //Range from 0 to 31
    pid_t  myprocessid = ATOMIC_VAR_INIT(0); //Initialize with myprocessid of 0;
    std::chrono::high_resolution_clock::time_point start_tp; //Note that time point is 12 hex characters, or 48-bits, long! \
    representing 2^48 microseconds, since ratio looks like its defining 1 second = 1000000:\
__d_ std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1, 1000000> > >::duration
    std::chrono::high_resolution_clock::time_point stop_tp;
    circularArray *ptrToNext;
    circularArray *ptrToPrevious;
} circularArray;
#ifdef QUEUE
typedef struct  {
    unsigned int entryNumber;
    int  mysignal;
    int  myprocessid;
    std::chrono::system_clock::time_point timePoint; //Note that time point is 12 hex characters, or 48-bits, long! \
    representing 2^48 microseconds, since ratio looks like its defining 1 second = 1000000:\
    __d_    std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1, 1000000> > >::duration
} QE;
template <class T, class Container = deque<T> > class myqueue;
#endif

class MySig : public SI { //Class named MySig first Inherits the base class, named SI. This means that by instatiating an instance  \
    of MySig, the MySig CONSTRUCTOR will instatiate an instance of the SI class and, thus, the SI CONSTRUCTOR will execute.
    SI si; 
typedef void (*p)(MySig *,unsigned int);
#define MEMBERFUNCTIONSTHATDOSIGHANDLERPOSTPROCESSING 0
#define MEMBER_FUNCTIONS_THAT_WRANGLE_SIGHANDLER_EXECUTION_TIME 1
private:

    
public:
    unsigned int sizeOfMySig;
    bool d;
    bool cas;
    pid_t myProcessID;
    p ptrToFunction[4][2];

#ifdef QUEUE
    QE oneQueueElement;
    ostringstream oss;
#endif
    struct std::tm tt;
    long setJumpValue;
    bool dojustonce;
    string timeReturned;
    char work[32];

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
#ifdef QUEUE
    int qvalue;
#endif
    map<int, p> ptf;
    long accumulateSIGALRMtimes  = 0;
    long accumulateSIGALRMcounts = 0;
    int envVariableCounter=0;
    string formattedTime;
    tm* nowTM;
    std::time_t t_t;
    locale locC;
    MySig(void) noexcept(true);
    ~MySig(void);
    void signalDescription (int );
    unsigned char *getQstart(void);
    static void sigUNKNOWN_handler(int );
    static void sigusr1_handler(int);
    static void sigalarm_handler(int);   //This SIGALRM handler
    static void INThandler(int);               /* the Ctrl-C handler       */
    static void doSIGINT(MySig *,unsigned int);
    static void doSIGALRM(MySig *,unsigned int);
    static void doSIGUSR1(MySig *,unsigned int);
    static void doUNKNOWNSIGNAL(MySig *,unsigned  int);
    static void manageCircularArray(MySig *,unsigned int );
    static void wrangleSIGALRMstartTime(MySig *, unsigned int );
    static void wrangleSIGINTstartTime(MySig *, unsigned int );
    static void wrangleSIGUSR1startTime(MySig *, unsigned int );
    static void wrangleSIGUNKNOWNstartTime(MySig *, unsigned int);
    int indexOfActiveElement = 0;
    circularArray A[NUMBER_OF_ELEMENTS_IN_CIRCULAR_ARRAY];
    circularArray* ptr_begin = &A[0];
    circularArray* ptr_end   = &A[NUMBER_OF_ELEMENTS_IN_CIRCULAR_ARRAY-1];
    circularArray* ptr_Active_Element = ptr_begin;
    atomic<circularArray *>ptr_Available_Element; // = ptr_begin;
/*
    atomic<circularArray *>pae;
    circularArray *paeWAS;
    circularArray *pAE;
*/
//
#ifdef QUEUE
    list<QE> *ptrl;
    list<QE> l;
    queue<QE> *ptrq; //Points to `q`
    queue<QE> q; //Queue is a queue of elements of type QE which is a struct as defined via the above `typedef struct` statement;
#endif
}; 
#endif /* doingSigIntANDSigAlrm_hpp */
