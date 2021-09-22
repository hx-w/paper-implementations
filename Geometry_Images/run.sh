# !/bin/sh
# Usage: sh run.sh <lang> <meshfile>


valid_argv=(
    cpp
    python
)

function run() {
    if [[ "${1}" == cpp ]]; then
        echo "[INFO] workdir changed"
        cd cpp/
        echo "[INFO] compiling start"
        sh build-darwin.sh
        if [[ $? -eq 0 ]]; then
            echo "[INFO] compiling success"
        else
            echo "[ERROR] compiling failed"
            exit
        fi
        echo "==== CPP START ===="
        ./main ../"${2}"
    elif [[ "${1}" == python ]]; then
        echo "[INFO] workdir changed"
        cd python/
        read -p "[WARN] current python3 version > 3.6? [Y/N] " YorN
        if [[ "yY" =~ "${YorN}" ]]; then
            echo "[INFO] install requirements"
            pip3 install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple
        else
            echo "[ERROR] python version unsatisfied"
            exit
        fi
        echo "==== PYTHON START ===="
        python3 run.py ../"${2}"
    fi
}

# parse args
if [ $# -eq 2 ]; then
    if [[ "${valid_argv[@]}" =~ "${1}" ]]; then
        echo "[INFO] lang set <${1}>"
        if [ ! -f "${2}" ]; then
            echo "[ERROR] mesh file not exist!"
            exit
        fi
        echo "[INFO] mesh file <${2}> confirmed"
        run $1 $2
    else
        echo "[ERROR] invalid lang <${1}>"
        exit
    fi
else
    echo "[ERROR] invalid params"
fi