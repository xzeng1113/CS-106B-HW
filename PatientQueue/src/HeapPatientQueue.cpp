/*
 * File: LinkedListPaientQueue.cpp
 * --------------------------
 * Name: Xiaoxi Zeng
 * Section leader: Semir Shafi
 * This prgram implements a priority queue using a binary heap (min).
 */

#include "HeapPatientQueue.h"
#include "strlib.h"
#include "string.h"

const int INITIAL_CAPACITY = 8;
const string SEPERATE = ", ";

HeapPatientQueue::HeapPatientQueue() {
    capacity = INITIAL_CAPACITY;
    elements = new Patient[capacity];
    count = 0;
}

HeapPatientQueue::~HeapPatientQueue() {
    delete [] elements;
}

void HeapPatientQueue::clear() {
    count = 0;
}

string HeapPatientQueue::frontName() {
    string toReturn;
    if(count == 0){
        throw("There is no top. I must abort. I never learned how to love...");
    }else{
        toReturn = elements[1].name;
    }
    return toReturn;
}

int HeapPatientQueue::frontPriority() {
    int priority;
    if(count == 0){
        throw("There is no top. I must abort. I never learned how to love...");
    }else{
        priority = elements[1].priority;
    }
    return priority;
}

bool HeapPatientQueue::isEmpty() {
    return count == 0;
}

void HeapPatientQueue::newPatient(string name, int priority) {
    if(count == capacity){
        expand();
    }
    Patient patient;
    patient.name = name;
    patient.priority = priority;
    int index = count + 1;
    elements[index] = patient;
    count ++;
    while(index >= 1){
        if(elements[index / 2].priority > elements[index].priority){
            Patient temp = elements[index / 2];
            elements[index / 2] = elements[index];
            elements[index] = temp;
            index = index / 2;
        }else if(elements[index / 2].priority == elements[index].priority){
            if(elements[index / 2].name > elements[index].name){
                Patient temp = elements[index / 2];
                elements[index / 2] = elements[index];
                elements[index] = temp;
                index = index / 2;
            }else{
                break;
            }
        }else{
            break;
        }
    }

}

string HeapPatientQueue::processPatient() {
    string toReturn;
    if(count == 0){
        throw("There is no top. I must abort. I never learned how to love...");
    }else{
        toReturn = elements[1].name;
        elements[1] = elements[count];
        count --;
        int currIndex = 1;
        while(currIndex < count){
            int leftChild = 2 * currIndex;
            int rightChild = 2 * currIndex + 1;
            int smallerChild;
            //Make sure both children exist
            if((rightChild) <= count){
                //Choose the smaller child
                if(elements[leftChild].priority > elements[rightChild].priority){
                    smallerChild = rightChild;
                }else if(elements[leftChild].priority < elements[rightChild].priority){
                    smallerChild = leftChild;
                }else{
                    //If the priority of left and right child are the same, then compare them in terms of alphabetical order
                    if(elements[leftChild].name > elements[rightChild].name){
                        smallerChild = leftChild;
                    }else{
                        smallerChild = rightChild;
                    }
                }
            }else{
                smallerChild = leftChild; //If the right child does not exist, then make the left child the smaller child
            }
            if(elements[currIndex].priority > elements[smallerChild].priority){
                Patient temp = elements[currIndex];
                elements[currIndex] = elements[smallerChild];
                elements[smallerChild] = temp;
                currIndex = smallerChild;
            }else if(elements[currIndex].priority == elements[smallerChild].priority){
                if(elements[currIndex].name < elements[smallerChild].name){
                    Patient temp = elements[currIndex];
                    elements[currIndex] = elements[smallerChild];
                    elements[smallerChild] = temp;
                    currIndex = smallerChild;
                }else{
                    break;
                }
            }else{
                break;
            }
        }
    }
    return toReturn;
}

void HeapPatientQueue::upgradePatient(string name, int newPriority) {
    // TODO: write this function
    bool nameExists = false;
    int position = 0;
    for(int i = 1; i <= count; i ++){
        if(elements[i].name == name){
            nameExists = true;
            if(elements[i].priority <= newPriority){
                throw("You are decreasing the priority.");
            }else{
                elements[i].priority = newPriority;
                position = i;
                break;
            }
        }
    }
    if(!nameExists){
        throw("That patient is not in the queue.");
    }
    if(position != 0){
        while(position >= 1){
            if(elements[position / 2].priority > elements[position].priority){
                Patient temp = elements[position / 2];
                elements[position / 2] = elements[position];
                elements[position] = temp;
                position = position / 2;
            }else if(elements[position / 2].priority == elements[position].priority){
                if(elements[position / 2].name > elements[position].name){
                    Patient temp = elements[position / 2];
                    elements[position / 2] = elements[position];
                    elements[position] = temp;
                    position = position / 2;
                }else{
                    break;
                }
            }else{
                break;
            }
        }
    }
}

string HeapPatientQueue::toString() {
    // TODO: write this function
    if(count == 0 ){
        return "{}";
    }
    string toReturn = "{";
    for(int i = 1; i < count; i++){
        toReturn += integerToString(elements[i].priority) + ":" + elements[i].name + SEPERATE;
    }
    toReturn += integerToString(elements[count].priority) + ":" + elements[count].name + "}";
    return toReturn; // this is only here so it will compile
}

void HeapPatientQueue::expand(){
    //Ask for new space for a new array
    Patient *newElements = new Patient[capacity * 2];
    //Copy the values over
    for(int i = 0; i < count; i++){
        newElements[i] = elements[i];
    }
    //Delete old array
    delete [] elements;
    //Point elements to new array
    elements = newElements;
    //Update capacity
    capacity *= 2;
}
