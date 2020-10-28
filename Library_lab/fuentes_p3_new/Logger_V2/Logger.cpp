//*****************************************************************
// File:   Logger.cpp
// Author: J. Ezpeleta-Unizar
// Date:   march 2017
// Coms:   A (monitor) class for the concurrent access to log files
//         The interesting thing is the way a FIFO access is granted to 
//         the monitor
//*****************************************************************

#include <Logger.hpp>
#include <errno.h>
#include <stdio.h>

//----------------------------------------------------------
Logger::Logger(string logName):
    logName(logName) {

    
    getMainID();
    messages = new string[MAX_MESS];
    echoed = false;
    ticketCounter = 1;
    next = 1;
    nMess = 0;
    createLogFile();
}

Logger::Logger(string logName, ostream& echo):
    logName(logName), echo(&echo) {

    getMainID();
    messages = new string[MAX_MESS];
    echoed = true;
    ticketCounter = 1;
    next = 1;
    nMess = 0;
    createLogFile();
}

//----------------------------------------------------------
Logger::~Logger() {
    save(); //write pending messages
    delete [] messages; //free buffer memory
}
//----------------------------------------------------------
void Logger::getMainID() {
    stringstream ss;
    ss << this_thread::get_id();
    mainID = ss.str();
}
//----------------------------------------------------------
void Logger::save() {
    
    logFile.open(logName, std::ios::out | std::ios::app);
    if (logFile.fail()) {
        std::runtime_error("Problems with file "+logName+"\n");
    }
    logFile.exceptions(logFile.exceptions() | std::ios::failbit | std::ifstream::badbit);

    for (size_t i=0; i<nMess; i++) {
        logFile << messages[i] << endl;
    }
    nMess = 0;
    logFile.close();
}
//----------------------------------------------------------
void Logger::createLogFile() {

    logFile.open(logName);
    if (logFile.fail()) {
        std::runtime_error("Problems with file "+logName+"\n");
    }
    logFile.exceptions(logFile.exceptions() | std::ios::failbit | std::ifstream::badbit);

    logFile << HEADER << endl;
    logFile.close();
}
//----------------------------------------------------------
void Logger::addMessage(string mess) {

    uint64_t myTicket;
    const uint64_t uno(1);
    chrono::nanoseconds ns;
    condition_variable* theCond;
    map<uint64_t,condition_variable*>::iterator it;
    //----------------------------------------------------------
    //Next two sentences are "thread safe"
    //Next sentence provides a unique ticket for the thread
    myTicket = atomic_fetch_add(&ticketCounter, uno);
    ns = chrono::duration_cast<chrono::nanoseconds>(
        chrono::system_clock::now().time_since_epoch()
    );

    //----------------------------------------------------------
    //Ensure mutex access to data
    unique_lock<mutex> lck(mtx);

    //Is it my turn?
    if (myTicket != next) { //No: I have to lock and wait
        theCond = new condition_variable();

        pair<uint64_t,condition_variable*> thePair(myTicket,theCond);
        waitingMap.insert(thePair);
        //locking me in my own var condition
        theCond->wait(lck);
        
        //find me, and remove me from the queue
        it = waitingMap.find(myTicket);
        waitingMap.erase(it);
        //My condition var is not needed any more
        delete theCond;
    }
    //It's my turn: myTicket=next
    //If buffer is full, save it
    if (nMess == MAX_MESS) {
        save();
    }

    const char SEP = ',';
    // const string ID("idUnico");
    const string ID = "id_" + mainID;

    vector<string> event_list = split(mess, ';');
    for (size_t i=0; i<event_list.size(); i++) {
        stringstream ss;
        ss << ID << SEP << event_list[i] << SEP << ns.count() << SEP << myTicket;
        messages[nMess] = ss.str();
        nMess++;
        if (echoed) {
            *echo << ss.str() << endl;
        }
    }
    
    next++;
    //Look for the next turn process
    it = waitingMap.find(next);
    if (it != waitingMap.end()) { //Is the next waiting?
        it->second->notify_one();
    }
}
//----------------------------------------------------------
vector<std::string> Logger::split(const std::string& s, char sep) {
   std::vector<std::string> elems;
   std::string el;
   std::istringstream seqEls(s);
   while (std::getline(seqEls, el, sep)) {
      elems.push_back(el);
   }
   return elems;
}