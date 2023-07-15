###DO NOT CHANGE THIS CODE
###WE WILL BE USING THIS CODE TO TEST YOUR SOLUTION
import random
import json

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
sum9 = num1+num2+num3+num4+num5+num6+num7+num8+num9
if (sum9>100):
  num9 = num9-(sum9-100)
  num10 = 0
else:
  num10 = 100-(num1+num2+num3+num4+num5+num6+num7+num8+num9)

deliveries = [num1, num2, num3, num4, num5, num6, num7, num8, num9, num10]
cities = ['Tilbury', 'Mississauga', 'Cornwall', 'London', 'Windsor', 'Niagara_Falls', 'Barrie', 'Kingston', 'Huntsville', 'North_Bay']

# Data to be written
dictionary = {

}

x=0
for i in cities:
  dictionary[i] = deliveries[x]
  x+=1



# Serializing json
json_object = json.dumps(dictionary, indent=4)

# Writing to sample.json
with open("cmake-build-debug/json/input.json", "w") as outfile:
    outfile.write(json_object)
