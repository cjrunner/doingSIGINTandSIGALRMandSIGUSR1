//
//  main.cpp
//  doingSIGINTandSIGALRM
//
//  Created by Clifford Campo on 9/29/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//
/*   UNIX-defined Signals
 ID  NAME            Default Action          Description
 01 SIGHUP          terminate process       terminal line hangup
 02 SIGINT          terminate process       interrupt program
 03 SIGQUIT         create core image       quit program
 04 SIGILL          create core image       illegal instruction
 05 SIGTRAP         create core image       trace trap
 06 SIGABRT         create core image       abort(3) call (formerly SIGIOT)
 07 SIGEMT          create core image       emulate instruction executed
 08 SIGFPE          create core image       floating-point exception
 09 SIGKILL         terminate process       kill program
 10 SIGBUS          create core image       bus error
 11 SIGSEGV         create core image       segmentation violation
 12 SIGSYS          create core image       non-existent system call invoked
 13 SIGPIPE         terminate process       write on a pipe with no reader
 14 SIGALRM         terminate process       real-time timer expired
 15 SIGTERM         terminate process       software termination signal
 16 SIGURG          discard signal          urgent condition present on socket
 17 SIGSTOP         stop process            stop (cannot be caught or ignored)
 18 SIGTSTP         stop process            stop signal generated from keyboard
 19 SIGCONT         discard signal          continue after stop
 20 SIGCHLD         discard signal          child status has changed
 21 SIGTTIN         stop process            background read attempted from control terminal
 22 SIGTTOU         stop process            background write attempted to control terminal
 23 SIGIO           discard signal          I/O is possible on a descriptor (see fcntl(2))
 24 SIGXCPU         terminate process       cpu time limit exceeded (see setrlimit(2))
 25 SIGXFSZ         terminate process       file size limit exceeded (see setrlimit(2))
 26 SIGVTALRM       terminate process       virtual time alarm (see setitimer(2))
 27 SIGPROF         terminate process       profiling timer alarm (see setitimer(2))
 28 SIGWINCH        discard signal          Window size change
 29 SIGINFO         discard signal          status request from keyboard
 30 SIGUSR1         terminate process       User defined signal 1
 31 SIGUSR2         terminate process       User defined signal 2

 
*/
#include "doingSigIntANDSigAlrm.hpp"
time_t t;
bool operatorWantsToQuit;
jmp_buf  JumpBufferForSIGINT;                    /* a jump buffer            */
bool quitNOW;
// NOT USED: atomic_int markOnTheWall=ATOMIC_VAR_INIT(0);
queue<QE> *ptrQ;
int entryNumber=0;
// NOT USED: QE *ptrQE;
pid_t myProcessID;
QE *ptrOneQElement;
unsigned char  *msStart;
unsigned char  *msEnd;
unsigned char *qStart;
unsigned char *qEnd;
sigset_t mask, oldmask; //Signal masks
char  in_character;
#ifdef PAUSE
const char *whichwaitmethod = "pause()";
#else
const char *whichwaitmethod = "sigsuspend";
#endif
/* ---------------------------------------------------------------- */
/*                   main program starts here                       */
/* ---------------------------------------------------------------- */

