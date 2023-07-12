//
// Created by Victor Kalenda on 2023-07-11.
//

#include <iostream>

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
    size = 0;
}

bool DeliveryList:: insert(logistics value)
{
    if(size == capacity)
    {
        cout << "Invalid Index" << endl;
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
        cout << "Invalid Index" << endl;
        return {-1, -1, false, -1, -1, -1, -1};
    }
    delivery* temp = head;
    for(int i = 0; i < index; i++)
    {
        temp = temp->next;
    }
    return temp->data;
}
void DeliveryList:: output_delivery(ostream & output_file)
{
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
        cout << "delivery " << i << " start_time " << temp->data.start_time << " end_time " << temp->data.end_time << endl;
        temp = temp->next;
    }
}