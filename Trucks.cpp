//
// Created by Victor Kalenda on 2023-07-03.
//

#include "Trucks.h"
#include <iostream>


using namespace std;

Trucks:: Trucks()
{
    size = 0;
    workers = 0;
    head = nullptr;
}

Trucks:: ~Trucks()
{
    truck* temp = nullptr;
    while(head != nullptr)
    {
        temp = head->next;
        delete head;
        head = temp;
    }
    size = 0;
    workers = 0;
}

logistics Trucks:: select(unsigned int index)
{
    if(index >= size)
    {
        cout << "Invalid index" << endl;
        return {-1, -1, -1, -1, -1};
    }
    truck* temp = head;
    for(int i = 0; i < index; i++)
    {
        temp = temp->next;
    }
    return temp->data;
}

bool Trucks:: insert(truck value)
{
    if(size == capacity)
    {
        cout << "Invalid Index" << endl;
        return false;
    }
    truck* new_truck = new truck(value);
    new_truck->next = head;
    head = new_truck;
    size++;
    workers++;
    return true;
}

bool Trucks:: replace(logistics input, unsigned int index)
{
    if(index >= size)
    {
        cout << "Invalid Index" << endl;
        return false;
    }
    truck* temp = head;
    for(int i = 0; i < index; i++)
    {
        temp = temp->next;
    }
    temp->data = input;
    return true;
}

bool Trucks:: remove(unsigned int index)
{
    if(size == 0 || index >= size)
    {
        cout << "Invalid Index" << endl;
        return false;
    }
    if(size == 1)
    {
        delete head;
        head = nullptr;
        return true;
    }
    if(index == 0)
    {
        truck* temp = head;
        head = head->next;
        delete temp;
        temp = nullptr;
        size --;
        workers--;
        return true;
    }
    truck* prev = nullptr;
    truck* curr = head;
    for(int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }
    if(index == size - 1)
    {
        prev->next = nullptr;
    }
    else
    {
        prev->next = curr->next;
    }
    delete curr;
    size--;
    workers--;
    return true;
}

unsigned int Trucks:: get_size()
{
    return size;
}

unsigned int Trucks:: get_workers()
{
    return workers;
}

void Trucks:: decrement_workers()
{
    workers--;
}

void Trucks:: print()
{
    truck* temp = head;
    for(int i = 0; i < size; i++)
    {
        cout << "Trucker " << i << " distance = " << temp->data.total_distance << " - hours = " << temp->data.total_hours << endl;
        temp = temp->next;
    }
    temp = nullptr;
}
