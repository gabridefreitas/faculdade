import cv2 as cv
import PySimpleGUI as sg

from config import Config
from ui import UserInterface, ButtonColor
from PySimpleGUI import Window


def drawSticker(background, sticker, x_offset=None, y_offset=None):
    bg_h, bg_w, bg_channels = background.shape
    fg_h, fg_w, fg_channels = sticker.shape

    assert (
        bg_channels == 3
    ), f"background image should have exactly 3 channels (RGB). found:{bg_channels}"
    assert (
        fg_channels == 4
    ), f"sticker image should have exactly 4 channels (RGBA). found:{fg_channels}"

    # center by default
    if x_offset is None:
        x_offset = (bg_w - fg_w) // 2
    if y_offset is None:
        y_offset = (bg_h - fg_h) // 2

    w = min(fg_w, bg_w, fg_w + x_offset, bg_w - x_offset)
    h = min(fg_h, bg_h, fg_h + y_offset, bg_h - y_offset)

    if w < 1 or h < 1:
        return

    # clip sticker and background images to the overlapping regions
    bg_x = max(0, x_offset)
    bg_y = max(0, y_offset)
    fg_x = max(0, x_offset * -1)
    fg_y = max(0, y_offset * -1)
    sticker = sticker[fg_y : fg_y + h, fg_x : fg_x + w]
    background_subsection = background[bg_y : bg_y + h, bg_x : bg_x + w]

    # separate alpha and color channels from the sticker image
    sticker_colors = sticker[:, :, :3]
    alpha_channel = sticker[:, :, 3] / 255  # 0-255 => 0.0-1.0

    # construct an alpha_mask that matches the image shape
    alpha_mask = np.dstack((alpha_channel, alpha_channel, alpha_channel))

    # combine the background with the sticker image weighted by alpha
    composite = background_subsection * (1 - alpha_mask) + sticker_colors * alpha_mask

    # overwrite the section of the background image that has been updated
    background[bg_y : bg_y + h, bg_x : bg_x + w] = composite

    return background


def handleButtonState(window: Window):
    allButtons = Config.filters + Config.stickers

    for button in allButtons:
        window[button.id].update(
            button_color=ButtonColor.active if button.isActive else ButtonColor.default,
        )


def handleButtonDisableState(window: Window, disabled: bool):
    allButtons = Config.filters + Config.stickers

    for button in allButtons:
        window[button.id].update(disabled=disabled)


def handleFilterEvent(event: str, window: Window):
    isFilterEvent = any(map(lambda filter: filter.id == event, Config.filters))

    if not isFilterEvent:
        return

    filter = Config.getFilter(event)
    filter.toggle()

    if filter.id == "cartoon":
        Config.getFilter("gray").isActive = False
        Config.getFilter("canny").isActive = False
        window["gray"].update(disabled=filter.isActive)
        window["canny"].update(disabled=filter.isActive)

    handleButtonState(window)


def handleStickerEvent(event: str, window: Window):
    isStickerEvent = any(map(lambda sticker: sticker.id == event, Config.stickers))

    if not isStickerEvent:
        return

    def deactivateAllStickers():
        for sticker in Config.stickers:
            sticker.isActive = False

    sticker = Config.getSticker(event)

    if sticker.isActive:
        deactivateAllStickers()
    else:
        deactivateAllStickers()
        sticker.toggle()

    handleButtonState(window)


def handleButtonEvent(event: str, window: Window):
    handleFilterEvent(event, window)
    handleStickerEvent(event, window)


def main():
    ui = UserInterface()
    isRecording = False
    frame = None
    fileSource = None
    image = None
    visibleImage = None
    capture = None
    stickers = []

    while True:
        event, values = ui.window.read(timeout=20)

        src = ui.window["-TEXT-"].get()

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

        elif event == "-RECORD-":
            if isRecording:
                capture.release()
                frame = None
                capture = None
                ui.window["-IMAGE-"].erase()
                handleButtonDisableState(ui.window, True if image is None else False)
            else:
                capture = cv.VideoCapture(0)
                handleButtonDisableState(ui.window, False)

            isRecording = not isRecording
            ui.window["-RECORD-"].update(
                button_color=ButtonColor.active if isRecording else ButtonColor.default
            )

        elif event == "-SAVE-":
            if isRecording:
                ui.window["-SAVE-"].get()
            elif image is not None:
                ui.window["-SAVE-"].update(args=visibleImage)

        elif event == "-IMAGE-":
            x, y = values[event]
            hasActiveSticker = any(
                map(lambda sticker: sticker.isActive, Config.stickers)
            )

            if hasActiveSticker:
                for sticker in Config.stickers:
                    if sticker.isActive:
                        sticker.x = x
                        sticker.y = y
                        stickers.append(sticker)

        elif event == "sticker-clear":
            stickers = []

        else:
            handleButtonEvent(event, ui.window)

        if isRecording:
            _, videoFrame = capture.read()
            frame = videoFrame

            for filter in Config.filters:
                if filter.isActive:
                    frame = filter.filter(frame)

            aspectRatio = frame.shape[1] / frame.shape[0]

            if aspectRatio > 1:
                frame = cv.resize(frame, (720, int(720 / aspectRatio)))
            else:
                frame = cv.resize(frame, (int(720 * aspectRatio), 720))

            imageBytes = cv.imencode(".png", frame)[1].tobytes()
            ui.window["-IMAGE-"].set_size((frame.shape[1], frame.shape[0]))
            ui.window["-IMAGE-"].draw_image(data=imageBytes, location=(0, 0))

            for sticker in stickers:
                imageBytes = cv.imencode(".png", sticker.image)[1].tobytes()

                ui.window["-IMAGE-"].draw_image(
                    data=imageBytes, location=(sticker.x, sticker.y)
                )

        elif image is not None:
            visibleImage = image

            for filter in Config.filters:
                if filter.isActive:
                    visibleImage = filter.filter(visibleImage)

            imageBytes = cv.imencode(".png", visibleImage)[1].tobytes()
            ui.window["-IMAGE-"].set_size(
                (visibleImage.shape[1], visibleImage.shape[0])
            )
            ui.window["-IMAGE-"].draw_image(data=imageBytes, location=(0, 0))

            for sticker in stickers:
                imageBytes = cv.imencode(".png", sticker.image)[1].tobytes()

                ui.window["-IMAGE-"].draw_image(
                    data=imageBytes, location=(sticker.x, sticker.y)
                )

    if capture is not None:
        capture.release()

    ui.window.close()


main()
