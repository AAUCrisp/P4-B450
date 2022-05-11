from PIL import Image
import matplotlib.pyplot as plt
import numpy as np


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


if __name__ == '__main__':
    
    f = np.asarray(Image.open("AK.png").convert('L'), dtype='int16')        # Opens the image as an array with separate pixels, only with the lumen value, typecasted to int for processing

    pos = 0  # Starting position of conversion
    size = 256 # How many pixels (from start) to include
    f = f[pos:pos+size,pos:pos+size]
    #f.astype(np.int16)
    f-=128 # Substranct 128 from all the values, to make them center around 0
    show(f, "Test") # Start GUI and show the result
    #print(f)
    blocksize = 8
    width = int (len(f))
    height = int (len(f[0]))
    imageDimensions = [width, height]
    #print(imageDimensions)
    blockPartition = np.zeros((int(imageDimensions[0]/blocksize), int(imageDimensions[1]/blocksize), blocksize, blocksize))

    # Partitions the image into blocks of size 8x8
    for i in range(len(blockPartition)):
        for ii in range(len(blockPartition[0])):
            for iii in range(blocksize):
                for iiii in range(blocksize):
                    blockPartition[i][ii][iii][iiii] = f[iii + (blocksize*i)][iiii + (blocksize*ii)]

    #print(blockPartition)               
    
    blockPartitionDCT = np.zeros((int(imageDimensions[0]/blocksize), int(imageDimensions[1]/blocksize), blocksize, blocksize))
    
    for i in range(len(blockPartitionDCT)):
        for ii in range(len(blockPartitionDCT[0])):
            for iii in range(blocksize):
                for iiii in range(blocksize):
                    tempOuter = 0
                    for n1 in range(blocksize):
                        tempInner = 0
                        for n2 in range(blocksize):
                            tempInner += blockPartition[i][ii][n1][n2] * np.cos((np.pi/blocksize)*(n2+1/2)*iiii)
                        tempOuter += tempInner * np.cos((np.pi/blocksize)*(n1+1/2)*iii)
                    blockPartitionDCT[i][ii][iii][iiii] = int(tempOuter)

    
    fDCT = np.zeros((imageDimensions[0], imageDimensions[1]))
    
    print(fDCT)

    
    for i in range(len(blockPartitionDCT)):
        print(i)
        for ii in range(len(blockPartitionDCT[0])):
            print(f'i = {i} : ii = {ii}')
            for iii in range(blocksize):
                print(f'i = {i} : ii = {ii} : iii = {iii}')
                for iiii in range(blocksize):
                    print(f'i = {i} : ii = {ii} : iii = {iii} : iiii = {iiii}')
                    fDCT[iii + (blocksize*i)][iiii + (blocksize*ii)] = blockPartitionDCT[i][ii][iii][iiii]
    print(fDCT)
    show((fDCT),"test")