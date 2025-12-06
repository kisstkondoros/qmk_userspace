# On github, the QMK userspace is checked out then QMK itself into ./qmk_firmware
mkdir -p ./qmk_firmware/keyboards/lily58/lightktrp
ln ./keyboards/lily58/lightktrp/keyboard.json ./qmk_firmware/keyboards/lily58/lightktrp/keyboard.json
ln ./keyboards/lily58/lightktrp/rules.mk ./qmk_firmware/keyboards/lily58/lightktrp/rules.mk
