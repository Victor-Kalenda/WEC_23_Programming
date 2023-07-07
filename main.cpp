/*
 * Contributors: Alexander Hamersma, Victor Kalenda
 *
 * Example solution to 2023 WEC Programming Question
 *
 * Difficulty level: 4
 *
 * All Drivers start work at 00:00 each day and work 14 hours straight
 */


#include <iostream>
#include <fstream>
#include "Queue.h"
#include "Trucks.h"

using namespace std;

struct expedition{
    int shortest_path = 2000; // Longer than any distance in the given dataset
    int dist_to_k = 0;
    int dist_to_dest = 0;
    float time_to_k = 0;
    float time_to_dest = 0;
};

Queue identify_priorities();
string get_city(istream & input_file, int & number_of_deliveries);
void ignore_characters(int count, istream & input_file);

Trucks send_trucks(Queue & packages);
void output_logistics(Trucks & deliveries);
void analyse_travel();
void ignore_cells(int count, istream & input_file);
int get_number(istream & input_file);
int get_day(istream & input_file);
int check_location(string city);
expedition get_trip(logistics & trucker, Queue & packages);
expedition return_home(logistics & trucker);

#define HOURLY               20
#define OVERTIME             30
#define OVERTIME_LAW         60
#define TRUCKER_BASE         200
#define LATE_FEE             200
#define COST_KM              0.68
#define SPEED                100.0
#define SPEED_EXCEPTION      110.0
#define TRAFFIC_FACTOR       0.5 // Not yet implemented
#define DAILY_WORK_LAW       14.0
#define CITIES               10
#define NIAGARA_FALLS        6 // Index of Niagara falls, for special case where speed changes
#define MISSISSAUGA          2

int *dist_array = new int[CITIES*CITIES];
char city_array[20];
int city_index = 0;

int main()
{

    analyse_travel();
    // Print array to test analysis
    /*
    for(int i = 0; i < CITIES; i++)
    {
        for(int j = 0; j < CITIES; j++)
        {
            cout << dist_array[ i * CITIES + j] << " - ";
        }
        cout << endl;
    }
    */

    Queue priorities = identify_priorities();
    priorities.print();

    //Trucks deliveries = send_trucks(priorities);
    //deliveries.print();
    // There is no longer a use for dist_array
    delete dist_array;
    dist_array = nullptr;

    //output_logistics(deliveries);
    return 0;
}

void analyse_travel()
{
    ifstream distances("Distance.csv");
    if(!distances)
    {
        cout << "File did not Open" << endl;
        return;
    }

    // Ignore the first line
    ignore_cells(22, distances);

    for(int i = 0; i < CITIES; i++)
    {
        // first item is the string location, not important
        ignore_cells(1, distances);
        for(int j = 0; j < CITIES; j++)
        {
            char test;
            if(i == j)
            {
                // distances between the same city
                dist_array[i * CITIES + j] = 0;
                ignore_cells(1, distances);
            }
            else
            {
                dist_array[i * CITIES + j] = get_number(distances);
                //distances >> test;
                //cout << test;
                //distances >> dist_array[i*j];
            }
        }
        // Ignore the rest of the data
        ignore_cells(11, distances);
    }
    distances.close();
}
void ignore_cells(int count, istream & input_file)
{
    int comma_count = 0;
    char character;
    while(comma_count < count && !input_file.eof())
    {
        if(input_file >> character)
        {
            if(character == ',')
            {
                comma_count++;
            }
        }
    }
}
int get_number(istream & input_file)
{
    char array[4] = {0, 0, 0, 0};
    char character = NULL;
    int i = 0;
    while(character != ',')
    {
        input_file >> character;
        if(character != ',')
        {
            array[i] = character;
            i++;
        }
    }
    return atoi(array);
}

