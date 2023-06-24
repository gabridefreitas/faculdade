from typing import Any, Callable


class Filter:
    def __init__(self, id: str, name: str, method: Callable[[str], Any]):
        self.id = id
        self.name = name
        self.filter = method
        self.isActive = False

    def toggle(self):
        self.isActive = not self.isActive
