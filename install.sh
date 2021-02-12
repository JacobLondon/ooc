
unameout="$(uname -s)"
dest=''
case "${unameout}" in
    Linux*)
        mkdir -p /usr/local/include
        mkdir -p /usr/local/lib
        inc='/usr/local/include'
        lib='/usr/local/lib'
        ;;
    Darwin*)
        # TODO
        ;;
    MINGW64*)
        mkdir -p /usr/local/include
        mkdir -p /usr/local/lib
        inc='/usr/local/include'
        lib='/usr/local/lib'
        ;;
    *)
        echo "Not supported installation: ${unameout}"
        exit 1
esac

if [ "$1" = "clean" ]; then
    rm -rf "${inc}/ooc"
    rm -f "${lib}/libooc.a"
else
    cp -a "include/ooc" "${inc}"
    cp libooc.a "${lib}"
fi

exit $?
