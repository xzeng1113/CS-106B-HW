/*
 * File: HeapPatientQueue.h
 * --------------------------
 * Name: Xiaoxi Zeng
 * Section leader: Semir Shafi
 */
#pragma once

#include <iostream>
#include <string>
#include "patientnode.h"
#include "patientqueue.h"
using namespace std;

class HeapPatientQueue : public PatientQueue  {
public:
    HeapPatientQueue();
    ~HeapPatientQueue();
    string frontName();
    void clear();
    int frontPriority();
    bool isEmpty();
    void newPatient(string name, int priority);
    string processPatient();
    void upgradePatient(string name, int newPriority);
    string toString();

private:
    struct Patient{
        string name;
        int priority;
    };
    Patient *elements;
    int capacity;
    int count;
    void expand();
};
