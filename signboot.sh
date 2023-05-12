version=${{ inputs.version_name }}
if [[ "$$version" == *"android12"* ]]; then
    echo '[+] Download prebuilt ramdisk'
	curl -Lo gki-kernel.zip https://dl.google.com/android/gki/gki-certified-boot-android12-5.10-"${{ inputs.patch_level }}"_r1.zip
	unzip gki-kernel.zip && rm gki-kernel.zip

	echo '[+] Unpack prebuilt boot.img'
	BOOT_IMG=$(find . -maxdepth 1 -name "boot*.img")
	$UNPACK_BOOTIMG --boot_img="$BOOT_IMG"
	rm "$BOOT_IMG"

	echo '[+] Building Image.gz'
	$GZIP -n -k -f -9 Image >Image.gz

	echo '[+] Building boot.img'
	$MKBOOTIMG --header_version 4 --kernel Image --output boot1.img --ramdisk out/ramdisk --os_version 12.0.0 --os_patch_level "${{ inputs.patch_level }}"
	$AVBTOOL add_hash_footer --partition_name boot --partition_size $((64 * 1024 * 1024)) --image boot.img --algorithm SHA256_RSA2048 --key $GITHUB_WORKSPACE/kernel-build-tools/linux-x86/share/avb/testkey_rsa2048.pem

	echo '[+] Building boot-gz.img'
	$MKBOOTIMG --header_version 4 --kernel Image.gz --output boot-gz.img --ramdisk out/ramdisk --os_version 12.0.0 --os_patch_level "${{ inputs.patch_level }}"
	$AVBTOOL add_hash_footer --partition_name boot --partition_size $((64 * 1024 * 1024)) --image boot-gz.img --algorithm SHA256_RSA2048 --key $GITHUB_WORKSPACE/kernel-build-tools/linux-x86/share/avb/testkey_rsa2048.pem

	echo '[+] Building boot-lz4.img'
	$MKBOOTIMG --header_version 4 --kernel Image.lz4 --output boot-lz4.img --ramdisk out/ramdisk --os_version 12.0.0 --os_patch_level "${{ inputs.patch_level }}"
	$AVBTOOL add_hash_footer --partition_name boot --partition_size $((64 * 1024 * 1024)) --image boot-lz4.img --algorithm SHA256_RSA2048 --key $GITHUB_WORKSPACE/kernel-build-tools/linux-x86/share/avb/testkey_rsa2048.pem
else
cp boot.img ../bootdir/
$MKBOOTIMG --header_version 4 --kernel Image --output boot1.img
$AVBTOOL add_hash_footer --partition_name boot --partition_size $((64 * 1024 * 1024)) --image boot1.img --algorithm SHA256_RSA2048 --key $GITHUB_WORKSPACE/kernel-build-tools/linux-x86/share/avb/testkey_rsa2048.pem
fi
cp boot1.img ./bootdir/
cp boot-lz4.img ./bootdir/
cp boot-lz4.img ./bootdir/