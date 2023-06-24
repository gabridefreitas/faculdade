from filter import Filter
from filter_methods import FilterMethods


class Config:
    def __init__(self):
        self.filters = [
            Filter("blur", "Blur", FilterMethods.Blur),
            Filter("gaussian-blur", "Gaussian Blur", FilterMethods.GaussianBlur),
            Filter("median-blur", "Median Blur", FilterMethods.MedianBlur),
            Filter(
                "bilateral-filter", "Bilateral Filter", FilterMethods.BilateralFilter
            ),
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

    def getActiveFilters(self):
        return list(filter(lambda filter: filter.isActive, self.filters))

    def getFilter(self, id: str):
        for filter in self.filters:
            if filter.id == id:
                return filter
