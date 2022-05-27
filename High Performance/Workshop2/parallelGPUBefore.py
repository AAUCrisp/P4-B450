from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
from numba import njit, prange, cuda
from time import perf_counter
import math as math
#import os
#os.environ['NUMBA_PARALLEL_DIAGNOSTICS'] = '1'

def close_figure(event):
    """
    Function to shut down the GUI
    =====
    Parameter documentation
    ----
    >>> event = something, `The event set to activate the shutdown`
    """
    if event.key == 'escape':
        plt.close(event.canvas.figure)


def show(im, text, imgtype=0):
    """
    Show Result in GUI
    =====
    Opens the processed picture in a GUI within a plot with info.

    Parameter documentation
    ----
    >>> im = array, `Array containing an image, pixel by pixel`
    >>> text = string, `Name of the plot`
    >>> imgtype = int, `type=1: For Discrete Cosine Transform & Quantization Images, need special setting for colors`
    """
    fig = plt.figure()
    if (imgtype == 1):      # If DCT or Quantization Image
        plt.imshow(im,cmap='gray',vmax = np.max(im)*0.01,vmin = 0)
    else:       # Other picture types
        plt.imshow(im, cmap=plt.cm.Greys_r)

    plt.title(text)     # Add title to the plot
    plt.colorbar()      # Add the color-comparison bar
    plt.gcf().canvas.mpl_connect('key_press_event', close_figure)       # Tell the GUI what to do when closed
    plt.show()      # Show the GUI


@cuda.jit
def quantizise_matrixGen(quantiziseMatrix):
    tx = cuda.threadIdx.x
    ty = cuda.threadIdx.y
    
    quantiziseMatrix[tx][ty] = 3 + (2*tx) + (2*ty)


#Turn image into block partitions
@njit(parallel=True)
def blockPartitioning(f, blockPartition, blocksize):
    for i in prange(len(blockPartition)):
        for ii in range(len(blockPartition[0])):
            for iii in range(blocksize):
                for iiii in range(blocksize):
                    blockPartition[i][ii][iii][iiii] = f[iii + (blocksize*i)][iiii + (blocksize*ii)]

@cuda.jit
def blockPartitioningDCT(blockPartitionDCT, blockPartition):
    #x, y = cuda.grid(2)

    tx = cuda.threadIdx.x
    ty = cuda.threadIdx.y
    bx = cuda.blockIdx.x
    by = cuda.blockIdx.y
    bd = cuda.blockDim.x

    for n1 in range(bd):
        tempInner = 0
        for n2 in range(bd):
            tempInner += blockPartition[bx][by][n1][n2] * math.cos((math.pi*((2*n1)+1)*tx)/(2*bd)) * math.cos((math.pi*((2*n2)+1)*ty)/(2*bd))
        blockPartitionDCT[bx][by][tx][ty] += tempInner
        
    if tx == 0:
        if ty == 0:
            blockPartitionDCT[bx][by][tx][ty] = blockPartitionDCT[bx][by][tx][ty] * (1/math.sqrt(bd)) * (1/math.sqrt(bd))
        else:
            blockPartitionDCT[bx][by][tx][ty] = blockPartitionDCT[bx][by][tx][ty] * (1/math.sqrt(bd)) * math.sqrt(2/bd)
    else:
        if ty == 0:
            blockPartitionDCT[bx][by][tx][ty] = blockPartitionDCT[bx][by][tx][ty] * (1/math.sqrt(bd)) * math.sqrt(2/bd)
        else:
            blockPartitionDCT[bx][by][tx][ty] = blockPartitionDCT[bx][by][tx][ty] * math.sqrt(2/bd) * math.sqrt(2/bd)

    

@njit(parallel=True)
def quantizise(blockPartitionQuant, blockPartitionDCT, blocksize, quantization_matrix):  
    for i in prange(len(blockPartitionQuant)):
        for ii in range(len(blockPartitionQuant[0])):
            for iii in range(blocksize):
                for iiii in range(blocksize):
                    blockPartitionQuant[i][ii][iii][iiii] = np.rint(blockPartitionDCT[i][ii][iii][iiii] / quantization_matrix[iii][iiii])

@njit(parallel=True)
def DCT3_Coef(blockPartitionQuant, blockPartitionDCT3, blocksize, quantization_matrix):
    for i in prange(len(blockPartitionDCT3)):
        for ii in range(len(blockPartitionDCT3[0])):
            for iii in range(blocksize):
                for iiii in range(blocksize):
                    blockPartitionDCT3[i][ii][iii][iiii] = blockPartitionQuant[i][ii][iii][iiii] * quantization_matrix[iii][iiii]



