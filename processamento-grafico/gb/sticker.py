import cv2 as cv


class Sticker:
    def __init__(
        self,
        id: str,
        src: str,
    ):
        self.id: str = id
        self.src: str = src
        self.isActive: bool = False
        self.x: int = None
        self.y: int = None

        image = cv.imread(src, cv.IMREAD_UNCHANGED)

        aspectRatio = image.shape[1] / image.shape[0]
        stickerImage = None
        buttonImage = None

        if aspectRatio > 1:
            stickerImage = cv.resize(image, (200, int(200 / aspectRatio)))
            buttonImage = cv.resize(stickerImage, (int(50 * aspectRatio), 50))
        else:
            stickerImage = cv.resize(image, (int(200 * aspectRatio), 200))
            buttonImage = cv.resize(stickerImage, (int(50 * aspectRatio), 50))

        self.image = cv.imencode(".png", stickerImage)[1].tobytes()
        self.buttonBytes = cv.imencode(".png", buttonImage)[1].tobytes()

    def position(self, x: int, y: int):
        self.x = x
        self.y = y

    def toggle(self):
        self.isActive = not self.isActive
