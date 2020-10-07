//
//  signalDescription.cpp
//  doingSIGINTandSIGALRMandSIGUSR1
//
//  Created by Clifford Campo on 10/6/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//

#include "doingSigIntANDSigAlrm.hpp"
#include "signalDescription.hpp"
/*
ID  NAME            Default Action          Description of Cause
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
void SI::signalDescription (int sigNumber) {

    if (sigNumber < 1 || sigNumber > 31) {
        map<int, sigInfo>::iterator pos;
        for (pos = allTheSigs.begin(); pos == allTheSigs.end(); ++pos) {
            cout << (allTheSigs.find(sigNumber))->second.signalNumber << comma;
            cout << pos->first << "………"  << pos->second.theSignal << comma << \
            pos->second.whatHappens << comma << pos->second.signalCause <<  endl;
        }
    } else {
/* Typical output looks like:
     2, 2, SIGINT, interrupt program
     14, 14, SIGALRM, real-time timer expired
     30, 30, SIGUSR1, User defined signal 1
*/
        cout << (allTheSigs.find(sigNumber))->second.signalNumber << comma;
        cout << (allTheSigs.find(sigNumber))->first << comma  << \
        (allTheSigs.find(sigNumber))->second.theSignal << comma << \
        (allTheSigs.find(sigNumber))->second.whatHappens << comma <<  \
        (allTheSigs.find(sigNumber))->second.signalCause << endl;
    }
    
}
SI::SI(void) { //Note: as soon as we enter the derived class's constructor, named MySig::MySig(void), than control is passed \
    to this base class's constructor, named SI::SI(void) so the base class can be constructed prior to the derived class \
    MySig::MySig(void) { … } being constructed
    cout << "Entered the constructor (SI::SI(void)) for this instance of the class named SI. Let's now initialize the \
    map container, named allTheSigs, using data from the array of typedefs, named si. The instance of the SI base class is " << \
    "at location " << this << endl;
    for (i=0; i < NumberOfSignals; i++) { 
        allTheSigs.insert({i, si[i]});
    }
}
SI::~SI(void) {
    cout << "Entered the Destructor (SI::~SI) for this instance of the base class, named SI, " << \
    "which is at location " << this << endl;
}