@cuda.jit
def restoreImage(blockPartitionDCT3, blockPartitionRestored):
    tx = cuda.threadIdx.x
    ty = cuda.threadIdx.y
    bx = cuda.blockIdx.x
    by = cuda.blockIdx.y
    bd = cuda.blockDim.x
    

    for n1 in range(bd):
        tempInner = 0
        for n2 in range(bd):
            if n1 == 0:
                if n2 == 0:
                   tempInner += (1/math.sqrt(bd)) * (1/math.sqrt(bd)) * blockPartitionDCT3[bx][by][n1][n2] * math.cos((math.pi*((2*tx)+1)*n1)/(2*bd)) * math.cos((math.pi*((2*ty)+1)*n2)/(2*bd))
                else:
                   tempInner += (1/math.sqrt(bd)) * math.sqrt(2/bd) * blockPartitionDCT3[bx][by][n1][n2] * math.cos((math.pi*((2*tx)+1)*n1)/(2*bd)) * math.cos((math.pi*((2*ty)+1)*n2)/(2*bd))
            else:
                if n2 == 0:
                    tempInner += (1/math.sqrt(bd)) * math.sqrt(2/bd) * blockPartitionDCT3[bx][by][n1][n2] * math.cos((math.pi*((2*tx)+1)*n1)/(2*bd)) * math.cos((math.pi*((2*ty)+1)*n2)/(2*bd))
                else:
                    tempInner += math.sqrt(2/bd) * math.sqrt(2/bd) * blockPartitionDCT3[bx][by][n1][n2] * math.cos((math.pi*((2*tx)+1)*n1)/(2*bd)) * math.cos((math.pi*((2*ty)+1)*n2)/(2*bd))

        blockPartitionRestored[bx][by][tx][ty] += tempInner


# Turn partition into image array
@njit(parallel=True)
def partitionToImageArray(partition, retImage, blocksize):

    for i in prange(len(partition)):
        for ii in range(len(partition[0])):
            for iii in range(blocksize):
                for iiii in range(blocksize):
                    retImage[iii + (blocksize*i)][iiii + (blocksize*ii)] = partition[i][ii][iii][iiii]


def printBlock(block, blocksize):
    if blocksize > 8:
        return

    column = ''
    divider = ''
    for x in range(blocksize):
        if x == 0:
            column = '              {:<11d}'.format(x)
            divider = '     -----------'
        else:
            column = column+'{:<11d}'.format(x)
            divider = divider+'-----------'
    print(column)
    print(divider)

    for x in range(len(block)):
        row = ''
        for y in range(len(block[x])):
            if y == 0:
                row = row+'{:4d} | {:8.2f}|  '.format(x, block[x][y])
            else:
                row = row+'{:8.2f}|  '.format(block[x][y])
        print(row)
    print("")