// Identifies which packages should be sent first and which ones should be sent last
// Priorities are ordered according to deadline and destination
Queue identify_priorities()
{
    // Collect all packages into a Queue
    Queue packages;
    ifstream general("general.csv");
    if(!general)
    {
        cout << "File did not Open" << endl;
        return packages;
    }
    ignore_cells(1, general);
    // Ignore "Deliveries"
    ignore_characters(10, general);
    for(int i = 0; i < CITIES; i++)
    {
        int demand = 0;
        string city = get_city(general, demand);
        //cout << city << " = " << demand << endl;
        ifstream location(city + ".csv");
        if(!location)
        {
            cout << "File did not Open" << endl;
            return packages;
        }
        // ignore the first line
        ignore_cells(2, location);
        for(int i = 0; i < demand; i++)
        {
            packages.enqueue(get_day(location), check_location(city));
        }
        location.close();
    }
    general.close();

    //cout << "Newly made Queue" << endl;
    //packages.print();

    // Reorder the packages according to deadline and destination
    packages.reorder(dist_array);

    //cout << "After Reorder" << endl;
    //packages.print();

    return packages;
}

string get_city(istream & input_file, int & number_of_deliveries)
{
    char city[20] = {NULL};
    char number_array[3];
    char character = '0';
    int j = 0;
    // get the city
    while(character != ',')
    {
        input_file >> character;
        if(character != ',')
        {
            city_array[city_index] = character;
            city_index++;
        }
    }
    city_index = 0;
    // transfer city_array to city, reset city_array
    for(int i = 0; i < 20; i++)
    {
        city[i] = city_array[i];
        city_array[i] = NULL;
    }
    character = '0';
    // get the number of deliveries
    while(character > 47 && character < 58 && !input_file.eof())
    {
        if(input_file >> character)
        {
            // account for the first letter in the city
            if (character > 58)
            {
                // remember the character for the next city
                city_array[city_index] = character;
                city_index++;
            }
            else
            {
                number_array[j] = character;
                j++;
            }
        }
    }
    number_of_deliveries = atoi(number_array);

    return city;
}
void ignore_characters(int count, istream & input_file)
{
    char junk;
    for(int i = 0; i < count; i++)
    {
        input_file >> junk;
    }
}
int get_day(istream & input_file)
{
    // The first 2 letters is all we need to identify the day
    char deadline[2] = {NULL};
    input_file >> deadline[0] >> deadline[1];
    // Jump to the next package
    ignore_cells(1, input_file);

    if(deadline[0] == 'M' && deadline[1] == 'o')
    {
        return 1;
    }
    else if(deadline[0] == 'T' && deadline[1] == 'u')
    {
        return 2;
    }
    else if(deadline[0] == 'W' && deadline[1] == 'e')
    {
        return 3;
    }
    else if(deadline[0] == 'T' && deadline[1] == 'h')
    {
        return 4;
    }
    else if(deadline[0] == 'F' && deadline[1] == 'r')
    {
        return 5;
    }
    else if(deadline[0] == 'S' && deadline[1] == 'a')
    {
        return 6;
    }
    else if(deadline[0] == 'S' && deadline[1] == 'u')
    {
        return 7;
    }
    // Fail case
    return 0;
}
int check_location(string city)
{
    if(city == "Tilbury")
    {
        return 1;
    }
    if(city == "Mississauga")
    {
        return 2;
    }
    if(city == "Cornwall")
    {
        return 3;
    }
    if(city == "London")
    {
        return 4;
    }
    if(city == "Windsor")
    {
        return 5;
    }
    if(city == "Niagara_Falls")
    {
        return 6;
    }
    if(city == "Barrie")
    {
        return 7;
    }
    if(city == "Kingston")
    {
        return 8;
    }
    if(city == "Huntsville")
    {
        return 9;
    }
    if(city == "North_Bay")
    {
        return 10;
    }
}


