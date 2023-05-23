import sys
import numpy as np
import cv2 as cv

img = cv.imread('baboon.png') 

if img is None:
    sys.exit("Could not read the image.")

gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

cv.imshow("Hello OpenCV - Default", img)
cv.imshow("Hello OpenCV - B&W", gray)

k = cv.waitKey(0)

if k == ord("s"):
    cv.imwrite("baboon_b&w.png", gray)