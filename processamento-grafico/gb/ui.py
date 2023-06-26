import PySimpleGUI as sg

from config import Config


class ButtonColor:
    default = ("black", "white")
    active = ("black", "lightblue")


class UserInterface:
    ID = "UniFilter"
    window = None
    layout = [
        [
            sg.Text("Media:"),
            sg.FileBrowse(
                button_color=ButtonColor.default,
                key="-FILE-",
                target="-TEXT-",
                auto_size_button=True,
            ),
            sg.FileSaveAs(
                button_color=ButtonColor.default,
                key="-SAVE-",
                auto_size_button=True,
            ),
            sg.Button(
                button_text="Record",
                button_color=ButtonColor.default,
                key="-RECORD-",
                auto_size_button=True,
            ),
        ],
        [
            sg.Text("Filters:"),
            *map(
                lambda filter: sg.Button(
                    button_text=filter.name,
                    enable_events=True,
                    key=filter.id,
                    button_color=ButtonColor.default,
                    auto_size_button=True,
                    disabled=True,
                ),
                Config.filters,
            ),
        ],
        [
            sg.Text("Stickers:"),
            *[
                *map(
                    lambda sticker: sg.Button(
                        image_data=sticker.buttonBytes,
                        enable_events=True,
                        key=sticker.id,
                        button_color=ButtonColor.default,
                        disabled=True,
                    ),
                    Config.stickers,
                ),
                sg.Button(
                    button_text="Clear",
                    enable_events=True,
                    key="sticker-clear",
                    button_color=ButtonColor.default,
                    auto_size_button=True,
                ),
            ],
        ],
        [
            sg.Graph(
                key="-IMAGE-",
                canvas_size=(720, 720),
                graph_bottom_left=(0, 720),
                graph_top_right=(720, 0),
                enable_events=True,
            )
        ],
        [sg.Text(key="-TEXT-")],
    ]

    def __init__(self):
        self.window = sg.Window(UserInterface.ID, UserInterface.layout, resizable=True)