int   main(int argc, const char **ptrToArrayOfPointersToCString, const char **ptrToArrayOfPointersPointingToCStrings) {
    ::myProcessID = getpid();
    int envVariableCounter;
    jmp_buf *jbSI;
    jbSI = &JumpBufferForSIGINT;
    std::cout << "Hello out there TV-folk, this is program, named \n" << *(ptrToArrayOfPointersToCString + 0) << \
    ", and having a processor id of " << ::myProcessID << ". is ready to demonstrate signal handling. \n" \
    "JumpBufferForSIGINT is an array of " << sizeof(JumpBufferForSIGINT) << " entries for holding the state of the machine" << \
    " and used by longjump for returning to the instruction, just after the setjmp instruction, from a signal handler. " << \
    "The setjmp instruction populates JumpBufferFor with the state of the machine at the time of execution of setjmp. Using " << \
    "the " << ::whichwaitmethod << " method for waiting on either a SIGINT, SIGALRM, or SIGUSR1 signal to occur" << std::endl;
    envVariableCounter = 0;
    //Output the environment variables
    while (*(ptrToArrayOfPointersPointingToCStrings + envVariableCounter) != 0) {
        std::cout << envVariableCounter << ". " << *(ptrToArrayOfPointersPointingToCStrings + envVariableCounter) << \
        " at location " << endl;
        envVariableCounter++;
    }
    MySig *ms = new MySig();
    
    ms->ptrq = ::ptrQ;
    ::msStart = ms->getQstart(); //Have msStart point to the START of this instance object.
    ::msEnd =  &ms->qend; //Have msEnd point to the END of this instance object.
    ::qStart = &ms->qsstart;
    ::qEnd = &ms->qend;
    cout << "A. At the START of execution class MySig starts at location " << &msStart << " and ends at location " << &msEnd << \
    " for a total length of " << (&msEnd - &msStart)*sizeof(ms->ptrq) << \
    " bytes, while the queue, named q, within this class starts at location " << &::qStart << " and ends at location " << \
    &::qEnd  << endl;
    
    signal(SIGINT, MySig::INThandler);        /* install Ctrl-C handler   */
    // install our interrupt handler
    signal(SIGALRM, MySig::sigalarm_handler); // install SIGALRM signal handler
    // install our interrupt handler
    signal(SIGUSR1, MySig::sigusr1_handler); // install SIGUSR1 signal handler
    
    while ( (!operatorWantsToQuit) ) {                        /* loop & wait Ctrl-C       */
        
        ms->setJumpValue=setjmp(JumpBufferForSIGINT);
        ; //Signal Handlers return here per the previous instruction's setjmp instruction. \
        At this point the signal handler should have pushed onto the FIFO Queue the element describing \
        itself including the time of day of the interrupt
/*
        if (ms->ptrq->size() > 0 ) {
            if (ms->ptrq->front().mysignal == SIGALRM) {
                cout << "Reestablishing SIGALRM signal handler" << endl;
                SIGALRM, MySig::sigalarm_handler); //Reestablish SIGALRM Signal Handler
            } else if (ms->ptrq->front().mysignal == SIGUSR1) {
                cout << "Reestablishing SIGUSR1 signal handler" << endl;
                signal(SIGUSR1, MySig::sigalarm_handler); //Reestablish SIGUSR1 Signal Handler
            } else if (ms->ptrq->front().mysignal == SIGINT) {
                cout << "Reestablishing SIGINT signal handler" << endl;
                signal(SIGINT, MySig::sigalarm_handler); //Reestablish SIGINT Signal Handler
            } else {
                cout << "ERROR: Don't know why we got here, nothing on queue name q (i.e., Items on q = " << ms->ptrq->size() << \
                ") while ms->setJumpValue = " << ms->setJumpValue << endl;
            }
        }
*/
        ms->whileLoopCounter++;
        cout << "The size of the queue, named q, is " << ms->q.size()  << " entries " << " and of length " << \
        &ms->qend - ms->getQstart() << " bytes." << endl;
        if (quitNOW) {
            std::cout << "It looks like flag quitNOW got set to " << quitNOW << " by the SIGUSR1 signal handler " << std::endl;
            break;
        }
        cout << ms->whileLoopCounter << ". alarm requested t= " << time(NULL) - t << endl; // write start time
        alarm(ms->ALARMTIME); 


        
        if (ms->dojustonce) {
            for (envVariableCounter = 0; envVariableCounter < ms->sizeofbuffer; envVariableCounter++ ) { 
                cout << envVariableCounter << ". " << std::hex << *(JumpBufferForSIGINT + envVariableCounter) << std::dec << endl;
            }
            ms->dojustonce=false;
        }
        /* set a return mark   */
//       ms->rc = sigwait(ms->ptr_sigset, nullptr);
/* We know pause() works, but let's try sigprocmask-SIG_BLOCK/sussuspend/sigprocmask-SIG_UNBLOCK */
#ifdef PAUSE
        pause(); //Do this if the Xcode's compiler preprocessor is defined
#else
/* Wait for a signal to arrive. */
        
        ms->rc = sigsuspend(&ms->oldmask);
        
#endif
        std::cout << ms->whileLoopCounter <<". We got an interrupt other than the 3 Amigos (SIGUSR1, SIGALRM, SIGINT). setJumpValue: " << \
            ms->setJumpValue << ", the size of the queue is " << ptrQ->size() << " entries. Size of queue, named q, is " << \
            &ms->qend - ms->getQstart() << " bytes. ms->ptr_theSignal = " << ms->ptr_theSignal << \
        ", the return code from " << *::whichwaitmethod << " is " << ms->rc << endl; //This code gets executed if we get an interrupt other than the 3 Amigos
//        if (ms->ptrq->front().mysignal == 2) signal(SIGALRM, MySig::sigalarm_handler); // install SIGALRM signal handler
//        sleep(ms->SLEEPTIME);                   /* pause until Ctrl-C comes */
//        if (ms->ptrq->empty() == false) {
//           ms->timeReturned = ms->timePointAsString(ms->ptrq->front().timePoint);
//        }
    } //End of while( (!operatorWantsToQuit) ) Loop
    ::msStart = ms->getQstart(); //Have msStart point to the START of this instance object.
    ::msEnd =  &ms->qend; //Have msEnd point to the END of this instance object.
    ::qStart = &ms->qsstart;
    ::qEnd = &ms->qend;
     
    while ( ms->ptrq->size() > 0 ) {
        cout << ms->envVariableCounter << ". Variable at top of queue is {" << &ms->ptrq->front().entryNumber << \
        "(" << ms->ptrq->front().entryNumber <<  "), " << &ms->ptrq->front().mysignal << \
        "(" << ms->ptrq->front().mysignal <<     "), " << &ms->ptrq->front().myprocessid << \
        "(" <<  ms->ptrq->front().myprocessid << "), " << &ms->ptrq->front().timePoint << \
        "(" <<  ms->timePointAsString(ms->ptrq->front().timePoint)   << ")}. and the size of the queue is " << ms->ptrq->size() << \
        " entries prior to popping-off the top element." <<endl;
        ms->ptrq->pop();
        ms->envVariableCounter++;
        
    }
    cout << "B. At the END of execution, class MySig starts at location " << &msStart << " and ends at location " << &msEnd << \
    " for a total length of " << &msEnd - &msStart << \
    " bytes, while the queue, named q, within this class starts at location " << &::qStart << " and ends at location " << \
    &::qEnd << " for a total length of " << &::qEnd-&::qStart << " bytes. " << endl;
    delete ms; //Remove the pointer to the instance of the the class named MySig
    return 0;
} //END OF MAIN    END OF MAIN    END OF MAIN    END OF MAIN

