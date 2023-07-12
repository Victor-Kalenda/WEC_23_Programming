//
// Created by Victor Kalenda on 2023-07-03.
//

#include "TruckList.h"
#include <iostream>


using namespace std;

truck:: truck()
{
    next = nullptr;
}

TruckList:: TruckList()
{
    size = 0;
    head = nullptr;
}
/*
TruckList:: ~TruckList()
{
    truck* temp = nullptr;
    while(head != nullptr)
    {
        temp = head->next;
        delete head;
        head = temp;
    }
    temp = nullptr;
    size = 0;
}
 */

// Insert truckers like a stack, order is irrelevant
bool TruckList:: insert(truck* new_truck)
{
    if(size == capacity)
    {
        cout << "Invalid Index" << endl;
        delete new_truck;
        new_truck = nullptr;
        return false;
    }
    new_truck->next = head;
    head = new_truck;
    size++;
    return true;
}

unsigned int TruckList:: get_size()
{
    return size;
}

void TruckList:: print()
{
    truck* temp = head;
    for(int i = 0; i < size; i++)
    {
        temp->data.deliveries.print();
        cout << temp->data.todays_hours << endl;
        temp = temp->next;
    }
    temp = nullptr;
}

truck* TruckList:: select(unsigned int index)
{
    if(index >= size)
    {
        cout << "Invalid Index" << endl;
        return head;
    }
    truck* temp = head;
    for(int i = 0; i < index; i++)
    {
        temp = temp->next;
    }
    return temp;
}