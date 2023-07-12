//
// Created by Victor Kalenda on 2023-07-03.
//

#ifndef WEC_EXAMPLE_TRUCKLIST_H
#define WEC_EXAMPLE_TRUCKLIST_H

#include "DeliveryList.h"

struct info{
    float todays_hours = 0.0;
    unsigned int current_day = 1;
    DeliveryList deliveries;
};

struct truck{
    truck();
    info data;
    truck* next = nullptr;
};

class TruckList {
    private:
        const unsigned int capacity = 100;
        unsigned int size; // the overall size of the linked list containing trucker information
        truck* head;

    public:
        TruckList();
        //~TruckList();
        bool insert(truck* new_truck);
        unsigned int get_size();
        void print();
        truck* select(unsigned int index);
};



#endif //WEC_EXAMPLE_TRUCKLIST_H
