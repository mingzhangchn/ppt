1 service目录实现ppt服务功能，通过binder通信来控制启动与停止（注：目前停止功能未实现）
2 client是一个库加一个测试程序，用于控制后台的PPT服务。
3 通过/mnt/npi.xml配置节目列表。
4 http服务根目录为/tmp/www/,端口为8080
5 启动服务号会在/tmp/www/下生成PPT客户端需求的x.xml与status.xml。
6 设备发现功能可用。
7 目前使用的测试版DTVBtuner已经被浏览器里的播放器占用，测试PPT时先要停掉浏览器。