// Send the trucks out, eliminating each package from the queue with as few trucks as possible
// return a list of the trucks required for the week with information on their projected journeys
// This implementation will push the most amount of work out of truckers possible (14 hour daily work law)
Trucks send_trucks(Queue & packages)
{
    Trucks workers;
    int today = 1;
    // Systematically go through each package allocating them, one trucker at a time
    while(packages.get_size() != 0)
    {
        // Keep adding drivers depending on demand on that day, Each day all truckers regain the ability to work
        while(packages.get_deadline(0) == today && packages.get_size() != 0)
        {
            truck trucker;
            float todays_hours = 0;
            // Allocate packages to drivers until they are unable to deliver the packages legally
            while(todays_hours < DAILY_WORK_LAW && packages.get_size() != 0)
            {
                // check if the driver has completed the packages for the day before
                if(packages.get_deadline(0) != today)
                {
                    // reset the hours worked for the day
                    todays_hours = 0;
                }
                // Find the distance of the shortest path from current position to warehouse to destination
                expedition trip = get_trip(trucker.data, packages);

                todays_hours += trip.time_to_k + trip.time_to_dest;
                // determine if the trucker can handle the package, if he can, dequeue the package
                if(todays_hours < DAILY_WORK_LAW)
                {
                    trucker.data.total_distance += trip.dist_to_k + trip.dist_to_dest;
                    trucker.data.total_hours += trip.time_to_k + trip.time_to_dest;
                    trucker.data.current_position = packages.get_destination(0);
                    if(trucker.data.longest_day < todays_hours)
                    {
                        trucker.data.longest_day = todays_hours;
                    }
                    trucker.data.late_deliveries = 0;
                    packages.dequeue();
                }
                // if the trucker cannot handle the package, log the trucker and start again with a new trucker
                else
                {
                    trucker.data.current_day = packages.get_deadline(0);
                    workers.insert(trucker);
                }
            }
        }
        today++;
        // reuse the truckers which are currently out of HQ
        int i = 0;
        cout << workers.get_workers()<< packages.get_size() << endl;
        while(i < workers.get_workers() && packages.get_size() != 0)
        {
            float todays_hours = 0;
            while(todays_hours < DAILY_WORK_LAW && packages.get_size() != 0)
            {
                if(packages.get_deadline(0) != today)
                {
                    // Send the remaining truckers home, these truckers will be at the end of the list
                    // Truckers sent home will not be manipulated for the rest of send_trucks
                    while(i < workers.get_size())
                    {
                        logistics trucker = workers.select(i);
                        expedition trip = return_home(trucker);
                        workers.replace(trucker, i);
                        workers.decrement_workers();
                        i++;
                    }
                    today++;
                    // Set i back to zero and remain in the for loop
                    i = 0;
                }
                else
                {
                    logistics trucker = workers.select(i);
                    expedition trip = get_trip(trucker, packages);
                    todays_hours += trip.time_to_k + trip.time_to_dest;
                    // determine if the trucker can handle the package, if he can, dequeue the package
                    if (todays_hours < DAILY_WORK_LAW)
                    {
                        trucker.total_distance += trip.dist_to_k + trip.dist_to_dest;
                        trucker.total_hours += trip.time_to_k + trip.time_to_dest;
                        trucker.current_position = packages.get_destination(0);
                        if(trucker.longest_day < todays_hours)
                        {
                            trucker.longest_day = todays_hours;
                        }
                        trucker.late_deliveries = 0;
                        packages.dequeue();
                    }
                    // if the trucker cannot handle the package, log the trucker and start again with a new trucker
                    else
                    {
                        trucker.current_day = packages.get_deadline(0);
                        // update the truckers information according to the new packages you allocated to them
                        workers.replace(trucker, i);
                    }
                }
            }
        }
    }
    // Bring each truck back to headquarters at the end of their trips
    for(int i = 0; i < workers.get_size(); i++)
    {
        logistics trucker = workers.select(i);
        expedition trip = return_home(trucker);
        workers.replace(trucker, i);
        workers.decrement_workers();
    }
    return workers;
}

