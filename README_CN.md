# SGL (Small Graphics Library)
# [English](README.md)

![SGL_LOGO](SGL_logo.png)

## 快速预览
<table>
  <tr>
    <td><img src="docs/gif/button.gif" width="250" alt="按钮"></td>
    <td><img src="docs/gif/checkbox.gif" width="250" alt="复选框"></td>
    <td><img src="docs/gif/label.gif" width="250" alt="标签"></td>
  </tr>
</table>
<table>
  <tr>
    <td><img src="docs/gif/switch.gif" width="250" alt="开关"></td>
    <td><img src="docs/gif/msgbox.gif" width="250" alt="消息框"></td>
    <td><img src="docs/gif/keyboard.gif" width="250" alt="键盘"></td>
  </tr>
</table>
<table>
  <tr>
    <td><img src="docs/gif/textbox.gif" width="250" alt="文本框"></td>
    <td><img src="docs/gif/arcbar.gif" width="250" alt="弧形进度条"></td>
    <td><img src="docs/gif/slider.gif" width="250" alt="滑块"></td>
  </tr>
</table>

### 最低硬件要求
| Flash大小 | Ram大小 |
| :---------:| :------: |
| 15kB       |    3kb   |

### 部分帧缓冲支持
最低只需单行像素的帧缓冲。

### 简介
SGL (Small Graphics Library) 是一个轻量级且快速的图形库，专为MCU级别处理器提供美观轻量的GUI（图形用户界面）。
请参考 `docs` 目录获取文档。

## 快速开始 (基于SDL2模拟)
步骤如下：
1. 安装gcc编译器
2. git clone https://github.com/sgl-org/sgl-port-windows.git    
3. cd sgl-port-windows && git submodule init    
4. git submodule update --remote    
3. cd demo && make -j8    
4. make run    

注意：请使用git终端或VSCode的git bash终端输入上述命令

## 1. 安装gcc编译器
从该地址下载 `gcc` (https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev0/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z)
然后解压到任意目录并添加到环境变量

## 2. 拉取sgl演示源码
git clone https://github.com/sgl-org/sgl-port-windows.git

## 3. 配置项目
cd sgl-port-windows && git submodule init    
git submodule update --remote      
cd demo && make -j8   

## 4. 运行
执行 `make run` 命令运行    
         
QQ群: 544602724
如有任何问题，请联系我: 1477153217@qq.com
