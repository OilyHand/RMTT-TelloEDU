# sudo /home/jsson/conda/envs/tello/bin/esptool.py --chip esp32 --port /dev/ttyUSB0 erase_flash

sudo /home/jsson/conda/envs/tello/bin/esptool.py --chip esp32 \
  merge_bin -o merged.bin \
  --flash_mode dio --flash_freq 40m --flash_size 2MB \
    0x1000   ./build/esp32.esp32.esp32/RMTT.ino.bootloader.bin \
    0x8000   ./build/esp32.esp32.esp32/RMTT.ino.partitions.bin \
    0x10000  ./build/esp32.esp32.esp32/RMTT.ino.bin

sudo /home/jsson/conda/envs/tello/bin/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 \
  --before default_reset --after hard_reset write_flash -z \
  --flash_mode dio --flash_freq 40m --flash_size 2MB \
    0x0 merged.bin
