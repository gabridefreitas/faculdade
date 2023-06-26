from PIL import ImageGrab
from PySimpleGUI import Window
from cv2 import Mat
from config import Config
from sticker import Sticker
from ui import ButtonColor


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


def applyFilters(image: Mat):
    filteredImage = image

    for filter in Config.filters:
        if filter.isActive:
            filteredImage = filter.filter(filteredImage)

    return filteredImage


def drawStickers(stickers: list[Sticker], window: Window):
    for sticker in stickers:
        window[Config.events["image"]].draw_image(
            data=sticker.image, location=(sticker.x, sticker.y)
        )


def save(filename, window: Window):
    widget = window[Config.events["image"]].Widget

    box = (
        widget.winfo_rootx(),
        widget.winfo_rooty(),
        widget.winfo_rootx() + widget.winfo_width(),
        widget.winfo_rooty() + widget.winfo_height(),
    )

    grab = ImageGrab.grab(bbox=box)
    grab.save(filename)
