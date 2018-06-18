from enum import Enum
import ctypes


class WinApi:
    class BoxType(Enum):
        MB_OK = 0x0
        MB_OKCXL = 0x01
        MB_YESNOCXL = 0x03
        MB_YESNO = 0x04
        MB_HELP = 0x4000

    class IconType(Enum):
        EXCLAIM = 0x30
        INFO = 0x40
        QUESTION = 0x20
        STOP = 0x10

    READABLE_MAP = {
        'okonly':       BoxType.MB_OK,
        'okcancel':     BoxType.MB_OKCXL,
        'yesnocancel':  BoxType.MB_YESNOCXL,
        'yesno':        BoxType.MB_YESNO
    }

    def box_type(string):
        return WinApi.READABLE_MAP.get(string) or WinApi.BoxType.MB_OK

    def show_message(title, content, box_type=BoxType.MB_OK, icon=None):
        return icon and ctypes.windll.user32.MessageBoxW(0, content, box_type.value | icon.value) or ctypes.windll.user32.MessageBoxW(0, content, title, box_type.value)
