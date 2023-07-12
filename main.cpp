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
#include "TruckList.h"

using namespace std;


Queue identify_priorities();
string get_city(istream & input_file, int & number_of_deliveries);
void ignore_characters(int count, istream & input_file);

TruckList send_trucks(Queue & packages);
void output_logistics(TruckList & deliveries);
void analyse_travel();
void ignore_cells(int count, istream & input_file);
int get_number(istream & input_file);
int get_day(istream & input_file);
int check_location(string city);
void add_trip(info & trucker, Queue & packages);

#define HOURLY               20
#define OVERTIME             30
#define OVERTIME_LAW         60
#define TRUCKER_BASE         200
#define LATE_FEE             200
#define COST_KM              0.68
#define SPEED                100.0
#define TRAFFIC_FACTOR       0.7 // Not yet implemented
#define DAILY_WORK_LAW       14.0
#define CITIES               10
#define MISSISSAUGA          2
#define TRUCK_CAPACITY       5

float *dist_array = new float[CITIES*CITIES];
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
    //priorities.print();

    TruckList deliveries = send_trucks(priorities);
    deliveries.print();
    // There is no longer a use for dist_array
    delete [] dist_array;
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
TruckList send_trucks(Queue & packages)
{
    TruckList workers;
    int current_day = 1;
    float time = 0.0;
    // Systematically go through each package allocating them, one trucker at a time
    while(packages.get_size() != 0)
    {
        // Keep adding drivers depending on demand on that day, Each day all truckers regain the ability to work
        while(packages.get_deadline(0) == current_day && packages.get_size() != 0)
        {
            truck* trucker = new truck();
            //info trucker;
            // Allocate packages to drivers until they are unable to deliver the packages legally
            while(trucker->data.todays_hours < DAILY_WORK_LAW && packages.get_size() != 0 && packages.get_deadline(0) == current_day)
            {
                // check if the driver has completed the packages for the day before
                if(packages.get_deadline(0) == current_day)
                {
                    // make the trucker travel to where they are needed
                    add_trip(trucker->data, packages);
                }
            }
            workers.insert(trucker);
        }
        current_day++;
        // reuse the truckers which are currently out of HQ
        int i = 0;
        while(i < workers.get_size() && packages.get_size() != 0 && packages.get_deadline(0) == current_day)
        {
            truck* trucker = workers.select(i);
            // reset the hours worked for the day
            trucker->data.todays_hours = 0;
            trucker->data.current_day = current_day;
            // Allocate packages to drivers until they are unable to deliver the packages legally
            while(trucker->data.todays_hours < DAILY_WORK_LAW && packages.get_size() != 0 && packages.get_deadline(0) == current_day)
            {
                // check if the driver has completed the packages for the day before
                if(packages.get_deadline(0) == current_day)
                {
                    // make the trucker travel to where they are needed
                    add_trip(trucker->data, packages);
                }
                else
                {
                    // keep reusing truckers in the field as long as possible
                    current_day = packages.get_deadline(0);
                }
            }
            i++;
        }
    }
    // return home can be implemented if need be
    return workers;
}

void add_trip(info & trucker, Queue & packages)
{
    logistics package;
    // determine if the trucker has just been reserved or if they are currently in the field
    if(!trucker.deliveries.empty())
    {
        package.start_destination = trucker.deliveries.select(0).end_destination;
        package.start_quantity = trucker.deliveries.select(0).end_quantity;
        package.start_time = (trucker.deliveries.select(0).end_time - trucker.deliveries.select(0).start_time) + (trucker.current_day - 1) * 24;
    }
    else
    {
        package.start_destination = MISSISSAUGA;
        package.end_destination = packages.get_destination(0);
    }
    // determine if the trucker needs to go home
    if(package.start_quantity == 0)
    {
        package.pickup = true;
        int target_warehouse = MISSISSAUGA;
        // add a trip to the most suitable warehouse for the next package destination
        /*
        int shortest_path = 1000;
        int target_warehouse = -1;
        // check which warehouse is closest to the next package destination
        for(int i = 10; i < 13; i++)
        {
            int travel_distance = dist_array[packages.get_destination(0) * CITIES + i];
            if(travel_distance < shortest_path)
            {
                shortest_path = travel_distance;
                target_warehouse = i;
            }
        }
         */
        package.end_destination = target_warehouse;
        package.end_time = package.start_time + dist_array[target_warehouse * CITIES + packages.get_destination(0)] / SPEED;
        package.end_quantity = TRUCK_CAPACITY;
    }
    else
    {
        // identify the next destination
        package.end_destination = packages.get_destination(0);
        package.pickup = false;
        package.end_time = package.start_time + dist_array[package.start_destination * CITIES + package.end_destination] / SPEED;
        // drop off as many packages as possible at the location, remove the packages that are at that location
        package.end_quantity = packages.offload(package.start_quantity, package.end_destination);
    }
    // determine if the trucker can travel to it's location with the hours it has already worked today
    if(trucker.todays_hours + (package.end_time - package.start_time) < DAILY_WORK_LAW)
    {
        trucker.todays_hours += package.end_time - package.start_time;
        trucker.deliveries.insert(package);
    }
    else
    {
        // clock out the trucker for the day
        trucker.todays_hours = DAILY_WORK_LAW;
    }
}

void output_logistics(TruckList & deliveries)
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
        // total cost of the driver
        // total time driving in the day
        // total distance travelled for the day
        // total cost of the day
    }
    expenses += late_deliveries * LATE_FEE + distance_travelled * COST_KM;

    output << "Number of trucks used - " << "Number of Late Deliveries - " << "Longest Work Day - " << "Total Overtime - "
            << "Total Distance - " << "Total Expenses" << endl;
    output << deliveries.get_size() << " - " <<  late_deliveries << " - " <<  longest_day << " - " <<  overtime
    << " - " << distance_travelled << " - " <<  expenses << endl;
    output.close();
}
