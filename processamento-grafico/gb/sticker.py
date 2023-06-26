import cv2 as cv


class Sticker:
    def __init__(
        self,
        id: str,
        src: str,
    ):
        self.id = id
        self.src = src
        self.isActive = False
        self.x = None
        self.y = None
        self.scale = None

        image = cv.imread(src)

        aspectRatio = image.shape[1] / image.shape[0]

        if aspectRatio > 1:
            self.image = cv.resize(image, (200, int(200 / aspectRatio)))

            buttonImage = cv.resize(self.image, (int(50 * aspectRatio), 50))

            self.buttonBytes = cv.imencode(".png", buttonImage)[1].tobytes()
        else:
            self.image = cv.resize(image, (int(200 * aspectRatio), 200))

            buttonImage = cv.resize(self.image, (int(50 * aspectRatio), 50))

            self.buttonBytes = cv.imencode(".png", buttonImage)[1].tobytes()

    def position(self, x: int, y: int, scale: int):
        self.x = x
        self.y = y
        self.scale = scale

        width = int(self.image.shape[1] * scale / 100)
        height = int(self.image.shape[0] * scale / 100)

        self.image = cv.resize(self.image, (width, height), interpolation=cv.INTER_AREA)

    def toggle(self):
        self.isActive = not self.isActive
