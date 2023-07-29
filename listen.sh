echo $GITHUB_WORKSPACE
ls $GITHUB_WORKSPACE
echo $ktag
while true; do
    for dir in $GITHUB_WORKSPACE/android-kernel/out/*/common/drivers/qaq; do
        if [ -e "$dir/$1" ]; then
            cp $dir/$1 $dir/$ktag.ko
            $GITHUB_WORKSPACE/upload.sh $dir/$ktag.ko
            exit 0
        fi
    done
    sleep 1
done
