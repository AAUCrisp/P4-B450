import numpy as np
import time as time
import concurrent.futures

size = 1000
min_value = -100        # Minimum value in each matrix entrance
max_value = 100         # Maximum value in each matrix entrance
max_sum = -np.inf


def AlgorithmMSP(A, i):
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
    tempResult = [None] * 6

    #print(f'Iteration {i} has started')
    # print("i is: " + str(i))      # Troubleshooting Print
    temp = [0] * size      # Creates empty array the size of the Y-axis

    for k in range(i, size):    # From current column to the last
        for j in range(size):      # For every row in the array
            temp[j] += A[j, k]        # Adds the summarised value up till the current index

        pSum = AlgorithmMSP_1D(temp, start, end, size)       # Checks the currently selected row/column combination for it's max value

        if pSum >= maxA:            # If the newly found Max-sum is larger than the currently largest
            maxA = pSum                 # Replace currently largest with the newly found Max-value
            startN = i                # Update the X-axis starting coordinate
            endN = k                  # Update the X-axis ending coordinate
            startM = start[0]       # Update the Y-axis starting coordinate
            endM = end[0]          # Update the Y-axis ending coordinate

            # print("New Starting Point")
    tempResult[0] = maxA
    tempResult[1] = startN
    tempResult[2] = endN
    tempResult[3] = startM
    tempResult[4] = endM
    tempResult[5] = i
    # print(f'Iteration {i} finished')
    return tempResult         # Return the ultimate highest combination sum, and its start and end coordinates


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


if __name__ == '__main__':
    array = np.random.randint(min_value, max_value, (size, size))
    results = []
    # print(results)
    print(f'Array of size {size}x{size}')     # printing dimension
    start_time = time.time()        # Start time, to see computation time

    with concurrent.futures.ProcessPoolExecutor() as executor:
        rest = [executor.submit(AlgorithmMSP, array, i) for i in range(size)]      # Calling the main function

        for f in concurrent.futures.as_completed(rest):
            print(f'{f.result()} | [MAXSUM] [START X] [END X] [START Y] [END Y] [ITERATION] | Finished at time: {time.time()-start_time}')
            results.append(f.result())

    for i in range(size):
        if results[i][0] > max_sum:
            max_sum = results[i][0]
            x_start = results[i][1]
            x_end = results[i][2]
            y_start = results[i][3]
            y_end = results[i][4]

    end_time = time.time()          # End time, to see computation time
    print("Start: X=" + str(x_start) + ":Y=" + str(y_start))        # Print starting coordinates for max value
    print("End: X=" + str(x_end) + ":Y=" + str(y_end))          # Print ending coordinates for max value
    print("Max sum: " + str(max_sum))        # Print max value from currently generated array
    print("Runtime: " + str(end_time - start_time) + " seconds")     # Print out the computation time
