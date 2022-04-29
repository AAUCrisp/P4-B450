import numpy as np
import time as time

x_steps = [5, 10, 100]          # Array N-size
y_steps = [5, 10, 100]          # Array M-size
iterations = 10         # The amount of times the whole thing runs
min_value = -100          # Minimum value in each matrix entrance
max_value = 100           # Maximum value in each matrix entrance



def AlgorithmMSP_Sequential(A: list[int], N: int, M: int):
    """
    MSP Sequential
    =====
    Function to find the max-value combination of an twodimentional array

    Parameter documentation
    ----
    >>> A = array, `Two-dimentional array`
    >>> N = int, `X-axis size of array`
    >>> M = int, `Y-axis size of array`

    `Returns maximum possible value-sum from a section of the array & the selection coordinates`
    >>> Max Sum = int
    >>> X-axis start position = int
    >>> X-axis end position = int
    >>> Y-axis start position = int
    >>> Y-axis end position = int
    """
    maxA = -np.inf      # Setup a variable to keep currently found max value.
    startN = 0          # Setup a variable to keep track of and return X-axis starting point of result
    endN = 0            # Setup a variable to keep track of and return X-axis end point of result
    startM = 0          # Setup a variable to keep track of and return Y-axis starting point of result
    endM = 0            # Setup a variable to keep track of and return Y-axis end point of result

    for i in range(N):      # For each column in the array
        """
        This loop is the main loop, looping over each column in the matrix.
        Used for moving the starting column for each iteration

        This loop concurrently removes the "earlier" column, to check for better combinations further into the matrix.
        I.E. 1st loop it takes from 1 column to the end, 2nd loop it's from second column, and so forth...
        """
        # print("I is: " + str(i))      # Troubleshooting Print
        temp = np.zeros([M,1])      # Creates empty array the size of the Y-axis

        for k in range(i,N):    # From current column to the last
            """
            This loop adds another column to the compared combination for each iteration, from the current starting column (considering i)
            """
            # print("K is: " + str(k))      # Troubleshooting Print

            for j in range(M):      # For every row in the array
                """
                This loop adds each row of the current column, to the combined sum of earlier combined columns...
                First iteration in temp will be on the 1st column, Second iteration is 1st to 2nd column combined, Third iteration 1st to 3rd column, and so forth.
                """
                temp[j] = temp[j]+A[j,k]        # Adds the summarised value up till the current index 
                # print(temp[j])

            pSum, temp_start, temp_end = AlgorithmMSP_1D(temp, M)       # Checks the currently selected row/column combination for it's max value
            if pSum >= maxA:            # If the newly found Max-sum is larger than the currently largest
                maxA = pSum                 # Replace currently largest with the newly found Max-value
                startN = i+1                # Update the X-axis starting coordinate
                endN = k+1                  # Update the X-axis ending coordinate
                startM = temp_start+1       # Update the Y-axis starting coordinate
                endM = temp_end+1          # Update the Y-axis ending coordinate

        # print("New Starting Point")
    return maxA, startN, endN, startM, endM         # Return the ultimate highest combination sum, and it's start and end coordinates




