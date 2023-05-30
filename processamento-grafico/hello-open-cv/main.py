import sys
import cv2 as cv


def handleKeyboardInput(saveIndex, inputKey, originalImage, visibleImage):
    result = [
        False,
        saveIndex,
        visibleImage,
    ]

    if inputKey == ord("1"):
        result[2] = cv.cvtColor(originalImage, cv.COLOR_BGR2GRAY)
    elif inputKey == ord("2"):
        result[2] = cv.cvtColor(originalImage, cv.COLOR_BGRA2BGR)
    elif inputKey == ord("s"):
        fileName = "_baboon".join([str(result[1]), ".png"])
        cv.imwrite(fileName, result[2])
        result[1] = result[1] + 1
        print("Saved filtered image as " + fileName)
    elif inputKey == ord("d"):
        print("Hello OpenCV")
    elif inputKey == ord("q"):
        result[0] = True
    else:
        print("Invalid input. Try again.")

    return result


def main():
    quit = False
    saveIndex = 0

    originalImage = cv.imread("baboon.png")
    visibleImage = originalImage

    if originalImage is None:
        sys.exit("Could not read the image.")

    while not quit:
        cv.imshow("Hello OpenCV", visibleImage)

        inputKey = cv.waitKey(0)

        result = handleKeyboardInput(saveIndex, inputKey, originalImage, visibleImage)

        quit = result[0]
        saveIndex = result[1]
        visibleImage = result[2]

        if quit:
            sys.exit("Goodbye!")


main()
