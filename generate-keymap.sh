qmk c2json ./keyboards/lily58/lightktrp/keymaps/ktrp/keymap.c --no-cpp | keymap -c keymap_config.yaml parse -c 10 -q - >keymap.yaml
keymap -c keymap_config.yaml draw keymap.yaml > keymap.svg