/* ---------------------------------------------------------------- */
/* FUNCTION  INThandler:                                            */
/*    This function handles the SIGINT (Ctrl-C) signal.             */
/* ---------------------------------------------------------------- */

void  MySig::INThandler(int  sig)
{
    
#ifdef PAUSE
    signal(sig, SIG_IGN);              /* ignore/deactivate Ctrl-C for now, thus preventing recursive stacking of SIGINTs    */
#else
    sigprocmask(SIG_BLOCK, &::mask, &::oldmask);
#endif
    ::ptrOneQElement->entryNumber = ::entryNumber++;
    ::ptrOneQElement->mysignal=SIGINT;
    ::ptrOneQElement->myprocessid = ::myProcessID;
 //   ::ptrOneQElement->timePoint.~time_point(); //Have to do this or else the next instruction will keep appending time onto timePoint
    ::ptrOneQElement->timePoint = std::chrono::system_clock::now(); //Time stamp this entry
    ::ptrQ->push(*ptrOneQElement);
    cout << "SIGINT: Detected Ctrl-C!\tDo you really want to quit? [y/n] " << endl;
    //        c = getchar();
    cin >> ::in_character;
    if ( ::in_character == 'y' || ::in_character == 'Y' ) operatorWantsToQuit = true;
    else {
#ifdef PAUSE
        signal(SIGINT, MySig::INThandler);    /* reactivate SIGINT before return  */
#else
        sigprocmask(SIG_UNBLOCK, &::mask, NULL); //Reactiviate SIGINT + SIGALRM + SIGUSR1
#endif
        longjmp(JumpBufferForSIGINT, SIGINTSETJUMP);       /* return to the setjmp mark*/
    }
}
/* ----------------------------------------------------------------------------------------------------------------------------- */
/* FUNCTION  INThandler:                                                                                                         */
/*    This function handles the SIGALRM signal.                                                                                  */
/* ----------------------------------------------------------------------------------------------------------------------------- */
void MySig::sigalarm_handler(int sig) {
#ifdef PAUSE
    signal(sig, SIG_IGN);              /* ignore/deactivate Ctrl-C for now, thus preventing recursive stacking of SIGINTs    */
#else
    sigprocmask(SIG_BLOCK, &::mask, &::oldmask);
#endif
    cout << "SIGALRM, having signal id of " << sig << ", fired at t=" <<  time(NULL) - t << endl; // write a message when called
    ::ptrOneQElement->entryNumber = ::entryNumber++;
    ::ptrOneQElement->mysignal=SIGALRM;
    ::ptrOneQElement->myprocessid = ::myProcessID;
    ::ptrOneQElement->timePoint = std::chrono::system_clock::now(); //Time stamp this entry
    ::ptrQ->push(*ptrOneQElement);
#ifdef PAUSE
    signal(SIGALRM, MySig::INThandler);    /* reactivate SIGALRM before return  */
#else
    sigprocmask(SIG_UNBLOCK, &::mask, NULL);
#endif
    longjmp(JumpBufferForSIGINT, SIGALRMSETJUMP); /* return to the setjmp mark*/
}
/* ----------------------------------------------------------------------------------------------------------------------------- */
/* FUNCTION  INTERRUPT handler:                                                                                                  */
/*    This function handles the SIGUSR1 signal.                                                                                  */
/* ----------------------------------------------------------------------------------------------------------------------------- */
void MySig::sigusr1_handler(int sig) {
#ifdef PAUSE
    signal(sig, SIG_IGN);              /* ignore/deactivate Ctrl-C for now, thus preventing recursive stacking of SIGINTs    */
#else
    sigprocmask(SIG_BLOCK, &::mask, &::oldmask); //Deactiviate SIGINT + SIGALRM + SIGUSR1
#endif
    cout << "SIGUSR1, having signal id of " << sig << ", fired at t=" <<  time(NULL) - t << endl; // write a message when called
    ::quitNOW = true;
    ::ptrOneQElement->timePoint.~time_point(); //Have to do this or else the next instruction will keep appending time onto timePoint
    ::ptrOneQElement->entryNumber = entryNumber++;
    ::ptrOneQElement->mysignal=SIGUSR1;
    ::ptrOneQElement->myprocessid = ::myProcessID;
    ::ptrOneQElement->timePoint = std::chrono::system_clock::now(); //Time stamp this entry
    ::ptrQ->push(*ptrOneQElement);
#ifdef PAUSE
    signal(SIGUSR1, MySig::INThandler);    /* reactivate SIGUSR1 before return  */
#else
    sigprocmask(SIG_UNBLOCK, &::mask, NULL); //Reactiviate SIGINT + SIGALRM + SIGUSR1
#endif
    longjmp(JumpBufferForSIGINT, SIGUSR1SETJUMP); /* return to the setjmp mark*/
    
}
MySig::~MySig(void) {
    std::cout << "Just entered MySig destructor with variable this set to location " << this << std::endl ;
}

