$MKBOOTIMG --header_version 4 --kernel out/Image --output boot.img
$AVBTOOL add_hash_footer --partition_name boot --partition_size $((64 * 1024 * 1024)) --image boot.img --algorithm SHA256_RSA2048 --key ../kernel-build-tools/linux-x86/share/avb/testkey_rsa2048.pem
