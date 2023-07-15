//
// Created by Victor Kalenda on 2023-07-03.
//

#include "Queue.h"
#include <iostream>

#define CITIES 13
#define MISSISSAUGA 2

using namespace std;

Queue:: package:: package(Datatype time, Datatype location)
{
    deadline = time;
    destination = location;
    next = nullptr;
}

Queue:: Queue()
{
    size = 0;
    head = nullptr;
    tail = nullptr;
}

Queue:: ~Queue()
{
    package *temp = nullptr;
    while(head != nullptr)
    {
        temp = head->next;
        delete head;
        head = temp;
    }
    size = 0;
}

bool Queue:: enqueue(Datatype time, Datatype drop_location)
{
    if(size == capacity)
    {
        cout << "Queue is full" << endl;
        return false;
    }
    else
    {
        package *new_package = new package(time, drop_location);
        if(size == 0)
        {
            head = new_package;
            tail = head;
        }
        else
        {
            tail->next = new_package;
            tail = new_package;
        }
        size++;
        return true;
    }
}

Datatype Queue:: dequeue()
{
    if(size == 0)
    {
        cout << "Queue is empty" << endl;
        return -1;
    }
    else
    {
        Datatype return_package = head->deadline;
        if(size == 1)
        {
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        else
        {
            package *next_package = head->next;
            delete head;
            head = next_package;
        }
        size--;
        return return_package;
    }
}

Datatype Queue:: get_deadline(unsigned int index)
{
    if (size == 0)
    {
        cout << "Queue is empty" << endl;
        return -1;
    }
    if (index >= size)
    {
        cout << "Invalid index" << endl;
        return -1;
    }
    else
    {
        package* temp = head;
        for(int i = 0; i < index; i++)
        {
            temp = temp->next;
        }
        return temp->deadline;
    }
}

Datatype Queue:: get_destination(unsigned int index)
{
    if (size == 0)
    {
        cout << "Queue is empty" << endl;
        return -1;
    }
    if (index >= size)
    {
        cout << "Invalid Index" << endl;
        return -1;
    }
    else
    {
        package* temp = head;
        for(int i = 0; i < index; i++)
        {
            temp = temp->next;
        }
        return temp->destination;
    }
}

unsigned int Queue:: get_size() const
{
    return size;
}

bool Queue:: remove(unsigned int index)
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
        size--;
        return true;
    }
    if(index == 0)
    {
        package* temp = head;
        head = head->next;
        delete temp;
        temp = nullptr;
        size--;
        return true;
    }
    package* prev = nullptr;
    package* curr = head;
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
    return true;
}

// reorders packages according to deadline and destination
bool Queue:: reorder(const float * array)
{
    if(size == 0)
    {
        return false;
    }
    // check the distance from each warehouse to a packages destination, log the data in an array
    float path_lengths[size];
    for(int i = 0; i < size; i++)
    {
        float shortest_path = 1000;
        for(int j = 10; j < CITIES; j++)
        {
            if(shortest_path > array[((*this).get_destination(i) - 1) * CITIES + (j)])
            {
                shortest_path = array[((*this).get_destination(i) - 1) * CITIES + (j)];
            }
        }
        path_lengths[i] = shortest_path;
    }
    // reorder the queue from shortest to longest trips from the warehouses to the destinations
    Queue temp;
    for(int i = 0; i < size; i++)
    {
        int shortest_trip = 0;
        for (int j = 0; j < size; j++)
        {
            if (path_lengths[shortest_trip] > path_lengths[j])
            {
                shortest_trip = j;
            }
        }
        temp.enqueue((*this).get_deadline(shortest_trip), (*this).get_destination(shortest_trip));
        path_lengths[shortest_trip] = 1000;
    }
    // Destruct the old Queue
    (*this).~Queue();
    // Assign the new Queue
    head = temp.head;
    tail = temp.tail;
    size = temp.size;
    temp.head = nullptr;
    temp.tail = nullptr;


    // reorder the queue from earliest to latest deadline
    for(int i = 0; i < size; i++)
    {
        int earliest_deadline = 8;
        int index = -1;
        for(int j = 0; j < size - i; j++)
        {
            if(earliest_deadline > (*this).get_deadline(j))
            {
                earliest_deadline = (*this).get_deadline(j);
                index = j;
            }
        }
        (*this).enqueue((*this).get_deadline(index), (*this).get_destination(index));
        (*this).remove(index);
    }
    return true;
}

void Queue:: print()
{
    package* temp = head;
    cout << "Format: deadline - destination" << endl;
    cout << "Size = " << size << endl;
    for(int i = 0; i < size; i++)
    {
        cout << "Package #" << i + 1 << " = " << temp->deadline << " - " << temp->destination << endl;
        temp = temp->next;
    }
    temp = nullptr;
}

int Queue:: offload(int quantity, int destination, int deadline)
{
    int num_packages = quantity;
    int index = 0;
    int queue_size = size;
    for(int i = 0; i < queue_size; i++)
    {
        if((*this).get_destination(index) == destination && (*this).get_deadline(index) == deadline)
        {
            (*this).remove(index);
            num_packages--;
            if(num_packages == 0)
            {
                return num_packages;
            }
        }
        else
        {
            index++;
        }
    }
    return num_packages;
}