if __name__ == '__main__':

    tempStart = perf_counter()
    f = np.asarray(Image.open("test.png").convert('L'), dtype='int16')        # Opens the image as an array with separate pixels, only with the lumen value, typecasted to int for processing
    tempStop = perf_counter()
    timeGreyscale = tempStop - tempStart
    print(f'\n\n\nGreyscale conversion took time: {timeGreyscale} seconds')
    
    blocksize = 8
    """
    pos = 0  # Starting position of conversion
    size = 256 # How many pixels (from start) to include
    f = f[pos:pos+size,pos:pos+size]
    """

    


    

    f-=128 # Substranct 128 from all the values, to make them center around 0
    show(f, "Default image") # Start GUI and show the result
    #print(f)
    width = int (len(f))
    height = int (len(f[0]))
    imageDimensions = [width, height]
    #print(imageDimensions)

    
    


    tempStart = perf_counter()
    # Turn image array into blockPartion
    blockPartition = np.zeros((int(imageDimensions[0]/blocksize), int(imageDimensions[1]/blocksize), blocksize, blocksize))
    blockPartitioning(f, blockPartition, blocksize)

    #Print first block of the block matrix
    print("\nUntouched:")
    printBlock(blockPartition[0][0], blocksize)
    tempStop = perf_counter()
    timePartitioning = tempStop - tempStart
    print(f'\nBlock partitioning took time: {timePartitioning} seconds')

    
    tempStart = perf_counter()
    # Use Discrete Cosine Transform on blockPartion
    blockPartitionDCT = np.zeros((int(imageDimensions[0]/blocksize), int(imageDimensions[1]/blocksize), blocksize, blocksize))
    #######################################
    ########### GPU SETTINGS ###############
    ########################################
    blockdim = (blocksize,blocksize)
    griddim = (blockPartitionDCT.shape[0], blockPartitionDCT.shape[1])
    print(griddim)
    


    ########################################
    ########################################
    ########################################    
    blockPartitioningDCT[griddim, blockdim](blockPartitionDCT, blockPartition)

    # Create image array for DCT version
    fDCT = np.zeros((imageDimensions[0], imageDimensions[1]))
    partitionToImageArray(blockPartitionDCT, fDCT, blocksize)

    #Print first block of the DCT block matrix
    print("\nDCT Version:")
    printBlock(blockPartitionDCT[0][0], blocksize)
    tempStop = perf_counter()
    timeDCT = tempStop - tempStart
    print(f'\nDiscrete cosine transform took time: {timeDCT} seconds')
    # Show DCT coefficiense version of image
    show(fDCT, "DCT", 1)


    tempStart = perf_counter()
    # Create quantization_matrix
    #temp_quantization_matrix = [[16, 11, 10, 16, 24, 40, 51, 61], [12, 12, 14, 19, 26, 58, 60, 55], [14, 13, 16, 24, 40, 57, 69, 56], [14, 17, 22, 29, 51, 87, 80, 62], [18, 22, 37, 56, 68, 109, 103, 77],[24, 35, 55, 64, 81, 104, 113, 92], [49, 64, 78, 87, 103, 121, 120, 101], [79, 92, 95, 98, 112, 100, 103, 99]]
    quantization_matrix = np.zeros((blocksize, blocksize))
    #for i in range(blocksize):
        #for ii in range(blocksize):
            #quantization_matrix[i][ii] = temp_quantization_matrix[i][ii]
    quantizise_matrixGen[[1,1], blockdim](quantization_matrix)
    print("\nQuantization Matrix:")
    printBlock(quantization_matrix, blocksize)
    tempStop = perf_counter()
    timeQuantGen = tempStop - tempStart
    print(f'\nQuantMatrixGen took time: {timeQuantGen} seconds')


    tempStart = perf_counter()
    # Use quantazise on the DCT block matrix
    blockPartitionQuant = np.zeros((int(imageDimensions[0]/blocksize), int(imageDimensions[1]/blocksize), blocksize, blocksize))
    quantizise(blockPartitionQuant, blockPartitionDCT, blocksize, quantization_matrix)

    # Make printable version of the quantizised version
    fQuant = np.zeros((imageDimensions[0], imageDimensions[1]))
    partitionToImageArray(blockPartitionQuant, fQuant, blocksize)

    # Print first block of quantazised DCT block matrix
    print("\nQuant:")
    printBlock(blockPartitionQuant[0][0], blocksize)
    tempStop = perf_counter()
    timeQuant = tempStop - tempStart
    print(f'\nQuantization took time: {timeQuant} seconds')

    # Show the quantizised version
    show(fQuant, "Quantization", 1)



    tempStart = perf_counter()
    # Make DCT3 Coefficiense block matrix
    blockPartitionDCT3 = np.zeros((int(imageDimensions[0]/blocksize), int(imageDimensions[1]/blocksize), blocksize, blocksize))
    DCT3_Coef(blockPartitionQuant, blockPartitionDCT3, blocksize, quantization_matrix)

    print("\nDeQuantizised Version:")
    printBlock(blockPartitionDCT3[0][0], blocksize)
    tempStop = perf_counter()
    timeDeQuant = tempStop - tempStart
    print(f'\nDequantization took time: {timeDeQuant} seconds')


    tempStart = perf_counter()
    # Restore image
    blockPartitionRestored = np.zeros((int(imageDimensions[0]/blocksize), int(imageDimensions[1]/blocksize), blocksize, blocksize))
    restoreImage[griddim, blockdim](blockPartitionDCT3, blockPartitionRestored)
    

    # Make printable version of restored
    fRestored = np.zeros((imageDimensions[0], imageDimensions[1]))
    partitionToImageArray(blockPartitionRestored, fRestored, blocksize)

    #fRestored+=128
    #Print first block of the block matrix
    print("\nRestored:")
    printBlock(blockPartitionRestored[0][0], blocksize)
    tempStop = perf_counter()
    timeRestore = tempStop - tempStart
    print(f'\nRestoration took time: {timeRestore} seconds')

    print(f'\n\nOverall took time: {timeGreyscale + timePartitioning + timeDCT + timeQuant + timeDeQuant + timeRestore} seconds')

    # Show restores image?
    show(fRestored, "Restored?")
