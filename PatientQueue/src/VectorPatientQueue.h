/*
 * File: VectorPatientQueue.h
 * --------------------------
 * Name: Xiaoxi Zeng
 * Section leader: Semir Shafi
 */

#pragma once

#include <iostream>
#include <string>
#include "patientqueue.h"
#include "vector.h"
using namespace std;

class VectorPatientQueue : public PatientQueue {
public:
    VectorPatientQueue();
    ~VectorPatientQueue();
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
        string patientName;
        int priority;
        int timestamp;
    };
    Vector<Patient> vecPatientQueue;
};
