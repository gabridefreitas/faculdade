import cv2 as cv
import numpy as np

from cv2 import Mat


class FilterMethods:
    @staticmethod
    def Blur(image: Mat):
        return cv.blur(image, (5, 5))

    @staticmethod
    def GaussianBlur(image: Mat):
        return cv.GaussianBlur(image, (5, 5), 0)

    @staticmethod
    def MedianBlur(image: Mat):
        return cv.medianBlur(image, 5)

    @staticmethod
    def BilateralFilter(image: Mat):
        return cv.bilateralFilter(image, 9, 75, 75)

    @staticmethod
    def Gray(image: Mat):
        return cv.cvtColor(image, cv.COLOR_BGR2GRAY)

    @staticmethod
    def Canny(image: Mat):
        return cv.Canny(image, 100, 200)

    @staticmethod
    def Brightness(image: Mat):
        return cv.convertScaleAbs(image, alpha=1.5, beta=0)

    @staticmethod
    def Sharpness(image: Mat):
        kernel = np.array([[-1, -1, -1], [-1, 9, -1], [-1, -1, -1]])
        return cv.filter2D(image, -1, kernel)

    @staticmethod
    def Summer(image: Mat):
        return cv.convertScaleAbs(image, alpha=1.5, beta=50)

    @staticmethod
    def Winter(image: Mat):
        return cv.convertScaleAbs(image, alpha=1.5, beta=-50)

    @staticmethod
    def Autumn(image: Mat):
        return cv.convertScaleAbs(image, alpha=1.5, beta=100)

    @staticmethod
    def Spring(image: Mat):
        return cv.convertScaleAbs(image, alpha=1.5, beta=-100)

    @staticmethod
    def Cartoon(image: Mat):
        gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
        gray = cv.medianBlur(gray, 5)
        edges = cv.adaptiveThreshold(
            gray, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY, 9, 9
        )
        color = cv.bilateralFilter(image, 9, 250, 250)
        cartoon = cv.bitwise_and(color, color, mask=edges)
        return cartoon

    @staticmethod
    def Sepia(image: Mat):
        image = np.array(image, dtype=np.float64)
        image = cv.transform(
            image,
            np.matrix(
                [[0.272, 0.534, 0.131], [0.349, 0.686, 0.168], [0.393, 0.769, 0.189]]
            ),
        )
        image[np.where(image > 255)] = 255
        return np.array(image, dtype=np.uint8)

    def HDR(image: Mat):
        return cv.detailEnhance(image, sigma_s=12, sigma_r=0.15)

    def Invert(image: Mat):
        return cv.bitwise_not(image)
