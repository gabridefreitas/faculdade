from config import Config
from ui import UserInterface
import cv2 as cv


def main():
    config = Config()
    UserInterface.init(config.filters)

    capture = cv.VideoCapture(1)

    while True:
        _, frame = capture.read()

        for filter in config.filters:
            if filter.isActive:
                frame = filter.filter(frame)

        cv.imshow(UserInterface.ID, frame)

        if cv.waitKey(1) == ord("q"):
            break

    capture.release()
    cv.destroyAllWindows()


main()
