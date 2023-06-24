import cv2 as cv
from filter import Filter
from mouse import Mouse


class UserInterface:
    ID = "UniFilter"

    @staticmethod
    def init(filters: list[Filter]):
        cv.namedWindow(UserInterface.ID)

        for filter in filters:
            cv.createButton(filter.name, filter.toggle, filter.id, cv.QT_PUSH_BUTTON, 1)

        cv.setMouseCallback(UserInterface.ID, Mouse.handler)
