from filter import Filter
from filter_methods import FilterMethods
from sticker import Sticker


class Config:
    filters = [
        Filter("blur", "Blur", FilterMethods.Blur),
        Filter("gaussian-blur", "Gaussian Blur", FilterMethods.GaussianBlur),
        Filter("median-blur", "Median Blur", FilterMethods.MedianBlur),
        Filter("bilateral-filter", "Bilateral Filter", FilterMethods.BilateralFilter),
        Filter("gray", "Gray", FilterMethods.Gray),
        Filter("canny", "Canny", FilterMethods.Canny),
        Filter("brightness", "Brightness", FilterMethods.Brightness),
        Filter("sharpness", "Sharpness", FilterMethods.Sharpness),
        Filter("summer", "Summer", FilterMethods.Summer),
        Filter("winter", "Winter", FilterMethods.Winter),
        Filter("autumn", "Autumn", FilterMethods.Autumn),
        Filter("spring", "Spring", FilterMethods.Spring),
        Filter("cartoon", "Cartoon", FilterMethods.Cartoon),
    ]
    stickers = [
        Sticker("sticker-eita", "assets/sticker-eita.png"),
        Sticker("sticker-heart", "assets/sticker-heart.png"),
        Sticker("sticker-love", "assets/sticker-love.png"),
        Sticker("sticker-new-story", "assets/sticker-new-story.png"),
        Sticker("sticker-top", "assets/sticker-top.png"),
    ]
    events = {
        "save": "save",
        "record": "record",
        "sticker-clear": "sticker-clear",
        "text": "text",
        "image": "image",
        "file": "file",
    }

    def getFilter(id: str):
        for filter in Config.filters:
            if filter.id == id:
                return filter

    def getSticker(id: str):
        for sticker in Config.stickers:
            if sticker.id == id:
                return sticker
