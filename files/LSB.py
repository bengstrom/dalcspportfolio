'''Lip Sync Battle (LSB) stylizer
    by Brian Engstrom
    
    Take images in RGBA format with transparent background and shift
    colored pixels.
'''
from PIL import Image
import os.path

#Settings
filename = "image.png" #Insert Image name here
offset = 100 # Set spacing between shifted images

#Set Directory and load File
directory = os.path.dirname(os.path.abspath(__file__))
filepath = os.path.join(directory, filename)
im = Image.open(filepath)
pix = im.load()

#Get image width and hieght
width,height = im.size

#Debug
#print im.mode
#print "W: "+str(width)
#print "H: "+str(height)

#Iterate through each pixel checking if not transparent
for column in range(0,height):
    for row in range(0,width):
        if im.getpixel((row,column))[3] >= 50:
            try:
                #If not transparent then generate shifted image
                if (row-offset >= 0) and (im.getpixel((row-offset,column))[3] <= 50):
                    
                    #If second shift is still within image, color pixel
                    if (row-2*offset>0)and (im.getpixel((row-2*offset,column))[3] != 255):
                        try:
                            pix[row-2*offset,column]= (12,216,247,255)
                        except IndexError:
                            print "2*offset error at row "+str(row)+" and column "+str(column)
                            continue
                    #Draw first shift
                    try:
                        pix[row-offset,column]= (138,12,237,255)
                    except IndexError:
                        print "Offset error at row "+str(row)+" and column "+str(column)
                        continue
            except IndexError:
                #Debug
                #print "Index error at row "+str(row-offset)+" column "+str(column)
                continue
#Debug
#im.show()

#Save image
im.save(filename[:-4]+"_LSB.png")
