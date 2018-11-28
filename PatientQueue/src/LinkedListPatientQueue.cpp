/*
 * File: LinkedListPaientQueue.cpp
 * --------------------------
 * Name: Xiaoxi Zeng
 * Section leader: Semir Shafi
 * This prgram implements a priority queue using a sorted linked list.
 */

#include "LinkedListPatientQueue.h"
#include "string.h"
#include "strlib.h"

const string SEPERATE = ", ";
LinkedListPatientQueue::LinkedListPatientQueue() {
    front = nullptr;
}

LinkedListPatientQueue::~LinkedListPatientQueue() {
    //Walk through queue and delete all elements
    while(front != nullptr){
        PatientNode *temp = front->next;
        delete front;
        front = temp;
    }
}

void LinkedListPatientQueue::clear() {
    while(front != nullptr){
        PatientNode *temp = front->next;
        delete front;
        front = temp;
    }
}

string LinkedListPatientQueue::frontName() {
    string toReturn;
    if(front == nullptr){
        throw("There is no top. I must abort. I never learned how to love...");
    }else{
        toReturn = front->name;
    }
    return toReturn;
}

int LinkedListPatientQueue::frontPriority() {
    int priority;
    if(front == nullptr){
        throw("There is no top. I must abort. I never learned how to love...");
    }else{
        priority = front->priority;
    }
    return priority;
}

bool LinkedListPatientQueue::isEmpty() {
    if(front == nullptr){
        return true;
    }
    return false;
}

void LinkedListPatientQueue::newPatient(string name, int priority) {
    if(front == nullptr){ //Zero node scenario
        front = new PatientNode;
        front->name = name;
        front->priority = priority;
        front->next = nullptr;
    }else if(front->next == nullptr){ //One node scenario
        //Create a new node
        PatientNode *newlyAdded = new PatientNode;
        newlyAdded->name = name;
        newlyAdded->priority = priority;
        newlyAdded->next = nullptr;
        if(front->priority <= newlyAdded->priority){
            front->next = newlyAdded;
        }else{
            PatientNode *temp = front;
            front = newlyAdded;
            newlyAdded->next = temp;
        }
    }else{ //N-nodes scenario
        PatientNode *newlyAdded = new PatientNode;
        newlyAdded->name = name;
        newlyAdded->priority = priority;
        newlyAdded->next = nullptr;
        //Add the new patient node in the front
        if((front->priority) > (newlyAdded->priority)){
            PatientNode *temp0 = front;
            front = newlyAdded;
            newlyAdded->next = temp0;
        }else{
            //Add the new node in the middle
            PatientNode *temp = front;
            while(temp->next != nullptr){
                if((temp->priority) <= (newlyAdded->priority) && (temp->next->priority) > (newlyAdded->priority)){
                    newlyAdded->next = temp->next;
                    temp->next = newlyAdded;
                    break;
                }else{
                    temp = temp->next;
                }
            }
            //Add the new node in the back
            if(temp->next == nullptr){
                temp->next = newlyAdded;
            }
        }

    }
}

string LinkedListPatientQueue::processPatient() {
    // TODO: write this function
    string toReturn;
    if(front == nullptr){
        throw("There is no top. I must abort. I never learned how to love...");
    }else{
        toReturn = front->name;
        PatientNode *temp = front->next;
        delete front;
        front = temp;
    }
    return toReturn;
}

void LinkedListPatientQueue::upgradePatient(string name, int newPriority) {
    // TODO: write this function
    if(front->name == name){
        if(front->priority <= newPriority){
            throw("You are decreasing the priority.");
        }else{
            front->priority = newPriority;
        }
    }else{
        PatientNode *curr1 = front;
        bool nameExist = false;
        while(curr1->next != nullptr){
            if(curr1->next->name == name){
                nameExist = true;
                if(curr1->next->priority <= newPriority){
                    throw("You are decreasing the priority.");
                }else{
                    curr1->next->priority = newPriority;
                    break;
                }
            }else{
                curr1 = curr1->next;
            }
        }

        if(!nameExist){
            throw("That patient is not in the queue.");
        }else{
            PatientNode *target = curr1->next;
            if(curr1->next->next != nullptr){
                curr1->next = curr1->next->next;
            }else{
                curr1->next = nullptr;
            }

            //If the newPriority is more urgent than the front one
            if((front->priority) > (target->priority)){
                PatientNode *temp1 = front;
                front = target;
                target->next = temp1;
            }else{
                //Modify the node pointer in the middle
                PatientNode *curr2 = front;
                while(curr2->next != nullptr){
                    if((curr2->priority) <= (target->priority) && (curr2->next->priority) > (target->priority)){
                        target->next = curr2->next;
                        curr2->next = target;
                        break;
                    }else{
                        curr2 = curr2->next;
                    }
                }
            }
        }
    }
}

string LinkedListPatientQueue::toString() {
    if(front == nullptr){
        return "{}";
    }else if(front->next == nullptr){
        string toReturn = "{" + integerToString(front->priority) + ":" + front->name + "}";
        return toReturn;
    }
    string toReturn = "{";
    PatientNode *temp = front;
    while(temp->next != nullptr){
        toReturn += integerToString(temp->priority) + ":" + temp->name + SEPERATE;
        temp = temp->next;
    }
    toReturn += integerToString((temp->priority)) + ":" + temp->name + "}";
    return toReturn; // this is only here so it will compile
}