MySig::MySig(void) noexcept(true) {
    ;
    std::cout << "Just entered MySig constructor with variable this set to location " << this << std::endl ;
    
    /* Set up the mask of signals to temporarily block. */
    sigemptyset (&mask); //Make sure this is clear.
   
    
//    sigusr1 = sigmask(SIGUSR1);
 //   sigalrm = sigmask(SIGALRM);
//    WAITMASK = (sig_t)(SIGALRM) | (sig_t)sigmask(SIGUSR1) | (sig_t)sigmask(SIGINT);  //Wait for these signals

    sigaddset (&mask, SIGALRM);
    sigaddset (&mask, SIGUSR1);
    sigaddset (&mask, SIGINT);
    //
//     SIGINT (bit 02)  ---------------------------------+
//     SIGALRM (bit 14) ---------------------+           |
//     SIGUSR1 (bit 30) -----+               |           |
//                           |               |           |
//                           V               V           V
//When done mask looks: "0b00100000000000000010000000000010" (33-bits) or as 0x20002002, or as 0o04000020002
//    sigset = sigusr1 + sigalrm;
    ::mask = this->mask; //Set the global mask so the signal handlers can use it, too.
    sizeofbuffer = sizeof(JumpBufferForSIGINT);
    operatorWantsToQuit=false;
    dojustonce = true;
    ::ptrQ = &this->q; //Establish addressability to multi-element queue
    ::ptrOneQElement = &oneQueueElement;
    ptrqstart = &MySigstart;
    ::msStart = &this->MySigstart; //Have msStart point to the START of this instance object.
    ::msEnd =  &this->qend; //Have msEnd point to the END of this instance object.
    ::qStart = &this->qsstart;
    ::qEnd = &this->qend;
    
//    ptr_sigset = &sigset;
    ptr_theSignal = &theSignal;
    t = time(nullptr); //At the instant of this instruction's execution, set t to the current time of day.
    }
    unsigned char *MySig::getQstart(void) {
        return ptrqstart;
    }
    std::string MySig::timePointAsString (const std::chrono::system_clock::time_point& tpIn) {
//    string timeOut;
//   auto now = chrono::system_clock::now();
        t_t = chrono::system_clock::to_time_t(tpIn);
        nowTM = std::localtime(&t_t);
        rcsz = strftime(mytime, 20, "%Y-%m-%dT%H:%M:%S %Z", nowTM );
        formattedTime = string(mytime);

       std::string ts = ctime(&t_t);
/*
        const time_put<char>& tpC = use_facet<time_put<char>>(locC);
        oss.clear();
        tpC.put (oss, oss, ' ', nowTM, format.c_str(), format.c_str()+format.size() ); //BEWARE that this might concatenate the \
        current value with the previous value(s). We might have to reset oss before prior to the oss command (eg. oss.clear())
*/
        return formattedTime;
    }
