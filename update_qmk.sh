echo "Force updating QMK repo"
cd ../qmk_firmware \
    && git fetch \
    && git checkout -f origin/master

echo "Linking custom keyboard"
cd ../qmk_userspace \
    && ./prepare_local.sh
