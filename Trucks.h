//
// Created by Victor Kalenda on 2023-07-03.
//

#ifndef WEC_EXAMPLE_TRUCKS_H
#define WEC_EXAMPLE_TRUCKS_H

#define MISSISSAUGA 2

struct logistics{
     float total_hours = 0;
     int total_distance = 0;
     float longest_day = 0;
     int late_deliveries = 0;
     int current_position = MISSISSAUGA; // Begin truckers at HQ1 by default
     int current_day = 0;
};

struct truck {
    logistics data;
    truck* next;
};

class Trucks {
    private:
        const unsigned int capacity = 200;
        unsigned int size; // the overall size of the linked list containing trucker information
        unsigned int workers; // the amount of truckers in the field, ready to deliver packages
        truck* head;
    public:
        Trucks();
        ~Trucks();
        logistics select(unsigned int index);
        bool insert(truck value);
        bool remove(unsigned int index);
        bool replace(logistics input, unsigned int index);
        unsigned int get_size();
        unsigned int get_workers();
        void decrement_workers();
        void print();
};


#endif //WEC_EXAMPLE_TRUCKS_H
