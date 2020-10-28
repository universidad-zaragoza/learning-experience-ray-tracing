//*****************************************************************
// File:   Logger.hpp
// Author: J. Ezpeleta-Unizar
// Date:   March 2017
// Coms:   A (monitor) class for the concurrent access to log files
//         The interesting thing is the way a FIFO access is granted to 
//         the monitor. See class code
//*****************************************************************

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <sstream>
#include <atomic>
#include <chrono>
#include <map>
#include <vector>

using namespace std;

class Logger {
private:

    string mainID;
    //----- for mutex access to monitor functions
    mutex mtx;
    //----- message buffer
    const int MAX_MESS = 4096; //buffer bound
    int nMess; //number of messages in the buffer
    string *messages; //message buffer
    //----- to output messages
    string logName; //the name of the log file
    ofstream logFile; //stream to the log file>
    bool echoed; //Should the message be echoed?
    ostream* echo; //stream for echoed messages
    //----- for handling the order of arrival
    atomic<uint64_t> ticketCounter; //increments one per process and access
    uint64_t next; //next ticket value having acces right
    //each waiting process will add the pair <numTicket,condVar> and then lock on the second component
    map<uint64_t,condition_variable*> waitingMap;
    //-----------------------------------------------------
    //Pre: 
    //Post: Creates the log file. If it already exists, its contents are emptied.
    void createLogFile();
    //-----------------------------------------------------
    //Pre:
    //Post: Saves the contents of "messages" into the log file and clears it
    void save();
    //-----------------------------------------------------
    //Pre:
    //Post: Splits "s" using "sep" as the separator char, and returns
    //      the vector of the components
    vector<std::string> split(const std::string& s, char sep);
    //-----------------------------------------------------
    //log file header, describing event structure
    //according to the requirements of the DLTL model checker
    const string HEADER = "threadID,sectionID,event,val,ts,ticket";
    //-----------------------------------------------------
    //Fulfills "mainID" with the thread ID of the main function
    void getMainID();

    
public:
	//------------------------- constructor
    //Pre:
    //Post: Creates the logger and associates the message ouput to the file "logName"
    //for non-echoed messages
    Logger(string logName);
    //Pre:
    //Post: creates the logger and associates the message ouput to the file "logName".
    //      Messages, besides stored in the f, will be echoed.
    //      Usually, "echo" will be either "cout" or "cerr", but it is not mandatory
    Logger(string logName, ostream& echo);

    //------------------------- destructor
    //Pre:
    //Post: free everything
    ~Logger();

    //-------------------------
    //Pre: 
    //Post: Adds a new message to the log (in a buffered way)
    //      ";" and "\n" chars are separators, indicating different events in the string
    void addMessage(string mess);
};

#endif 
