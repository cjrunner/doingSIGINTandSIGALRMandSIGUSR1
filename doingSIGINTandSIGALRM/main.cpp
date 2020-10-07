//
//  main.cpp
//  doingSIGINTandSIGALRM
//
//  Created by Clifford Campo on 9/29/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//


#include "doingSigIntANDSigAlrm.hpp"
#include "signalDescription.hpp"
time_t t;
bool operatorWantsToQuit;
jmp_buf  JumpBufferForSIGINT;                    /* a jump buffer            */
bool quitNOW;
// bool d;
// NOT USED: atomic_int markOnTheWall=ATOMIC_VAR_INIT(0);
//queue<QE> *ptrQ;
int entryNumber=0;
#ifdef QUEUE
QE *ptrQE;
QE *ptrOneQElement;
#endif
unsigned char *msStart;
unsigned char *msEnd;
unsigned char *qStart;
unsigned char *qEnd;
unsigned int  whichJump=0;
int8_t someSignal; //Signals are numbered from 0 to 31, requiring just 5 bits of storage. See #define FLAGS_SIGNUMBERMASK
std::chrono::high_resolution_clock::time_point interrupt_Handler_SIGINT_tp_start;
std::chrono::high_resolution_clock::time_point interrupt_Handler_SIGUSR1_tp_start;
std::chrono::high_resolution_clock::time_point interrupt_Handler_SIGALRM_tp_start;
std::chrono::high_resolution_clock::time_point interrupt_Handler_SIGUNK_tp_start;
sigset_t mask, oldmask; //Signal masks
char  in_character;
#ifdef PAUSE
const char *whichwaitmethod = "pause()";
#else
const char *whichwaitmethod = "sigsuspend";
#endif
// main Entry Point      main Entry Point      main Entry Point      main Entry Point      main Entry Point      main Entry Point
int   main(int argc, const char **ptrToArrayOfPointersToCString, const char **ptrToArrayOfPointersPointingToCStrings) {
    
    int envVariableCounter;
    jmp_buf *jbSI;
    jbSI = &JumpBufferForSIGINT;
    MySig *ms = new MySig(); // Note that MySig is a class derived from SI.
#ifdef DEBUG
//    SI *sigInfo = new SI();
    ms->SI::signalDescription(SIGINT);  //Need the scope resolution signifier, SI::, in order to make this main program compile \
    successfully with the base class, named SI. With just ms->signalDescription(SIGINT) the linker projectile vomits
    ms->SI::signalDescription(SIGALRM);
    ms->SI::signalDescription(SIGUSR1);

    cout << "Hello out there TV-folk, this is program, named \n" << *(ptrToArrayOfPointersToCString + 0) << \
    ", and having a processor id of " << getpid() << ". is ready to demonstrate signal handling. \n" \
    "JumpBufferForSIGINT is an array of " << sizeof(JumpBufferForSIGINT) << " entries for holding the state of the machine" << \
    " and used by longjump for returning to the instruction, just after the setjmp instruction, from a signal handler. " << \
    "The setjmp instruction populates JumpBufferFor with the state of the machine at the time of execution of setjmp. Using " << \
    "the " << ::whichwaitmethod << " method for waiting on either a SIGINT, SIGALRM, or SIGUSR1 signal to occur" << std::endl;
#endif
    envVariableCounter = 0;
    //Output the environment variables. When in DEBUG mode, there are >23 environment variables. When not in DEBUG mode, then \
    there are about 21 environment variables
    while (*(ptrToArrayOfPointersPointingToCStrings + envVariableCounter) != 0) {
        if (ms->d) cout << envVariableCounter << ". " << *(ptrToArrayOfPointersPointingToCStrings + envVariableCounter) << \
        " at location " << (ptrToArrayOfPointersPointingToCStrings + envVariableCounter) << endl;
        envVariableCounter++;
    }
    
#ifdef QUEUE //Should we do queue or circular array processing, can have one or the other but not both.
//    ms->ptrq = ::ptrQ;
    ::msStart = ms->getQstart(); //Have msStart point to the START of this instance object.
    ::msEnd =  &ms->qend; //Have msEnd point to the END of this instance object.
    ::qStart = &ms->qsstart;
    ::qEnd = &ms->qend;
//    if (ms->d) cout << "A. At the START of execution class MySig starts at location " << &msStart << \
    " and ends at location " << &msEnd << " for a total length of " << (&msEnd - &msStart)*sizeof(ms->ptrq) << \
    " bytes, while the queue, named q, within this class starts at location " << &::qStart << " and ends at location " << \
    &::qEnd  << endl;
#endif

    while ( (!operatorWantsToQuit) ) {  // loop & then wait, either via pause or sigsuspend, depending upon the presence or \
        absence of the PAUSE precompiler directive for a signal to come through
        ms->setJumpValue=setjmp(JumpBufferForSIGINT); //This is so the operating system will return to the next instruction when \
        any of the three signal handlers completes.
        ms->ptrToFunction[::whichJump][0](ms, ::whichJump);

        ms->whileLoopCounter++;
//        if (ms->d) cout << "The size of the queue, named q, is " << ms->q.size()  << " entries " << " and of length " << \
        &ms->qend - ms->getQstart() << " bytes." << endl;
        if (quitNOW) {
            if (ms->d) cout << "It looks like flag quitNOW got set to " << quitNOW << " by the SIGUSR1 signal handler " << std::endl;
            break;
        }
        if (ms->dojustonce && ms->d ) { //Even if in debug mode (ms->d = true), do this code block just once. If not in debug mode \
            don't do this for loop, ever.
            for (envVariableCounter = 0; envVariableCounter < ms->sizeofbuffer; envVariableCounter++ ) if (ms->d) cout << \
                envVariableCounter << ". " << std::hex << \
                *(JumpBufferForSIGINT + envVariableCounter) << std::dec << endl; // END of for loop
            ms->dojustonce=false; //Once is enough.
        }
        if (ms->d) cout << ms->whileLoopCounter << ". Reactivating ALARM t= " << time(NULL) - t << endl; // write start time
        alarm(ms->ALARMTIME);
        /* set a return mark   */
//       ms->rc = sigwait(ms->ptr_sigset, nullptr);
/* We know pause() works, but let's try sigprocmask-SIG_BLOCK/sussuspend/sigprocmask-SIG_UNBLOCK */
#ifdef PAUSE
    pause(); //Do this if the Xcode's compiler preprocessor directive, PAUSE, is defined
#else
/* Wait for a signal to arrive. */
        
    ms->rc = sigsuspend(&ms->oldmask);
        
#endif
    if (ms->d) cout << ms->whileLoopCounter <<". We got an interrupt from other than one of the ”3 Amigos“ " \
    " (SIGUSR1, SIGALRM, SIGINT). setJumpValue: " << ms->setJumpValue << ", the size of the circulary array " << sizeof(ms->A) << \
    " entries. Size of circular array, named A, is " << &ms->ptr_end - &ms->ptr_begin << " bytes. ms->ptr_theSignal = " << \
    ms->ptr_theSignal << ", the return code from " << *::whichwaitmethod << " is " << ms->rc << \
        " with a description of " << strerror(errno) << ". Note that ::someSignal looks like: " << ::someSignal << endl; //This \
        code gets executed if we get an interrupt other than the 3 Amigos

    } //End of while( (!operatorWantsToQuit) ) Loop
    ::msStart = ms->getQstart(); //Have msStart point to the START of this instance object.
#ifdef QUEUE
    ::msEnd =  &ms->qend; //Have msEnd point to the END of this instance object.
    ::qStart = &ms->qsstart;
    ::qEnd =   &ms->qend;
#endif

#ifdef DEBUG
        cout << "B. At the END of execution, class MySig starts at location " << &msStart << " and ends at location " << \
        &msEnd << " for a total length of " << &msEnd - &msStart << " bytes, while the queue, named q, " << \
        "within this class starts at location " << &::qStart << " and ends at location " << &::qEnd << " for a total length of " \
        << &::qEnd-&::qStart << " bytes. " << endl;
        cout << "ms->accumulateSIGALRMcounts: " << ms->accumulateSIGALRMcounts << "\nms->accumulateSIGALRMtimes: " << \
           ms->accumulateSIGALRMtimes << "\nAverage Execution Time of SIGALRM: " << \
           (ms->accumulateSIGALRMtimes)/(ms->accumulateSIGALRMcounts) << " NanoSeconds" << endl;
//        delete sigInfo;  //Remove the pointer to the instance of the the class named SI
#endif

    delete ms; //Remove the pointer to the instance of the the class named MySig. by deleting the instance of class, named MySig, \
    we will also delete the instance of the inherited class, named SI.
    return 0;
} //END OF MAIN    END OF MAIN    END OF MAIN    END OF MAIN    END OF MAIN    END OF MAIN    END OF MAIN    END OF MAIN
//=================================================================================================================================
//MySig::INThandler    MySig::INThandler    MySig::INThandler    MySig::INThandler    MySig::INThandler    MySig::INThandler
void  MySig::INThandler(int  sig) {
    
#ifdef PAUSE
    signal(sig, SIG_IGN);              /* ignore/deactivate Ctrl-C for now, thus preventing recursive stacking of SIGINTs    */
#else
    sigprocmask(SIG_BLOCK, &::mask, &::oldmask);
#endif
    ::interrupt_Handler_SIGINT_tp_start = std::chrono::high_resolution_clock::now();
    ::whichJump = SIGINTSETJUMP;
    longjmp(JumpBufferForSIGINT, SIGINTSETJUMP);       /* return to the setjmp mark*/
}
//MySig::sigalrm_handler    MySig::sigalrm_handler    MySig::sigalrm_handler    MySig::sigalrm_handler    MySig::sigalrm_handler
void MySig::sigalarm_handler(int sig) {
#ifdef PAUSE
    signal(sig, SIG_IGN);              /* ignore/deactivate Ctrl-C for now, thus preventing recursive stacking of SIGINTs    */
#else
    sigprocmask(SIG_BLOCK, &::mask, &::oldmask); /* deactivate SIGALRM + SIGUSR1 + SIGINT  while handling this interrupt */
#endif
    ::interrupt_Handler_SIGALRM_tp_start = std::chrono::high_resolution_clock::now();
    ::whichJump = SIGALRMSETJUMP;
    longjmp(JumpBufferForSIGINT, SIGALRMSETJUMP); /* return to the setjmp mark*/
}
//=================================================================================================================================
//MySig::sigusr1_handler    MySig::sigusr1_handler    MySig::sigusr1_handler    MySig::sigusr1_handler    MySig::sigusr1_handler
void MySig::sigusr1_handler(int sig) {
#ifdef PAUSE
    signal(sig, SIG_IGN);              /* ignore/deactivate Ctrl-C for now, thus preventing recursive stacking of SIGINTs    */
#else
    sigprocmask(SIG_BLOCK, &::mask, &::oldmask); //Deactiviate SIGINT + SIGALRM + SIGUSR1 while handling this interrupt
#endif
    ::interrupt_Handler_SIGUSR1_tp_start = std::chrono::high_resolution_clock::now();
    ::quitNOW = true;
    ::whichJump = SIGUSR1SETJUMP; //Set this GLOBAL MEMORY VARIABLE so the SIGUSR1 post-processing routine gets control

    longjmp(JumpBufferForSIGINT, SIGUSR1SETJUMP); /* return to the setjmp mark*/
}
//=================================================================================================================================
//MySig::sigUNKNOWN_handler    MySig::sigUNKNOWN_handler    MySig::sigUNKNOWN_handler    MySig::sigUNKNOWN_handler
void MySig::sigUNKNOWN_handler(int sig) {
#ifdef PAUSE
    signal(sig, SIG_IGN);              /* ignore/deactivate Ctrl-C for now, thus preventing recursive stacking of SIGINTs    */
#else
    sigprocmask(SIG_BLOCK, &::mask, &::oldmask); //Deactiviate SIGINT + SIGALRM + SIGUSR1 while handling this interrupt
#endif
    ::someSignal = sig;
    ::interrupt_Handler_SIGUNK_tp_start = std::chrono::high_resolution_clock::now();
    ::whichJump = SIGUNKNOWNJUMP; //Set this GLOBAL MEMORY VARIABLE so the SIGUSR1 post-processing routine gets control
    longjmp(JumpBufferForSIGINT, SIGUNKNOWNJUMP); /* return to the setjmp mark*/
}
//MySig Destructor    MySig Destructor    MySig Destructor    MySig Destructor    MySig Destructor    MySig Destructor
MySig::~MySig(void) {
#ifdef DEBUG
    if (d) cout << "Just entered MySig destructor with variable this set to location " << this << std::endl ;
#endif
}
//=================================================================================================================================
//MySig Constructor    MySig Constructor    MySig Constructor    MySig Constructor    MySig Constructor    MySig Constructor
MySig::MySig(void) noexcept(true) {
    /* Set up the mask of signals to temporarily block. */
#ifdef DEBUG //Query preprocessor to determine if we're compiling for debug mode.
    d = true;
#else
    d = false; 
#endif
    if(d) cout << "Just entered MySig constructor with variable this set to location " << this << std::endl ;
    sizeOfMySig = sizeof(MySig);
//Start Of Signal PreProcessing    Start Of Signal PreProcessing    Start Of Signal PreProcessing    Start Of Signal PreProcessing 
//Set-up the 2D array of pointer to function
    ptrToFunction[SIGUSR1SETJUMP][MEMBERFUNCTIONSTHATDOSIGHANDLERPOSTPROCESSING] = &doSIGUSR1;
    ptrToFunction[SIGALRMSETJUMP][MEMBERFUNCTIONSTHATDOSIGHANDLERPOSTPROCESSING] = &doSIGALRM;
    ptrToFunction[SIGINTSETJUMP][MEMBERFUNCTIONSTHATDOSIGHANDLERPOSTPROCESSING]  = &doSIGINT;
    ptrToFunction[SIGUNKNOWNJUMP][MEMBERFUNCTIONSTHATDOSIGHANDLERPOSTPROCESSING] = &doUNKNOWNSIGNAL;
    ptrToFunction[SIGUSR1SETJUMP][MEMBER_FUNCTIONS_THAT_WRANGLE_SIGHANDLER_EXECUTION_TIME] = &wrangleSIGUSR1startTime;
    ptrToFunction[SIGALRMSETJUMP][MEMBER_FUNCTIONS_THAT_WRANGLE_SIGHANDLER_EXECUTION_TIME] = &wrangleSIGALRMstartTime;
    ptrToFunction[SIGINTSETJUMP][MEMBER_FUNCTIONS_THAT_WRANGLE_SIGHANDLER_EXECUTION_TIME]  = &wrangleSIGALRMstartTime;
    ptrToFunction[SIGUNKNOWNJUMP][MEMBER_FUNCTIONS_THAT_WRANGLE_SIGHANDLER_EXECUTION_TIME] = &wrangleSIGUNKNOWNstartTime;
    sigemptyset (&mask); //Make sure this is clear.
   
    
//    sigusr1 = sigmask(SIGUSR1);
 //   sigalrm = sigmask(SIGALRM);
//    WAITMASK = (sig_t)(SIGALRM) | (sig_t)sigmask(SIGUSR1) | (sig_t)sigmask(SIGINT);  //Wait for these signals

    sigaddset (&mask, SIGALRM);
    sigaddset (&mask, SIGUSR1);
    sigaddset (&mask, SIGINT);
        sigaddset (&mask, SIGUSR2); //We'll assign sigusr2 to the UNKNOWN group of signals
        sigaddset (&mask, SIGTRAP); //We'll assign sigtrap to the UNKNOWN group of signals
        sigaddset (&mask, SIGEMT);  //We'll assign sigemt to the UNKNOWN group of signals
    //
//     SIGINT (bit 02)  ---------------------------------+
//     SIGALRM (bit 14) ---------------------+           |
//     SIGUSR1 (bit 30) -----+               |           |
//                           |               |           |
//                           V               V           V
//When done mask looks: "0b00100000000000000010000000000010" (33-bits) or as 0x20002002, or as 0o04000020002
//    sigset = sigusr1 + sigalrm;
    ::mask = this->mask; //Set the global mask so the signal handlers can use it, too.
        signal(SIGINT, MySig::INThandler);        /* initial install Ctrl-C (SIGINT) signal  handler   */
        signal(SIGALRM, MySig::sigalarm_handler); // initial install SIGALRM signal handler
        signal(SIGUSR1, MySig::sigusr1_handler); // initial install SIGUSR1 signal handler
        signal(SIGUSR2, MySig::sigUNKNOWN_handler); //sigUNKNOWN_handler processes all signals other than SIGINT, SIGALRM, and SIGUSR1
        signal(SIGHUP, MySig::sigUNKNOWN_handler);
        signal(SIGQUIT, MySig::sigUNKNOWN_handler);
        signal(SIGILL, MySig::sigUNKNOWN_handler);
        signal(SIGHUP, MySig::sigUNKNOWN_handler);
        signal(SIGTRAP, MySig::sigUNKNOWN_handler);
        signal(SIGHUP, MySig::sigUNKNOWN_handler);
        signal(SIGABRT, MySig::sigUNKNOWN_handler);
        signal(SIGHUP, MySig::sigUNKNOWN_handler);
        signal(SIGEMT, MySig::sigUNKNOWN_handler);
        signal(SIGFPE, MySig::sigUNKNOWN_handler);
        signal(SIGKILL, MySig::sigUNKNOWN_handler);
        signal(SIGHUP, MySig::sigUNKNOWN_handler);
        signal(SIGBUS, MySig::sigUNKNOWN_handler);
        signal(SIGSEGV, MySig::sigUNKNOWN_handler);
        signal(SIGHUP, MySig::sigUNKNOWN_handler);
        signal(SIGSYS, MySig::sigUNKNOWN_handler);
        signal(SIGPIPE, MySig::sigUNKNOWN_handler);
        signal(SIGTERM, MySig::sigUNKNOWN_handler);
        signal(SIGURG, MySig::sigUNKNOWN_handler);
        signal(SIGSTOP, MySig::sigUNKNOWN_handler);
        signal(SIGTSTP, MySig::sigUNKNOWN_handler);
        signal(SIGCONT, MySig::sigUNKNOWN_handler);
        signal(SIGCHLD, MySig::sigUNKNOWN_handler);
        signal(SIGTTIN, MySig::sigUNKNOWN_handler);
        signal(SIGTTOU, MySig::sigUNKNOWN_handler);
        signal(SIGIO, MySig::sigUNKNOWN_handler);
        signal(SIGXCPU, MySig::sigUNKNOWN_handler);
        signal(SIGHUP, MySig::sigUNKNOWN_handler);
        signal(SIGXFSZ, MySig::sigUNKNOWN_handler);
        signal(SIGPROF, MySig::sigUNKNOWN_handler);
        signal(SIGHUP, MySig::sigUNKNOWN_handler);
        signal(SIGWINCH, MySig::sigUNKNOWN_handler);
        signal(SIGINFO, MySig::sigUNKNOWN_handler);
//End Of Signal PreProcessing    End Of Signal PreProcessing    End Of Signal PreProcessing    End Of Signal PreProcessing
    sizeofbuffer = sizeof(JumpBufferForSIGINT);
    operatorWantsToQuit=false;
    dojustonce = true;
//    ::ptrQ = &this->q; //Establish addressability to multi-element queue
//    ::ptrOneQElement = &oneQueueElement;
/* The next two were removed from class named MySig
    ptrqstart = &MySigstart;

    ::msStart = &this->MySigstart; //Have msStart point to the START of this instance the object named MySIG.
*/
#ifdef QUEUE
    ::msEnd   = &this->qend; //Have msEnd point to the END of this instance object.
    ::qStart  = &this->qsstart;
    ::qEnd    = &this->qend;
#endif
    ::whichJump = ZERO; //Make sure this is clean
//Build a circular array.
    for (rc = 0; rc < NUMBER_OF_ELEMENTS_IN_CIRCULAR_ARRAY; rc++) {
        A[rc].u2x8.flag = 0; //Make sure the two 8-bit components (represented by union as a single 16-bit component) is CLEAN
        A[rc].c1 = (int8_t)(0b00110000 + rc/10); //Do this so this array's Element Number shows up in a memory dump as the TENS digit
        A[rc].c2 = (int8_t)(0b00110000 + rc%10); //Do this so this array's Element Number shows up in a memory dump as the ONES digit
//        A[rc].entryNumber = rc;
        A[rc].myprocessid = getpid();
//Build the forward-pointers
        if(rc <  NUMBER_OF_ELEMENTS_IN_CIRCULAR_ARRAY - ONE ) A[rc].ptrToNext = &A[ONE + rc];
        else {
            A[rc].ptrToNext = &A[0]; //Come here if rc is the array index number of the last array entry so the last array entry \
will forward-point to the first entry of the array
//Build the back-pointers
            A[rc].u2x8.int8s.atomic_flag=FLAGS_NOTINUSE;  // = 0b00000000
        }
        if (rc != ZERO ) A[rc].ptrToPrevious = &A[rc - ONE]; //Do This if not first array entry so its back pointer points to \
previous array entry.
        else   {
            A[rc].ptrToPrevious = &A[ NUMBER_OF_ELEMENTS_IN_CIRCULAR_ARRAY - ONE ]; //Do This if at first array entry so its \
            back pointer points to last array entry.
            A[rc].u2x8.int8s.atomic_flag=FLAGS_INUSE;     // = 0b00000001;
        }
 //       pae.store(&A[0]); //Make Element 0 the active element;
    //ptr_Active_Element.store(&A[0]);
    } //End of for statement
    myProcessID = getpid();
//    ptr_sigset = &sigset;
    ptr_theSignal = &theSignal;
//    ::d = this->d;
    t = time(nullptr); //At the instant of this instruction's execution, set t to the current time of day.
} //END OF MySig Constructor    END OF MySig Constructor    END OF MySig Constructor    END OF MySig Constructor
//=================================================================================================================================
unsigned char *MySig::getQstart(void) {
    return ptrqstart;
}
//=================================================================================================================================
std::string MySig::timePointAsString (const std::chrono::system_clock::time_point& tpIn) {
//    string timeOut;
//   auto now = chrono::system_clock::now();
    t_t = chrono::system_clock::to_time_t(tpIn);
    nowTM = std::localtime(&t_t);
    rcsz = strftime(mytime, TIMESIZE, "%Y-%m-%dT%H:%M:%S %Z", nowTM );
    formattedTime = string(mytime);

//    std::string ts__ = ctime(&t_t);
/*
        const time_put<char>& tpC = use_facet<time_put<char>>(locC);
        oss.clear();
        tpC.put (oss, oss, ' ', nowTM, format.c_str(), format.c_str()+format.size() ); //BEWARE that this might concatenate the \
        current value with the previous value(s). We might have to reset oss before prior to the oss command (eg. oss.clear())
*/
    return formattedTime;
}
//=================================================================================================================================
void MySig::doSIGINT(MySig *ms,unsigned int i) {
#ifdef DEBUG
    cout << "doSIGINT processing routine here because of jump index " << i << "\nSIGINT: Detected Ctrl-C!" <<
        "\nDo you really want to quit? [y/n] " << endl;
#endif
    manageCircularArray(ms, SIGINT);

    cin >> ::in_character;
    if ( ::in_character == 'y' || ::in_character == 'Y' ) {
        operatorWantsToQuit = true;
    } else {
#ifdef PAUSE
        signal(SIGINT, MySig::INThandler);    /* reactivate SIGINT before return  */
#else
        sigprocmask(SIG_UNBLOCK, &::mask, NULL); //Reactiviate SIGINT + SIGALRM + SIGUSR1
#endif
        }
}
//=================================================================================================================================
void MySig::doSIGUSR1(MySig *ms, unsigned int i) {
#ifdef DEBUG
    cout << "doSIGUSR1 processing routine here because of jump index " << i << endl;
#endif
    manageCircularArray(ms, SIGUSR1);

#ifdef PAUSE
    signal(SIGUSR1, MySig::sigusr1_handler(SIGUSR1));    /* reactivate SIGUSR1 before return  */
#else
    sigprocmask(SIG_UNBLOCK, &::mask, NULL); //Reactiviate SIGINT + SIGALRM + SIGUSR1 before returning to user program at the \
instruction following the setjmp instruction
#endif
}
//=================================================================================================================================
void MySig::doSIGALRM(MySig *ms, unsigned int i) { 
#ifdef DEBUG
    cout << "SIGALRM, having signal id of " << SIGALRM << ", fired at t=" <<  time(NULL) - t << \
    "\ndoALRM processing routine here because of jump index " << i << endl; // write a message when called
#endif
    
    manageCircularArray(ms, SIGALRM);

#ifdef PAUSE
    signal(SIGALRM, MySig::sigusr1_handler);    /* reactivate SIGALRM before return  */
#else
    sigprocmask(SIG_UNBLOCK, &::mask, NULL); /* reactivate SIGALRM + SIGUSR1 + SIGINT before return  */
#endif
} //End of MySig::doSIGALRM
//=================================================================================================================================
void MySig::doUNKNOWNSIGNAL(MySig *ms, unsigned int i) {
    manageCircularArray(ms, ::someSignal );
#ifdef DEBUG
    cout << "doUNKNOWNSIGNAL processing routine here because of jump index " << i << "; We got a signal: " << ::someSignal << \
    ", which fired at t=" <<  time(NULL) - t << endl;
#endif
#ifdef PAUSE
    signal(SIGALRM, MySig::sigusr1_handler);    /* reactivate SIGALRM before return  */
#else
    sigprocmask(SIG_UNBLOCK, &::mask, NULL); /* reactivate the signal that is other than SIGUSR1, SIGALRM, or SIGINT before return */
#endif
}
//=================================================================================================================================
//CIRCULAR ARRAY MANAGER    CIRCULAR ARRAY MANAGER    CIRCULAR ARRAY MANAGER    CIRCULAR ARRAY MANAGER    CIRCULAR ARRAY MANAGER
void MySig::manageCircularArray(MySig *ms, unsigned int some_signal) {
//=================================================================================================================================
// ATOMIC OPERATION FOLLOWS      ATOMIC OPERATION FOLLOWS      ATOMIC OPERATION FOLLOWS      ATOMIC OPERATION FOLLOWS
// Currently we expect the flag field of the current Active Array Element to look like this, --------------------+
// (i.e., 0b10000000) and the current value of the NEXT Active Array Element to look like 0b00000000.            |
// What we expect to happen upon ATOMIC operation is that 0b00000000 of the NEXT Active Array Element            |
// will be exchanged with the 0b10000000 vale of the current Active Array Element, resulting in                  |
// the Currect Active Arra Elemnet becomming DEACTIVATED and the NEXT Active Array Element becomming             |
//ACTIVATED.                                                                                                     |
// Next array element that we want to make as the active array element + ----------+                             |
// currently active element --+ that we will deactivate by activating the next el  |                             |
//                            |                                                    |                             |
//                            |                                                    |                             |
//                            V                                                    V                             V
    ms->ptr_Active_Element->u2x8.int8s.atomic_flag = ms->ptr_Active_Element->ptrToNext->u2x8.int8s.atomic_flag.exchange(FLAGS_INUSE); // \
make the next array element, following the current active array element, as the active array element  while ***simultaneously*** \
deactivating the currently active element.
// ATOMIC OPERATION COMPLETE      ATOMIC OPERATION COMPLETE      ATOMIC OPERATION COMPLETE      ATOMIC OPERATION COMPLETE
//     ms->ptr_Active_Element->u2x8.flag = ms->ptr_Active_Element->ptrToNext->u2x8.atomic_flag.exchange(0b10000000); //Alternative?
    ms->ptr_Active_Element = ms->ptr_Active_Element->ptrToNext; // Point to the new active element (Can this instruction be made \
    into an atomic operation, too? Too bad there aren't any ATOMIC functions)
    ms->indexOfActiveElement++; //Can this instruction be included in the much-desired ATOMIC function?
    ms->ptr_Available_Element = ms->ptr_Active_Element->ptrToNext;
    ms->ptr_Active_Element->stop_tp = std::chrono::high_resolution_clock::now(); //Time stamp this entry. Don't include in this \
    time-stamp the exection time of the next `cout` instruction
//=================================================================================================================================

    bitset<8> the_atomic_flag(ms->ptr_Active_Element->u2x8.int8s.atomic_flag);  //All this just to format data in binary.
    cout << ms->indexOfActiveElement << ". Active Element ID is: " << ms->ptr_Active_Element->c1 << ms->ptr_Active_Element->c2 << \
    " Active Element’s Address: " <<  ms->ptr_Active_Element   << ", The atomic_flag field looks like: " << the_atomic_flag << \
    ms->ptr_Active_Element->u2x8.int8s.atomic_flag << endl;

    ms->ptrToFunction[::whichJump][MEMBER_FUNCTIONS_THAT_WRANGLE_SIGHANDLER_EXECUTION_TIME](ms, ::whichJump);
} // END CIRCULAR ARRAY MANAGER     END CIRCULAR ARRAY MANAGER     END CIRCULAR ARRAY MANAGER     END CIRCULAR ARRAY MANAGER
//=================================================================================================================================
void MySig::wrangleSIGALRMstartTime(MySig *ms, unsigned int s) {
    ms->ptr_Active_Element->start_tp = interrupt_Handler_SIGALRM_tp_start;
#ifdef DEBUG
    cout << s << ". Time to execute SIGALRM interrupt " << \
    ms->ptr_Active_Element->stop_tp.time_since_epoch().count() -  ms->ptr_Active_Element->start_tp.time_since_epoch().count() << \
    " Nanoseconds " << endl;
    if (!::quitNOW) {
        ms->accumulateSIGALRMtimes += \
        ms->ptr_Active_Element->stop_tp.time_since_epoch().count() -  ms->ptr_Active_Element->start_tp.time_since_epoch().count();
        ms->accumulateSIGALRMcounts++;
    }
#endif
} //END OF WRANGLE SIGALRM START TIME    END OF WRANGLE SIGALRM START TIME    END OF WRANGLE SIGALRM START TIME
//=================================================================================================================================
void MySig::wrangleSIGUSR1startTime(MySig *ms, unsigned int s) {
    ms->ptr_Active_Element->start_tp = interrupt_Handler_SIGUSR1_tp_start;
#ifdef DEBUG
    cout << "s. Time to execute SIGUSR1 interrupt " << \
    ms->ptr_Active_Element->stop_tp.time_since_epoch().count() -  ms->ptr_Active_Element->start_tp.time_since_epoch().count() << \
    " Nanoseconds " << endl;
#endif
} //END OF WRANGLE SIGUSER1 START TIME    END OF WRANGLE SIGUSER1 START TIME    END OF WRANGLE SIGUSER1 START TIME
//=================================================================================================================================
void MySig::wrangleSIGINTstartTime(MySig *ms, unsigned int s) {
    ms->ptr_Active_Element->start_tp = interrupt_Handler_SIGINT_tp_start;
#ifdef DEBUG
    cout << s << ". Time to execute SIGINT interrupt " << \
    ms->ptr_Active_Element->stop_tp.time_since_epoch().count() -  ms->ptr_Active_Element->start_tp.time_since_epoch().count() << \
    " Nanoseconds " << endl;
#endif
} //END OF WRANGLE SIGINT START TIME    END OF WRANGLE SIGINT START TIME    END OF WRANGLE SIGINT START TIME
//=================================================================================================================================
void MySig::wrangleSIGUNKNOWNstartTime(MySig *ms, unsigned int theSignal) {
#ifdef DEBUG
    ms->ptr_Active_Element->start_tp = interrupt_Handler_SIGUNK_tp_start;
    cout << "Unknown Signal. Have no signal handler to provide the instant the unknown signal occurred. " << theSignal << \
    ". Time to execute signal from SIGNAL SOURCE  " << ::someSignal << \
    ms->ptr_Active_Element->stop_tp.time_since_epoch().count() -  ms->ptr_Active_Element->start_tp.time_since_epoch().count() << \
    " Nanoseconds " << endl;
    
#endif
} //END OF WRANGLE SIGUNKNOWN START TIME    END OF WRANGLE SIGUNKNOWN START TIME    END OF WRANGLE SIGUNKNOWN START TIME
//=================================================================================================================================
