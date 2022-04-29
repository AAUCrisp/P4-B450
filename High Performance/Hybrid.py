from termios import N_STRIP
import numpy as np
import time as time

size = 1000
min_value = -100        # Minimum value in each matrix entrance
max_value = 100         # Maximum value in each matrix entrance

def matrix_Generator(N):      
    #print("The dimension : " + str(N))     # printing dimension
      
    res = [list(np.random.randint(min_value,max_value+1,N)) for i in range(N)] # using list comprehension - matrix creation of n * n
      
    #print("The created matrix of N * N: \n")     # print result
    column = ''
    divider = ''
    for x in range(N):
        if x == 0:
            column = '       {:4d}'.format(x)
            divider = '     ------'
        else:
            column = column+'{:4d}'.format(x)
            divider = divider+'----'
    #print(column)
    #print(divider)

    for x in range(len(res)):
        row = ''
        for y in range(len(res[x])):
            if y == 0:
                row = row+'{:4d} | {:4d}'.format(x, res[x][y])
            else:
                row = row+'{:4d}'.format(res[x][y])
        #print(row)
    #print("")
    return res


def AlgorithmMSP_Sequential(A):
    global size
    """
    MSP Sequential
    Function to find the max-value combination of an twodimentional array

    Parameter documentation
    """
    maxA = -np.inf      # Setup a variable to keep currently found max value.
    startN, endN, startM, endM = None, None, None, None          # Setup a variable to keep track of and return X-axis starting point of result
    pSum = 0
    start = [0]
    end = [0]
    temp = [None] * size


    for i in range(size):      # For each column in the array
        print("I is: " + str(i))      # Troubleshooting Print
        temp = [0] * size      # Creates empty array the size of the Y-axis

        for k in range(i,size):    # From current column to the last
            for j in range(size):      # For every row in the array
                temp[j] += A[j][k]        # Adds the summarised value up till the current index

            pSum = AlgorithmMSP_1D(temp, start, end, size)       # Checks the currently selected row/column combination for it's max value
            
            if pSum >= maxA:            # If the newly found Max-sum is larger than the currently largest
                maxA = pSum                 # Replace currently largest with the newly found Max-value
                startN = i                # Update the X-axis starting coordinate
                endN = k                  # Update the X-axis ending coordinate
                startM = start[0]       # Update the Y-axis starting coordinate
                endM = end[0]          # Update the Y-axis ending coordinate

        #print("New Starting Point")
    return maxA, startN, endN, startM, endM         # Return the ultimate highest combination sum, and it's start and end coordinates




def AlgorithmMSP_1D(a, start, end, m):
    """
    MSP Sequential One-dimantional Assist
    =====
    Assist to MSP Sequential to search through Y-axis

    Parameter documentation
    """
    pSum = 0     # Creates and adds the first row to the comparison variable
    maxA = -np.inf      # Apparently you can just put a minus in front of an object-call.... weird ass Python
    temp_start = 0      # Variable to be able to work with and find the starting point
    i = None
    # print(a)        # Troubleshooting Print
    # print("Række: 1 - " + str(pSum[0]))       # Troubleshooting Print
    for i in range(m):        # For each row in the given array, except the first
        """
        This loop adds the current row, to the combined sum of earlier combined rows...
        First entry i pSum will be on the 1st row, Second entry is 1st to 2nd row combined, Third entry 1st to 3rd row, and so forth.
        """
        pSum += a[i]

        if pSum < 0:           
            pSum = 0            
            temp_start = i+1

        elif pSum > maxA:
            maxA = pSum
            end[0] = i
            start[0] = temp_start

    # print("pSum: " + str(pSum))         # Troubleshooting Print
    # print(maxA)
    return maxA     # Return the highest combination sum, and it's start and end Y-coordinate



if __name__=='__main__':
    array = matrix_Generator(size)
    start_time = time.time()        # Start time, to see computation time
    max_sum, x_start, x_end, y_start, y_end = AlgorithmMSP_Sequential(array)      # Calling the main function
    end_time = time.time()          # End time, to see computation time
    print("Start: X=" + str(x_start) + ":Y=" + str(y_start))        # Print starting coordinates for max value
    print("End: X=" + str(x_end) + ":Y=" + str(y_end))          # Print ending coordinates for max value
    print("Max sum: " + str(max_sum))        # Print max value from currently generated array

    print("Runtime: " + str(end_time - start_time) + " ms")     # Print out the computation time
