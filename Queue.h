//
// Created by Victor Kalenda on 2023-07-03.
//

#ifndef WEC_EXAMPLE_QUEUE_H
#define WEC_EXAMPLE_QUEUE_H


typedef int Datatype;

class Queue {
    private:
        struct package{
            package(Datatype time, Datatype location);
            Datatype destination;
            Datatype deadline;
            package* next;
        };
        package* head;
        package* tail;
        const unsigned int capacity = 200;
        unsigned int size;
    public:
        Queue();
        ~Queue();
        Datatype get_deadline(unsigned int index);
        Datatype get_destination(unsigned int index);
        unsigned int get_size() const;
        bool enqueue(Datatype time, Datatype drop_location);
        Datatype dequeue();
        int offload(int quantity, int destination, int deadline);
        bool reorder(const float * array);
        bool remove(unsigned int index);
        void print();
};


#endif //WEC_EXAMPLE_QUEUE_H
