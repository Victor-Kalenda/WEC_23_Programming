//
// Created by Victor Kalenda on 2023-07-11.
//

#include <iostream>
#include <iomanip>


using namespace std;

#include "DeliveryList.h"

delivery:: delivery(logistics value)
{
    data.end_destination = value.end_destination;
    data.start_destination = value.start_destination;
    data.end_quantity = value.end_quantity;
    data.start_quantity = value.start_quantity;
    data.end_time = value.end_time;
    data.start_time = value.start_time;
    data.pickup = value.pickup;
}

DeliveryList:: DeliveryList()
{
    head = nullptr;
    size = 0;
}
DeliveryList:: ~DeliveryList()
{
    delivery* temp = nullptr;
    while(head != nullptr)
    {
        temp = head->next;
        delete head;
        head = temp;
    }
    temp = nullptr;
    size = 0;
}

bool DeliveryList:: insert(logistics value)
{
    if(size == capacity)
    {
        cout << "DeliveryList Reached Capacity" << endl;
        return false;
    }
    delivery* new_delivery = new delivery(value);
    new_delivery->next = head;
    head = new_delivery;
    size++;
    return true;
}
logistics DeliveryList:: select(unsigned int index)
{
    if(index >= size)
    {
        cout << "Invalid Index DeliveryList Select" << endl;
        return {1000, 1000, false, 1000, 1000, 1000, 1000};
    }
    delivery* temp = head;
    for(int i = 0; i < index; i++)
    {
        temp = temp->next;
    }
    return temp->data;
}

unsigned int DeliveryList:: get_size() const
{
    return size;
}

bool DeliveryList:: empty() const
{
    return (size == 0);
}

void DeliveryList:: print()
{
    delivery* temp = head;
    for(int i = 0; i < size; i++)
    {
        cout << "delivery " << setw(2) << i << "| start_time " << setw(7) << temp->data.start_time << "| end_time "
            << setw(7) << temp->data.end_time << "| start_quantity " << temp->data.start_quantity << "| end_quantity "
            << temp->data.end_quantity << "| start_destination " << setw(2) << temp->data.start_destination
            << "| end_destination " << setw(2) << temp->data.end_destination << endl;
        temp = temp->next;
    }
}

// reorders the deliveries in order of time
void DeliveryList:: reverse_list()
{
    if(size == 0)
    {
        return;
    }
    delivery* prev = nullptr;
    delivery* curr = head;
    while(head != nullptr)
    {
        head = head->next;
        curr->next = prev;
        prev = curr;
        curr = head;
    }
    head = prev;
    prev = nullptr;
    curr = nullptr;
}