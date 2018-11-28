/*
 * File: VectorPatientQueue.cpp
 * --------------------------
 * Name: Xiaoxi Zeng
 * Section leader: Semir Shafi
 * This prgram implements the priority queque using the
 * unsorted vector in the Stanford library.
 */

#include "VectorPatientQueue.h"
#include "vector.h"
#include "strlib.h"
#include "string.h"

const string SEPERATE = ", ";

VectorPatientQueue::VectorPatientQueue() {
    vecPatientQueue = {};
}

VectorPatientQueue::~VectorPatientQueue() {

}

void VectorPatientQueue::clear() {
    vecPatientQueue.clear();
}

string VectorPatientQueue::frontName() {
    string toReturn;
    if(vecPatientQueue.isEmpty()){
        throw("There is no top. I must abort. I never learned how to love...");
    }else{
        Patient highPriorityPatient = vecPatientQueue[0];
        toReturn = highPriorityPatient.patientName;
        for(int i = 1; i < vecPatientQueue.size(); i++){
            Patient currPatient = vecPatientQueue[i];
            int temp = currPatient.priority;
            if( highPriorityPatient.priority > temp){
                toReturn = currPatient.patientName;
                highPriorityPatient = currPatient;
            }else if(highPriorityPatient.priority == temp){
                if((vecPatientQueue.size() - highPriorityPatient.timestamp) > (vecPatientQueue.size() - currPatient.timestamp)){
                    toReturn = highPriorityPatient.patientName;
                }else{
                    toReturn = currPatient.patientName;
                }
            }
        }
    }
    return toReturn;
}

int VectorPatientQueue::frontPriority() {
    Patient highPriorityPatient = vecPatientQueue[0];
    int highPriority = highPriorityPatient.priority;
    if(vecPatientQueue.isEmpty()){
        throw("There is no top. I must abort. I never learned how to love...");
    }else{
        for(int i = 1; i < vecPatientQueue.size(); i++){
            Patient currPatient = vecPatientQueue[i];
            int temp = currPatient.priority;
            if( highPriorityPatient.priority > temp){
                highPriority = temp;
                highPriorityPatient = currPatient;
            }
        }
    }
    return highPriority;
}

bool VectorPatientQueue::isEmpty() {
    if(vecPatientQueue.isEmpty()){
        return true;
    }
    return false;
}

void VectorPatientQueue::newPatient(string name, int priority) {
    Patient newPatient;
    newPatient.patientName = name;
    newPatient.priority = priority;
    newPatient.timestamp = vecPatientQueue.size(); //Use timestamp to record how long a patient has been waiting in the queue
    vecPatientQueue.add(newPatient);
}

string VectorPatientQueue::processPatient() {
    string toReturn;
    if(vecPatientQueue.isEmpty()){
        throw("There is no top. I must abort. I never learned how to love...");
    }else{
        Patient highPriorityPatient = vecPatientQueue[0];
        int index = 0;
        toReturn = highPriorityPatient.patientName;
        for(int i = 1; i < vecPatientQueue.size(); i++){
            Patient currPatient = vecPatientQueue[i];
            int temp = currPatient.priority;
            if( highPriorityPatient.priority > temp){
                index = i;
                toReturn = currPatient.patientName;
                highPriorityPatient = currPatient;
            }else if(highPriorityPatient.priority == temp){
                if((vecPatientQueue.size() - highPriorityPatient.timestamp) > (vecPatientQueue.size() - currPatient.timestamp)){
                    toReturn = highPriorityPatient.patientName;
                }else{
                    index = i;
                    toReturn = currPatient.patientName;
                }
            }
        }
        vecPatientQueue.remove(index);
    }
    return toReturn;
}

void VectorPatientQueue::upgradePatient(string name, int newPriority) {
    int counter = 0;
    int patientIndex;
    for(int i = 0; i < vecPatientQueue.size(); i++){
        Patient currPatient = vecPatientQueue[i];
        if(currPatient.patientName == name){
            counter ++;
            if(counter == 1){
                patientIndex = i;
            }else if(counter > 1){ //if a patient name occurs multiple times, change the one with the highest priority
                if(vecPatientQueue[patientIndex].priority > currPatient.priority){
                    patientIndex = i;
                }
            }
        }
    }
    if(counter == 0){
        throw("That patient is not in the queue.");
    }
    if(vecPatientQueue[patientIndex].priority <= newPriority){
        throw("You are decreasing the priority.");
    }else{
        vecPatientQueue[patientIndex].priority = newPriority;
    }
}

string VectorPatientQueue::toString() {
    if(vecPatientQueue.isEmpty()){
        return "{}";
    }
    string toReturn = "{";
    for(int i = 0; i < vecPatientQueue.size() - 1; i ++){
        toReturn += integerToString(vecPatientQueue[i].priority) + ":" + vecPatientQueue[i].patientName + SEPERATE;
    }
    toReturn += integerToString(vecPatientQueue[(vecPatientQueue.size() - 1)].priority) + ":" + vecPatientQueue[(vecPatientQueue.size() - 1)].patientName + "}";
    return toReturn;
}
