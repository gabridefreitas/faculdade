import cv2 as cv
import PySimpleGUI as sg

from config import Config
from helpers import (
    applyFilters,
    drawStickers,
    handleButtonDisableState,
    handleButtonEvent,
    save,
)
from ui import UserInterface, ButtonColor


def main():
    ui = UserInterface()
    isRecording = False
    fileSource = None
    image = None
    visibleImage = None
    capture = None
    stickers = []
    videoFrameSize = False
    imageFrameSize = False
    savedIndex = 0

    while True:
        event, values = ui.window.read(timeout=20)

        src = ui.window[Config.events["text"]].get()

        if not isRecording and image is None:
            handleButtonDisableState(ui.window, True)

        if event == "Exit" or event == sg.WIN_CLOSED:
            break

        elif src != "" and src != fileSource:
            fileSource = src
            img = cv.imread(src)
            aspectRatio = img.shape[1] / img.shape[0]
            handleButtonDisableState(ui.window, False)

            if aspectRatio > 1:
                image = cv.resize(img, (720, int(720 / aspectRatio)))
            else:
                image = cv.resize(img, (int(720 * aspectRatio), 720))

        elif event == Config.events["record"]:
            if isRecording:
                capture.release()
                capture = None
                videoFrameSize = False
                ui.window[Config.events["image"]].erase()
                handleButtonDisableState(ui.window, True if image is None else False)
            else:
                capture = cv.VideoCapture(0)
                handleButtonDisableState(ui.window, False)

            isRecording = not isRecording
            ui.window[Config.events["record"]].update(
                button_color=ButtonColor.active if isRecording else ButtonColor.default
            )

        elif event == Config.events["save"]:
            if visibleImage is not None:
                save(f"output/uni-filter-{savedIndex}.png", ui.window)
                savedIndex += 1

        elif event == Config.events["image"]:
            x, y = values[event]
            hasActiveSticker = any(
                map(lambda sticker: sticker.isActive, Config.stickers)
            )

            if hasActiveSticker:
                for sticker in Config.stickers:
                    if sticker.isActive:
                        sticker.position(x, y)
                        stickers.append(sticker)

        elif event == Config.events["sticker-clear"]:
            stickers = []

        else:
            handleButtonEvent(event, ui.window)

        if isRecording:
            _, videoFrame = capture.read()
            filteredFrame = applyFilters(videoFrame)
            frameBytes = cv.imencode(".png", filteredFrame)[1].tobytes()
            visibleImage = filteredFrame

            if videoFrameSize is False:
                videoFrameSize = True
                ui.window[Config.events["image"]].set_size(
                    (videoFrame.shape[1], videoFrame.shape[0])
                )

            ui.window[Config.events["image"]].draw_image(
                data=frameBytes, location=(0, 0)
            )
            drawStickers(stickers, ui.window)

        elif image is not None:
            filteredImage = applyFilters(image)
            visibleImageBytes = cv.imencode(".png", filteredImage)[1].tobytes()
            visibleImage = filteredImage

            if imageFrameSize is False:
                imageFrameSize = True
                ui.window[Config.events["image"]].set_size(
                    (image.shape[1], image.shape[0])
                )

            ui.window[Config.events["image"]].draw_image(
                data=visibleImageBytes, location=(0, 0)
            )
            drawStickers(stickers, ui.window)

    if capture is not None:
        capture.release()
    ui.window.close()


main()
