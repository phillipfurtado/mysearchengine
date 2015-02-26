import sys
import math
import operator
import re

from os import listdir
from os.path import isfile, join
from PIL import Image

IMAGE_WIDTH = 355
IMAGE_HEIGHT = 355

BORDER_DISCOUNT = 0.11

BLOCK_SIZE = 36
BLOCKS_SIZE = BLOCK_SIZE * BLOCK_SIZE

X_BLOCK_SIZE = IMAGE_WIDTH / BLOCK_SIZE;
Y_BLOCK_SIZE = IMAGE_HEIGHT / BLOCK_SIZE;

NORTH_POINT = IMAGE_HEIGHT * BORDER_DISCOUNT
WEST_POINT = IMAGE_WIDTH * BORDER_DISCOUNT
SOUTH_POINT = IMAGE_HEIGHT - WEST_POINT
EAST_POINT = IMAGE_WIDTH - WEST_POINT

HALF_WIDTH = IMAGE_WIDTH / 2
HALF_HEIGHT = IMAGE_HEIGHT / 2

def getImageRegion(x, y):

    if x >= WEST_POINT and x <= EAST_POINT and y >= NORTH_POINT and y <= SOUTH_POINT:
        return "E"
    
    if x <= HALF_WIDTH and y < HALF_HEIGHT:
        return "A"
    
    if x <= HALF_WIDTH and y >= HALF_HEIGHT:
        return "D"
    
    if x > HALF_WIDTH and y <= HALF_HEIGHT:
        return "B"
    
    return "C"

def convertHistogramIntoWords(histogram):
    
    newHistogram = {}

    #necessary?
    for key, value in enumerate(histogram):
        newHistogram[key] = value

    newHistogram = sorted(newHistogram.items(), key=operator.itemgetter(1), reverse=True)
    len95percent = int(len(newHistogram) * 0.95)

    words = ''
    count = 0
    for key,value in newHistogram:
        if count == len95percent:
            break
        words += 'x' + str(key)
        ++count
    
    return words

def generateSDLCDescriptorSample(image):
    
    lowColorImg = image.convert('P')
    
    yLeft = 0
    yRight = Y_BLOCK_SIZE

    imageDescription = []
    
    for y in range(0, BLOCK_SIZE):
        
        xLeft = 0
        xRight = X_BLOCK_SIZE
        
        for x in range(0, BLOCK_SIZE):
            
            imageBlock = lowColorImg.transform((BLOCK_SIZE, BLOCK_SIZE), Image.EXTENT, (xLeft, yLeft, xRight, yRight))
            imageHistogram = imageBlock.histogram()
            
            blockDescription = getImageRegion(xRight, yRight) + convertHistogramIntoWords(imageHistogram)
                        
            imageDescription.append(blockDescription)

            xLeft += X_BLOCK_SIZE
            xRight += X_BLOCK_SIZE

        yLeft += Y_BLOCK_SIZE
        yRight += Y_BLOCK_SIZE
    
    return ' '.join(imageDescription)

def persistSample(path, sdlc):
    fo = open(path, "wb")
    fo.write(sdlc)
    fo.close()
    
def main(argv):
    
    print "Welcome to SDLC Descriptor Python implementation."
    
    imageCollectionPath = "data/colecaoDafitiPosthaus"
    queriesCollectionPath = "data/queries"
    
    print "Verifying datafiPosthaus and query collections."
       
    imageList = [ f for f in listdir(imageCollectionPath) if isfile(join(imageCollectionPath, f)) ]
    
    queryImageList = [ f for f in listdir(queriesCollectionPath) if isfile(join(queriesCollectionPath, f)) and re.match(r'[0-9]+.*\.jpg', f) ]
    
    print "Running SDLC Descriptor over DafitiPosthaus collection."
    print "This will take a time, make sure to have at least 20GB of free space on disk."

    for imagePath in queryImageList:
        imageFile = queriesCollectionPath + "/" + imagePath;
  
        try:
            im = Image.open(imageFile)
  
            sdlcImage = generateSDLCDescriptorSample(im)
              
            sdlcImagePath = queriesCollectionPath + "/" + imagePath.split(".")[0] + ".sdlc"
  
            persistSample(sdlcImagePath, sdlcImage)
  
        except IOError:
            pass
      
    print "Finished the second collection."
    print "Checkout for files with extersion .sdlc on each collection folder."
    
    for imagePath in imageList:
        imageFile = imageCollectionPath + "/" + imagePath;
        try:
            im = Image.open(imageFile)
               
            sdlcImage = generateSDLCDescriptorSample(im)

            sdlcImagePath = imageCollectionPath + "/" + imagePath.split(".")[0] + ".sdlc"
            persistSample(sdlcImagePath, sdlcImage)
                           
        except IOError:
            pass
       
    print "Finished the first collection."
    print "Running SDLC Descriptor over query collection."
     
if __name__ == '__main__':
    main(sys.argv)
