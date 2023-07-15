###DO NOT CHANGE THIS CODE
###WE WILL BE USING THIS CODE TO TEST YOUR SOLUTION
import random
import json


def day_rand(total_deliveries, day_array):
    mon = random.randrange(0,100)
    tue = random.randrange(0,100)
    wed = random.randrange(0,100)
    thu = random.randrange(0,100)
    fri = random.randrange(0,100)
    sat = random.randrange(0,100)
    sun = random.randrange(0,100)

    sum = mon + tue + wed +thu + fri + sat +sun

    mon = round(total_deliveries*mon/sum)
    tue = round(total_deliveries*tue/sum)
    wed = round(total_deliveries*wed/sum)
    thu = round(total_deliveries*thu/sum)
    fri = round(total_deliveries*fri/sum)
    sat = round(total_deliveries*sat/sum)
    sun = total_deliveries-(mon+tue+wed+thu+fri+sat)

    day_array.append(mon)
    day_array.append(tue)
    day_array.append(wed)
    day_array.append(thu)
    day_array.append(fri)
    day_array.append(sat)
    day_array.append(sun)

def city_rand():
    total_deliveries = 100


    num1 = random.randrange(0,100)
    num2 = random.randrange(0,100)
    num3 = random.randrange(0,100)
    num4 = random.randrange(0,100)
    num5 = random.randrange(0,100)
    num6 = random.randrange(0,100)
    num7 = random.randrange(0,100)
    num8 = random.randrange(0,100)
    num9 = random.randrange(0,100)
    num10 = random.randrange(0,100)

    sum = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10


    num1 = round(num1/sum*total_deliveries)
    num2 = round(num2/sum*total_deliveries)
    num3 = round(num3/sum*total_deliveries)
    num4 = round(num4/sum*total_deliveries)
    num5 = round(num5/sum*total_deliveries)
    num6 = round(num6/sum*total_deliveries)
    num7 = round(num7/sum*total_deliveries)
    num8 = round(num8/sum*total_deliveries)
    num9 = round(num9/sum*total_deliveries)
    num10 = 100-(num1+num2+num3+num4+num5+num6+num7+num8+num9)

    deliveries = [num1, num2, num3, num4, num5, num6, num7, num8, num9, num10]
    cities =['Tilbury', 'Mississauga', 'Cornwall', 'London', 'Windsor', 'Niagara_Falls', 'Barrie', 'Kingston', 'Huntsville', 'North_Bay']

# Data to be written
    dictionary = {

    }

    x=0
    for i in cities:
        day_array = []
        day_rand(deliveries[x], day_array)
        dictionary[i] = {}

        dictionary[i]["Total"] = deliveries[x]
        dictionary[i]["Monday"] = day_array[0]
        dictionary[i]["Tuesday"] = day_array[1]
        dictionary[i]["Wednesday"] = day_array[2]
        dictionary[i]["Thursday"] = day_array[3]
        dictionary[i]["Friday"] = day_array[4]
        dictionary[i]["Saturday"] = day_array[5]
        dictionary[i]["Sunday"] = day_array[6]
        x+=1


    # Serializing json
        json_object = json.dumps(dictionary, indent=4)

        # Writing to sample.json
        with open("cmake-build-debug/json/input.json", "w") as outfile:
            outfile.write(json_object)

city_rand()




