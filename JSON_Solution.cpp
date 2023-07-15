/*
 * Created by Victor Kalenda on 2023-04-03
 *
 * Example solution to 2023 WEC Programming Question
 *
 * Difficulty level: 4
 *
 */


#include <iostream>
#include <fstream>
#include "Queue.h"
#include "TruckList.h"
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::ordered_json;

using namespace std;

void analyse_travel();
void print_distances();

Queue identify_priorities();
string get_city(istream & input_file, int & number_of_deliveries);
void ignore_characters(int count, istream & input_file);
int get_day(istream & input_file);
int check_location(string city);

TruckList send_trucks(Queue & packages);
float get_weeks_hours(info & trucker);
void add_trip(info & trucker, Queue & packages);

void output_logistics(TruckList & trucks);
string get_day(unsigned int day);
string get_location(unsigned int city);
string get_delivery_type(bool pickup);
string convert_time(float time);
string convert_hours(float time);
float round_2_dec(float num);

#define HOURLY               20
#define OVERTIME             30.0
#define OVERTIME_LAW         60.0
#define TRUCKER_BASE         300.0
#define LATE_FEE             200 // Not implemented because system is designed to make late fees an impossibility
#define COST_KM              0.68
#define SPEED                100.0
#define TRAFFIC_FACTOR       0.7 // Not yet implemented
#define DAILY_WORK_LAW       14.0
#define WEEKLY_WORK_LAW      70.0
#define CITIES               13
#define MISSISSAUGA          2
#define TRUCK_CAPACITY       5
#define START_TIME           6.0

float *dist_array = new float[CITIES*CITIES];
char city_array[20];
int city_index = 0;

int main()
{
    analyse_travel();
    // Print array to test analysis
    //print_distances();

    Queue packages = identify_priorities();
    // print queue to test the addition and reordering of data into the queue
    //packages.print();

    TruckList trucks = send_trucks(packages);
    // print trucklist to see how the truckers were added and what data is contained in each of them
    //trucks.print();

    output_logistics(trucks);

    // There is no longer a use for dist_array
    delete [] dist_array;
    dist_array = nullptr;

    return 0;
}

/*
 * Analyse the data in the Distance.csv file. Correctly transfer it to a dynamically allocated 2D array. dist_array
 * will be used to determine the distances of locations from one another in the network.
 */
void analyse_travel()
{
    ifstream input_file("json/distance.json");
    if(!input_file)
    {
        cout << "File did not Open" << endl;
        return;
    }
    json distances = json::parse(input_file);
    for(int i = 0; i < CITIES; i++)
    {
        // first item is the string location, not important
        for(int j = 0; j < CITIES; j++)
        {
            if(i == j)
            {
                // distances between the same city
                dist_array[i * CITIES + j] = 0;
            }
            else
            {
                dist_array[i * CITIES + j] = distances[get_location(i + 1)][get_location(j + 1)];
            }
        }
    }
    input_file.close();
}
void print_distances()
{
    for(int i = 0; i < CITIES; i++)
    {
        for(int j = 0; j < CITIES; j++)
        {
            cout << dist_array[ i * CITIES + j] << " - ";
        }
        cout << endl;
    }
}

/*
 * Identifies which packages should be sent first and which ones should be sent last. Priorities are ordered according
 * to deadline and destination.
 */
Queue identify_priorities()
{
    // Collect all packages into a Queue
    Queue packages;
    ifstream input_file("json/input.json");
    if(!input_file)
    {
        cout << "File did not Open" << endl;
        return packages;
    }

    json demand = json::parse(input_file);

    for(int city = 0; city < CITIES; city++)
    {
        for(int day = 0; day < 7; day++)
        {
            for(int num_packages = 0; num_packages < demand[get_location(city + 1)][get_day(day)]; num_packages++)
            {
                packages.enqueue(day + 1, city + 1);
            }
        }
    }
    input_file.close();

    //cout << "Newly made Queue" << endl;
    //packages.print();

    // Reorder the packages according to deadline and destination
    packages.reorder(dist_array);

    //cout << "After Reorder" << endl;
    //packages.print();

    return packages;
}

/*
 * Send the trucks out, eliminating each package from the queue with as few trucks as possible. return a list of the
 * trucks required for the week with information on their projected journeys. This implementation will push the most
 * amount of work out of truckers possible (14 hour daily work law) without exceeding the 70 hour weekly work law.
 */
