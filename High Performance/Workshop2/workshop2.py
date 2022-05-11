from PIL import Image
import matplotlib.pyplot as plt
import numpy as np


def close_figure(event):
    if event.key == 'escape':
        plt.close(event.canvas.figure)

def show(im, text, imgtype=0):
    fig = plt.figure()
    if (imgtype == 1):
        plt.imshow(im,cmap='gray',vmax = np.max(im)*0.01,vmin = 0)
    else:
        plt.imshow(im, cmap=plt.cm.Greys_r)

    plt.title(text)
    plt.colorbar()
    plt.gcf().canvas.mpl_connect('key_press_event', close_figure)
    plt.show()

if __name__ == '__main__':
    
    f = np.asarray(Image.open("AK.png").convert('L'), dtype='int16')

    pos = 0
    size = 8
    f = f[pos:pos+size,pos:pos+size]
    #f.astype(np.int16)
    f-=128
    show(f, "Test")
    print(f)
    blocksize = 4
    width = int (len(f)/blocksize)
    height = int (len(f[0])/blocksize)
    imageDimensions = [width, height]
    print(imageDimensions)
    blockPartition = np.zeros((imageDimensions[0], imageDimensions[1], blocksize, blocksize))
    print(blockPartition)
    print(len(blockPartition))

    #"""
    for i in range(len(blockPartition)):
        for ii in range(len(blockPartition[0])):
            for iii in range(blocksize):
                for iiii in range(blocksize):
                    blockPartition[i][ii][iii][iiii] = f[iii + (blocksize*i)][iiii + (blocksize*ii)]

    print(blockPartition)               
    #"""
    