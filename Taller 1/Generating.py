import sys
import random

def restriction(val, min_val, max_val):
    return max(min(val, max_val), min_val)

def generate_color(mean, std_dev):
    color = random.gauss(mean,std_dev)
    return restriction(int(color), 0, 255)
def fill_random_matrix(my_array,width,height,mean, std_dev):
    for i in range(height):
        for j in range(width):
            my_array[i][j] =  (generate_color(mean, std_dev), generate_color(mean, std_dev), generate_color(mean, std_dev))
    return my_array

filename = sys.argv[1]
width = int(sys.argv[2])
height = int(sys.argv[3])
percentages = [float(arg) for arg in sys.argv[4:]]
final = [[0]*width for _ in range(height)]
with open(filename, 'w') as f:
    f.write("P3\n{} {}\n255\n".format(width, height))
    mean = 255 / 2
    std_dev = 100
    final = fill_random_matrix(final,width,height,mean,std_dev)
    cumulative_percentage = 1.0
    for percentage in percentages:
        cumulative_percentage *= percentage
        w = int(width * cumulative_percentage)
        h = int(height * cumulative_percentage)
        std_dev = int(100 * cumulative_percentage)
        final = fill_random_matrix(final,w,h,mean,std_dev)
    #now we write our matrix
    for i in range(height):
        for j in range(width):
            R,G,B = final[i][j]
            f.write("{} {} {}\t".format(R,G,B))
        f.write("\n")