TruckList send_trucks(Queue & packages)
{
    TruckList workers;
    int current_day = 1;
    // Systematically go through each package allocating them, one trucker at a time
    while(packages.get_size() != 0)
    {
        // Keep adding drivers depending on demand on that day, Each day all truckers regain the ability to work
        while(packages.get_deadline(0) == current_day && packages.get_size() != 0)
        {
            truck* trucker = new truck();
            trucker->data.current_day = current_day;
            // Allocate packages to drivers until they are unable to deliver the packages legally
            while(trucker->data.available && packages.get_size() != 0 && packages.get_deadline(0) == current_day)
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
            trucker->data.daily_hours[current_day - 1] = 0;
            trucker->data.available = true;
            trucker->data.current_day = current_day;
            // Allocate packages to drivers until they are unable to deliver the packages legally
            while(trucker->data.available && packages.get_size() != 0 && packages.get_deadline(0) == current_day)
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
        if(trucker.daily_hours[trucker.current_day - 1] == 0)
        {
            // make all truckers start driver at 06:00 and work 14 hours straight to 20:00
            package.start_time = (trucker.current_day - 1) * 24 + START_TIME;
        }
        else
        {
            package.start_time = trucker.deliveries.select(0).end_time;
        }
    }
    else
    {
        package.start_time = (trucker.current_day - 1) * 24 + START_TIME;
        package.start_destination = MISSISSAUGA;
        package.end_destination = packages.get_destination(0);
    }
    // determine if the trucker needs to go home
    if(package.start_quantity == 0)
    {
        package.pickup = true;
        // add a trip to the most suitable warehouse for the next package destination
        int shortest_path = 1000;
        int target_warehouse = -1;
        // check which warehouse is closest to the next package destination
        for(int i = 10; i < 13; i++)
        {
            int travel_distance = dist_array[(packages.get_destination(0) - 1) * CITIES + (i)];
            if(travel_distance < shortest_path)
            {
                shortest_path = travel_distance;
                target_warehouse = i;
            }
        }
        package.end_destination = target_warehouse;
        package.end_time = package.start_time + dist_array[(target_warehouse - 1) * CITIES + (packages.get_destination(0) - 1)] / SPEED;
        // determine if the trucker can travel to its location with the hours it has already worked today and over the week
        float weeks_hours = get_weeks_hours(trucker);
        float trip = package.end_time - package.start_time;
        if(trucker.daily_hours[trucker.current_day - 1] + trip < DAILY_WORK_LAW && weeks_hours + trip < WEEKLY_WORK_LAW)
        {
            package.end_quantity = TRUCK_CAPACITY;
            trucker.daily_hours[trucker.current_day - 1] += package.end_time - package.start_time;
            trucker.deliveries.insert(package);
        }
        else
        {
            // clock out the trucker for the day
            trucker.available = false;
        }
    }
    else
    {
        // identify the next destination
        package.end_destination = packages.get_destination(0);
        package.pickup = false;
        package.end_time = package.start_time + dist_array[(package.start_destination - 1) * CITIES + (package.end_destination - 1)] / SPEED;

        // determine if the trucker can travel to its location with the hours it has already worked today and over the week
        float weeks_hours = get_weeks_hours(trucker);
        float trip = package.end_time - package.start_time;
        if(trucker.daily_hours[trucker.current_day - 1] + trip < DAILY_WORK_LAW && weeks_hours + trip < WEEKLY_WORK_LAW)
        {
            // drop off as many packages as possible at the location, dequeue the packages that have been delivered
            package.end_quantity = packages.offload(package.start_quantity, package.end_destination, trucker.current_day);
            trucker.daily_hours[trucker.current_day - 1] += package.end_time - package.start_time;
            trucker.deliveries.insert(package);
        }
        else
        {
            // clock out the trucker for the day
            trucker.available = false;
        }
    }
}
float get_weeks_hours(info & trucker)
{
    float weeks_hours = 0;
    for(int i = 0; i < 7; i++)
    {
        weeks_hours += trucker.daily_hours[i];
    }
    return weeks_hours;
}

/*
 * Outputs the data of each trucker in the appropriate format
 */
void output_logistics(TruckList & trucks)
{
    ofstream output("json_solution_output.json");
    if(!output)
    {
        cout << "File did not Open" << endl;
        return;
    }
    // JSON object for storing the information and inserting it into the output file
    json structure;

    float final_cost = 0;

    structure["Final_cost"] = 0;
    for(int i = 0; i < trucks.get_size(); i++)
    {
        // cost for the truck, TRUCKER_BASE accounted for by day_cost
        float truck_cost = 0;
        float weeks_hours = 0;
        // cost for the day, 300 base cost accounted for in daily cost calculation
        float day_cost = TRUCKER_BASE;
        truck* trucker = trucks.select(i);
        // reorder the deliveries list to be ordered according to earliest and latest trips
        trucker->data.deliveries.reverse_list();

        for (int day = 0; day < 7; day++)
        {
            float day_distance = 0;
            structure["Trucker_" + to_string(i + 1)][get_day(day)]["Total_time_driving"] = 0;
            structure["Trucker_" + to_string(i + 1)][get_day(day)]["Total_distance"] = 0;
            structure["Trucker_" + to_string(i + 1)][get_day(day)]["Total_cost"] = 0;
            int delivery_num = 0;
            for(int delivery = 0; delivery < trucker->data.deliveries.get_size(); delivery++)
            {
                logistics trip = trucker->data.deliveries.select(delivery);
                // ensure that only the data for the given day is logged
                if(trip.start_time >= (day) * 24 && trip.end_time <= (day + 1) *  24)
                {
                    delivery_num++;
                    day_distance += dist_array[(trip.start_destination - 1) * CITIES + (trip.end_destination - 1)];
                    structure["Trucker_" + to_string(i + 1)][get_day(day)]["Delivery_" + to_string(delivery_num)] = {
                            {"Start_time",        convert_time(trip.start_time)},
                            {"End_time",          convert_time(trip.end_time)},
                            {"Start_destination", get_location(trip.start_destination)},
                            {"End_destination",   get_location(trip.end_destination)},
                            {"Distance",          dist_array[(trip.start_destination - 1) * CITIES + (trip.end_destination - 1)]},
                            {"Delivery_type",     get_delivery_type(trip.pickup)},
                            {"Start_quantity",    trip.start_quantity},
                            {"End_quantity",      trip.end_quantity}
                    };
                    // check if the trucker worked overtime on the day
                    if (weeks_hours > OVERTIME_LAW)
                    {
                        float overtime_hours = weeks_hours - OVERTIME_LAW;
                        if (overtime_hours <= trucker->data.daily_hours[day])
                        {
                            // if your worked some overtime over the day
                            day_cost += overtime_hours * OVERTIME;
                            day_cost += (trucker->data.daily_hours[day] - overtime_hours) * HOURLY;
                        }
                        else
                        {
                            // if all your hours for the day were overtime
                            day_cost += (overtime_hours - trucker->data.daily_hours[day]) * OVERTIME;
                        }
                    }
                    else
                    {
                        day_cost += trucker->data.daily_hours[day] * HOURLY;
                    }
                    // add cost per km driven
                    day_cost += dist_array[(trip.start_destination - 1) * CITIES + (trip.end_destination - 1)] * COST_KM;
                }
            }
            // log the data for the deliveries on that day
            structure["Trucker_" + to_string(i + 1)][get_day(day)]["Total_cost"] = round(day_cost);
            structure["Trucker_" + to_string(i + 1)][get_day(day)]["Total_distance"] = day_distance;
            structure["Trucker_" + to_string(i + 1)][get_day(day)]["Total_time_driving"] = convert_hours(trucker->data.daily_hours[day]);
            weeks_hours += trucker->data.daily_hours[day];

            // add to weekly cost of the trucker
            truck_cost += day_cost;
            // reset the day cost for the next iteration
            day_cost = 0;
        }
        final_cost += truck_cost;
        /*
        if(weeks_hours > OVERTIME_LAW)
        {
            // add normal hours
            truck_cost += OVERTIME_LAW * HOURLY;
            truck_cost += (weeks_hours - OVERTIME_LAW) * OVERTIME;
        }
        else
        {
            truck_cost += weeks_hours * HOURLY;
        }
         */

    }
    structure["Final_cost"] = round(final_cost);
    output << structure.dump(4);
    output.close();
}
string get_day(unsigned int day)
{
    switch(day)
    {
        case 0:
            return "Monday";
        case 1:
            return "Tuesday";
        case 2:
            return "Wednesday";
        case 3:
            return "Thursday";
        case 4:
            return "Friday";
        case 5:
            return "Saturday";
        case 6:
            return "Sunday";
        default:
            return "INVALID DAY";
    }

}
string get_location(unsigned int city)
{
    switch(city)
    {
        case 1:
            return "Tilbury";
        case 2:
            return "Mississauga";
        case 3:
            return "Cornwall";
        case 4:
            return "London";
        case 5:
            return "Windsor";
        case 6:
            return "Niagara_Falls";
        case 7:
            return "Barrie";
        case 8:
            return "Kingston";
        case 9:
            return "Huntsville";
        case 10:
            return "North_Bay";
        case 11:
            return "Goderich";
        case 12:
            return "Toronto";
        case 13:
            return "Picton";
        default:
            return "Invalid Location";
    }
}
string get_delivery_type(bool pickup)
{
    if(pickup)
    {
        return "Pickup";
    }
    return "Drop-off";
}
string convert_time(float time)
{
    int time_minutes = round(time * 60);
    int hours = int(time);
    int minutes = time_minutes % 60;
    if(time > 24)
    {
        hours = hours - (24 * (hours/24));
    }
    string output;

    // account for zeros
    if(hours < 10)
    {
        output = "0" + to_string(hours);
    }
    else
    {
        output = to_string(hours);
    }

    if(minutes < 10)
    {
        output += ":0" + to_string(minutes);
    }
    else
    {
        output += ":" + to_string(minutes);
    }
    return output;
}
string convert_hours(float time)
{
    int time_minutes = round(time * 60);
    int hours = int(time);
    int minutes = time_minutes % 60;
    string output;
    // account for zeros
    if(hours < 10)
    {
        output = "0" + to_string(hours);
    }
    else
    {
        output = to_string(hours);
    }

    if(minutes < 10)
    {
        output += ":0" + to_string(minutes);
    }
    else
    {
        output += ":" + to_string(minutes);
    }
    return output;
}
