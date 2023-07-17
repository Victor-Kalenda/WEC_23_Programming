/*
 * Created by Victor Kalenda on 2023-04-03
 *
 * Example solution to 2023 WEC Programming Question
 *
 * Difficulty level: 4
 *
 * Remark: the Distances.csv file does not include the warehouses, therefore, Mississauga was assumed to be a warehouse
 * as well as the headquarters for all trucks.
 *
 */


#include <iostream>
#include <fstream>
#include "Queue.h"
#include "TruckList.h"
#include "nlohmann/json.hpp"

// for convenience
using json = nlohmann::ordered_json;

using namespace std;

void analyse_travel();
int get_number(istream & input_file);
void ignore_cells(int count, istream & input_file);
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
double to_2_dec(float num);

#define HOURLY               20
#define OVERTIME             float(30.0)
#define OVERTIME_LAW         float(60.0)
#define TRUCKER_BASE         float(300.0)
#define LATE_FEE             200
#define COST_KM              float(0.68)
#define SPEED                float(100.0)
#define TRAFFIC_FACTOR       0.7 // Not yet implemented
#define DAILY_WORK_LAW       float(14.0)
#define WEEKLY_WORK_LAW      float(70.0)
#define CITIES               10
#define MISSISSAUGA          2
#define TRUCK_CAPACITY       5
#define START_TIME           float(6.0)

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
            if(i == j)
            {
                // distances between the same city
                dist_array[i * CITIES + j] = 0;
                ignore_cells(1, distances);
            }
            else
            {
                dist_array[i * CITIES + j] = float(get_number(distances));
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
        for(int j = 0; j < demand; j++)
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
    return 0;
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
            package.start_time = float(trucker.current_day - 1) * 24 + START_TIME;
        }
        else
        {
            package.start_time = trucker.deliveries.select(0).end_time;
        }
    }
    else
    {
        package.start_time = float(trucker.current_day - 1) * 24 + START_TIME;
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
            int travel_distance = dist_array[(packages.get_destination(0) - 1) * CITIES + (i - 1)];
            if(travel_distance < shortest_path)
            {
                shortest_path = travel_distance;
                target_warehouse = i;
            }
        }
         */
        package.end_destination = target_warehouse;
        package.end_time = package.start_time + dist_array[(target_warehouse - 1) * CITIES + (package.start_destination - 1)] / SPEED;
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
    ofstream output("json/csv_solution_output.json");
    if(!output)
    {
        cout << "File did not Open" << endl;
        return;
    }
    // JSON object for storing the information and inserting it into the output file
    json structure;

    float final_cost = 0;
    cout << "Number of Trucks: " << trucks.get_size() << endl;
    cout << "Number of Late Deliveries: " << trucks.late_deliveries << endl;

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
                if(trip.start_time >= float(day) * 24 && trip.end_time <= float(day + 1) *  24)
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

                }
            }
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
            day_cost += day_distance * COST_KM;
            // log the data for the deliveries on that day
            structure["Trucker_" + to_string(i + 1)][get_day(day)]["Total_cost"] = to_2_dec(day_cost);
            structure["Trucker_" + to_string(i + 1)][get_day(day)]["Total_distance"] = day_distance;
            structure["Trucker_" + to_string(i + 1)][get_day(day)]["Total_time_driving"] = convert_hours(trucker->data.daily_hours[day]);
            weeks_hours += trucker->data.daily_hours[day];

            // add to weekly cost of the trucker
            truck_cost += day_cost;
            // reset the day cost for the next iteration
            day_cost = 0;
        }
        final_cost += truck_cost;
        // output individual truck information
        cout << "Truck #" << i + 1 << " Cost = " << truck_cost << endl;
        float total_hours = 0;
        cout << "Daily Hours" << endl;
        for(int day = 0; day < 7; day++)
        {
            cout << get_day(day) << ": " << setw(6) << to_2_dec(trucker->data.daily_hours[day]) << "     ";
            total_hours += trucker->data.daily_hours[day];
        }
        cout << endl << "Total Hours Driven: " << total_hours << endl << endl;


    }
    final_cost += float(trucks.late_deliveries) * LATE_FEE;
    cout << "Final Cost: " << to_2_dec(final_cost) << endl;
    structure["Final_cost"] = to_2_dec(final_cost);
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
    int time_minutes = int(round(time * 60));
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
    int time_minutes = int(round(time * 60));
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
double to_2_dec(float num)
{
    int i = int(round(num * 100));
    return (i / 100.0);
}