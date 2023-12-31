//
// Created by Victor Kalenda on 2023-07-03.
//

#ifndef WEC_EXAMPLE_TRUCKLIST_H
#define WEC_EXAMPLE_TRUCKLIST_H

#include "DeliveryList.h"

struct info{
    float daily_hours[7];
    bool available = true;
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
        const unsigned int capacity = 400;
        unsigned int size; // the overall size of the linked list containing trucker information
        truck* head;

    public:
        unsigned int late_deliveries = 0;
        TruckList();
        ~TruckList();
        bool insert(truck* new_truck);
        [[nodiscard]]unsigned int get_size() const;
        void print();
        truck* select(unsigned int index); // pointer to a pointer
};



#endif //WEC_EXAMPLE_TRUCKLIST_H