def AlgorithmMSP_1D(a: list[int], m: int):
    """
    MSP Sequential One-dimantional Assist
    =====
    Assist to MSP Sequential to search through Y-axis

    Parameter documentation
    ----
    >>> a = array, `Two-dimentional array`
    >>> m = int, `Y-axis size of array`

    `Returns max sum, and Y-axis coordinates`

    >>> Max Sum = int
    >>> Y-axis start position = int
    >>> Y-axis end position = int
    """
    min_pSum = 0        # Sets a starting value for the comparison
    pSum = [a[0]]       # Creates and adds the first row to the comparison variable
    maxA = -np.inf      # Apparently you can just put a minus in front of an object-call.... weird ass Python
    start = 0           # Variable to set and return the starting point of the optimal combination
    temp_start = 0      # Variable to be able to work with and find the starting point
    end = 0             # Variable to set and return the starting point of the optimal combination

    # print(a)        # Troubleshooting Print
    # print("Række: 1 - " + str(pSum[0]))       # Troubleshooting Print
    for i in range(1,m):        # For each row in the given array, except the first
        """
        This loop adds the current row, to the combined sum of earlier combined rows...
        First entry i pSum will be on the 1st row, Second entry is 1st to 2nd row combined, Third entry 1st to 3rd row, and so forth.
        """
        pSum.append(pSum[i-1]+a[i])     # Add the sum of another row to the array
        # print("Række: " + str(i+1) + " - " + str(pSum[i]))
    
    # print("pSum: " + str(pSum))         # Troubleshooting Print


    for i in range(0,m):        # For each column combination
        """
        Should kinda loop over pSum, for clearance, but it's a bit more complicated with arrays.

        If the sum of the current (combination, minus the smallest sum seen) is greater the current larges sum seen, the current largest sum seen will be updated with the current column combination.
        The smalles sum is included, to check if it makes sence to start an entirely new combination instead of continuing.
        When this happens it also saves the start and end point of the Y-axis for the combination.

        Likewise, when the current combination is actually smaller than the current smallest sum seen, it will update that variable too.
        """
        # maxA = max(maxA, pSum[i] - min_pSum)      # Old method, doesn't leave us options to return coordinates.
        
        # print("pSum is: " + str(pSum[i]))             # Troubleshooting Print
        # print("Minimum Sum is: " + str(min_pSum))     # Troubleshooting Print

        if(maxA < pSum[i] - min_pSum):      # If current combination is higher than current set Max-combination
            maxA = pSum[i] - min_pSum           # Replace the Max with the newly found Max
            end = i                             # Save the current end coordinate for returning
            start = temp_start                  # Set the starting coordinate to last "seen" starting point
        

        # min_pSum = min(min_pSum, pSum[i])         # Old method, doesn't leave us options to return coordinates.

        if(min_pSum >= pSum[i]):            # If current combination is lower than the last "seen" minimum
            min_pSum = pSum[i]                  # Replace the Min with the newly found Min
            temp_start = i+1                    # Set a new starting point, to use in case it eventually finds a new Max

    # print(maxA)
    return maxA, start, end     # Return the highest combination sum, and it's start and end Y-coordinate



"""-----------------------------
   ---  CODE RUNNING AREA ---
-----------------------------"""
average_runtime = []
for i in range(len(x_steps)):       # For each matrix size/step option
    combined_start_time = time.time()       # Start timer for the combined runtime

    for j in range(iterations):     # Run each step of matrix-size for the set amount of iterations
        
        array = np.random.randint(min_value, max_value + 1, size = (x_steps[i], y_steps[i]))      # Creating our two-dimentional array to search through

        start_time = time.time()        # Start time, to see computation time
        print(array)                    # Print given array for troubleshooting purposes

        max_sum, x_start, x_end, y_start, y_end = AlgorithmMSP_Sequential(array, x_steps[i], y_steps[i])      # Calling the main function

        print("Start: X=" + str(x_start) + ":Y=" + str(y_start))        # Print starting coordinates for max value
        print("End: X=" + str(x_end) + ":Y=" + str(y_end))          # Print ending coordinates for max value
        print("Max sum: " + str(max_sum[0]))        # Print max value from currently generated array
        end_time = time.time()          # End time, to see computation time

        print("Runtime: " + str(end_time - start_time) + " seconds")     # Print out the computation time

    combined_end_time = time.time()         # Stop timer for the combined runtime for current matrix size
    # print("Step is: " + str(i+1))         # For troubleshooting purposes
    average_runtime.append((combined_end_time - combined_start_time)/iterations)
    print("\nAverage Runtime for a " + str(x_steps[i]) + "x" + str(y_steps[i]) + " Matrix:" + str(average_runtime[i]) + " seconds \n_______________________________")     # Print out the averate computation time for current matrix size

print("\n")
for i in range(len(x_steps)):       # Print out all matrix size averages for comparison
    print("Average Runtime for a " + str(x_steps[i]) + "x" + str(y_steps[i]) + " Matrix: " + str(average_runtime[i]) + " seconds")     # Print out the averate computation time