expedition get_trip(logistics & trucker, Queue & packages)
{
    expedition temp;
    temp.dist_to_k = dist_array[(trucker.current_position - 1) * CITIES + (MISSISSAUGA - 1)];
    temp.dist_to_dest = dist_array[(MISSISSAUGA - 1) * CITIES + (packages.get_destination(0) - 1)];
    if(trucker.current_position == NIAGARA_FALLS)
    {
        temp.time_to_k = float(temp.dist_to_k)/ SPEED_EXCEPTION;
    }
    else
    {
        temp.time_to_k = float(temp.dist_to_k) / SPEED;
    }

    // if the destination is Niagara Falls
    if (packages.get_destination(0) == NIAGARA_FALLS)
    {
        temp.time_to_dest = float(temp.dist_to_dest) / SPEED_EXCEPTION;
    }
    else
    {
        temp.time_to_dest = float(temp.dist_to_dest) / SPEED;
    }
    //cout << "curr_pos = " << trucker.current_position << " destination = " << packages.get_destination(0) << " - " << temp.dist_to_k + temp.dist_to_dest << " - " << temp.time_to_k + temp.time_to_dest << endl;
    /*
    for (int k = 10; k < 13; k++)
    {

        temp.dist_to_k = dist_array[trucker.current_position * CITIES + k];
        temp.dist_to_dest = dist_array[k * CITIES + packages.get_destination(0)];
        temp.shortest_path = temp.dist_to_k + temp.dist_to_dest;
        // if the trucker is currently in Niagara Falls
        if(trucker.current_position == NIAGARA_FALLS)
        {
            temp.time_to_k = temp.dist_to_k / SPEED_EXCEPTION;
        }
        else
        {
            temp.time_to_k = temp.dist_to_k / SPEED;
        }

        // if the destination is Niagara Falls
        if (packages.get_destination(0) == NIAGARA_FALLS)
        {
            temp.time_to_dest = temp.dist_to_dest / SPEED_EXCEPTION;
        }
        else
        {
            temp.time_to_dest = temp.dist_to_dest / SPEED;
        }
    }
    */
    return temp;
}

expedition return_home(logistics & trucker)
{
    expedition temp;
    temp.dist_to_dest = dist_array[(trucker.current_position - 1) * CITIES + (MISSISSAUGA - 1)];
    temp.shortest_path = temp.dist_to_dest;
    // if the trucker is currently in Niagara Falls
    if(trucker.current_position == NIAGARA_FALLS)
    {
        temp.time_to_dest = temp.dist_to_dest / SPEED_EXCEPTION;
    }
    else
    {
        temp.time_to_dest = temp.dist_to_dest / SPEED;
    }
}

void output_logistics(Trucks & deliveries)
{
    ofstream output("output.csv");
    if(!output)
    {
        cout << "File did not Open" << endl;
        return;
    }
    // final totals
    int late_deliveries = 0;
    int longest_day = 0;
    int overtime = 0;
    int distance_travelled = 0;
    int expenses = 0;

    for(int i = 0; i < deliveries.get_size(); i++)
    {
        logistics temp = deliveries.select(i);
        late_deliveries += temp.late_deliveries;
        if(longest_day < temp.longest_day)
        {
            longest_day = temp.longest_day;
        }
        if(temp.total_hours > OVERTIME_LAW)
        {
            overtime += temp.total_hours - OVERTIME_LAW;
            expenses += (temp.total_hours - OVERTIME_LAW) * OVERTIME + (OVERTIME_LAW * HOURLY);
        }
        else
        {
            expenses += temp.total_hours * HOURLY;
        }
        distance_travelled += temp.total_distance;
        expenses += TRUCKER_BASE;
    }
    expenses += late_deliveries * LATE_FEE + distance_travelled * COST_KM;

    output << "Number of trucks used - " << "Number of Late Deliveries - " << "Longest Work Day - " << "Total Overtime - "
            << "Total Distance - " << "Total Expenses" << endl;
    output << deliveries.get_size() << " - " <<  late_deliveries << " - " <<  longest_day << " - " <<  overtime
    << " - " << distance_travelled << " - " <<  expenses << endl;
    output.close();
}
