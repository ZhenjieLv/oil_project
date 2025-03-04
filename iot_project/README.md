# 海油项目
此项目包括：
- 边缘一体机程序(Jetson NX 板子)

# 边缘一体机程序(Jetson NX 板子)

## 依赖环境

- c++
- pkg-config（这个一般是自带的）
- spdlog
- jsoncpp
- QT 5.9.0
- MySQL
- kafka 3.0.2
- zookeeper 3.6.1
- cryptopp/crypto++


具体安装方式见：doc文件夹中的`环境安装.md`

## 编译方式

调试版本：
```shell
mkdir build && cd build
cmake ..
make -j6
```

发布版本：
```shell
mkdir build && cd build
cmake -DIS_DEBUG=OFF ..
make -j6
```


