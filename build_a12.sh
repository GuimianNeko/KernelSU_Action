    if [ "$ANDROID_VERSION" = "android12" ]; then
            cd android-kernel && mkdir bootimgs
            cp ./out/*/dist/Image ./bootimgs
            cp ./out/*/dist/Image.lz4 ./bootimgs
            cp ./out/*/dist/Image ../
            cp ./out/*/dist/Image.lz4 ../
            $GZIP -n -k -f -9 ../Image >../Image.gz
            cd ./bootimgs
            
            GKI_URL=https://dl.google.com/android/gki/gki-certified-boot-${{ github.event.inputs.TAG }}_r1.zip
	    FALLBACK_URL=https://dl.google.com/android/gki/gki-certified-boot-android12-5.10-2023-01_r1.zip
            status=$(curl -sL -w "%{http_code}" "$GKI_URL" -o /dev/null)
                
            if [ "$status" = "200" ]; then
                curl -Lo gki-kernel.zip "$GKI_URL"
            else
                echo "[+] $GKI_URL not found, using $FALLBACK_URL"
                curl -Lo gki-kernel.zip "$FALLBACK_URL"
            fi
                
                unzip gki-kernel.zip && rm gki-kernel.zip
                echo 'Unpack prebuilt boot.img'
                $UNPACK_BOOTIMG --boot_img="./boot-5.10.img"
                
                echo 'Building Image.gz'
                $GZIP -n -k -f -9 Image >Image.gz
                
                echo 'Building boot.img'
                $MKBOOTIMG --header_version 4 --kernel Image --output boot.img --ramdisk out/ramdisk --os_version 12.0.0 --os_patch_level 2024-01
                $AVBTOOL add_hash_footer --partition_name boot --partition_size $((64 * 1024 * 1024)) --image boot.img --algorithm SHA256_RSA2048 --key $GITHUB_WORKSPACE/kernel-build-tools/linux-x86/share/avb/testkey_rsa2048.pem
                cp ./boot.img ./${{ github.event.inputs.TAG }}-boot.img
                
                echo 'Building boot-gz.img'
                $MKBOOTIMG --header_version 4 --kernel Image.gz --output boot-gz.img --ramdisk out/ramdisk --os_version 12.0.0 --os_patch_level 2024-01
                $AVBTOOL add_hash_footer --partition_name boot --partition_size $((64 * 1024 * 1024)) --image boot-gz.img --algorithm SHA256_RSA2048 --key $GITHUB_WORKSPACE/kernel-build-tools/linux-x86/share/avb/testkey_rsa2048.pem
                cp ./boot-gz.img ./${{ github.event.inputs.TAG }}-boot-gz.img

                echo 'Building boot-lz4.img'
                $MKBOOTIMG --header_version 4 --kernel Image.lz4 --output boot-lz4.img --ramdisk out/ramdisk --os_version 12.0.0 --os_patch_level 2024-01
                $AVBTOOL add_hash_footer --partition_name boot --partition_size $((64 * 1024 * 1024)) --image boot-lz4.img --algorithm SHA256_RSA2048 --key $GITHUB_WORKSPACE/kernel-build-tools/linux-x86/share/avb/testkey_rsa2048.pem
                cp ./boot-lz4.img ./${{ github.event.inputs.TAG }}-boot-lz4.img
                cd ..
	fi
