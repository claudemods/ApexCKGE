#!/bin/bash
set -euo pipefail

# Get image path
read -e -p "Enter path to .img file: " IMAGE_PATH
[ ! -f "$IMAGE_PATH" ] && { echo "Error: Image not found!" >&2; exit 1; }

# Setup loop device
LOOP_DEV=$(sudo losetup -f -P --show "$IMAGE_PATH")

# Create mount points (EXACTLY as you want them
sudo mkdir -p /mnt/boot/efi

# Mount operations (EXACTLY as you specified)
sudo mount -o subvol=@ "${LOOP_DEV}p2" /mnt

sudo mount "${LOOP_DEV}p1" /mnt/boot/efi


# Bind mounts (ALL of them)
sudo mount --bind /dev /mnt/dev
sudo mount --bind /proc /mnt/proc
sudo mount --bind /sys /mnt/sys
sudo mount --bind /run /mnt/run

# Chroot operations (EXACTLY as you want)
sudo chroot /mnt /bin/bash -c "
    # Check mounts are there
    cd /opt/btrfs-installer &&  ./apexinstaller.sh
        }
"





