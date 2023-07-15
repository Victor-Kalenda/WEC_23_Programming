//
// Created by Victor Kalenda on 2023-07-11.
//

#include <iostream>

using namespace std;


#ifndef WEC_EXAMPLE_DELIVERYLIST_H
#define WEC_EXAMPLE_DELIVERYLIST_H

#define MISSISSAUGA 2
#define START_TIME  float(6.0)

struct logistics{
    float start_time = START_TIME;
    float end_time = START_TIME;
    bool pickup = true;
    unsigned int start_destination = MISSISSAUGA;
    unsigned int end_destination = MISSISSAUGA;
    unsigned int start_quantity = 0;
    unsigned int end_quantity = 0;
};

struct delivery{
    explicit delivery(logistics value);
    logistics data;
    delivery* next = nullptr;
};

class DeliveryList {
    private:
        const unsigned int capacity = 700;
        unsigned int size;
        delivery* head;
    public:
        DeliveryList();
        ~DeliveryList();
        [[nodiscard]]bool empty() const;
        bool insert(logistics value);
        logistics select(unsigned int index);
        [[nodiscard]]unsigned int get_size() const;
        void print();
        void reverse_list();
};


#endif //WEC_EXAMPLE_DELIVERYLIST_H
