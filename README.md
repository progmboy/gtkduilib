# gtkduilib

duilib向gtk的移植

##编译

windows:
```
install [msys2](https://msys2.github.io/)
then install gtk3.0.x from msys2 shell
install codeblocks
open the workspace and build
```

linux (ubuntu):
```
sudo apt-get install codebloks libgtk3.0xxxx
open the workspace and build
```

##Demo
大家可以看下demo.

player run in windows:

![player_windows](https://github.com/progmboy/gtkduilib/raw/master/snapshots/1.png)

player run in ubuntu:

![player_ubuntu](https://github.com/progmboy/gtkduilib/raw/master/snapshots/2.jpg)

360safe run in ubuntu:

![360demo](https://github.com/progmboy/gtkduilib/raw/master/snapshots/3.jpg)

##和原版的duilib有啥不同
基本上兼容原版duilib的格式，主要有下面几种不同.

1. 全部使用unicode, 便于移植, windows A/W很是闹心呀.
2. 绘制字体使用pango的接口，没有保留原版的绘制接口.
3. 好多LPCTSTR类型的参数使用const CUIString& 传递.
4. 使用pugixml作为xml解析.
5. 直接使用gdk-pixbuf加载图片

##备注
1. 做这个的目的其实是为了学习下gtk.duilib的框架很好，把windows消息机制移植到gtk的机制，控件代码基本上都不用改。
2. gtk for windows的bug多多，建议还是在linux下弄
3. gtk 在windows下是支持加载系统字体的,但是只支持english name, 大家写xml的时候注意下。
4. 不喜勿喷, 谢谢 ^_^
5. 欢迎pull request

##TODO:
太多了。
- [ ] 上传demo
- [ ] 键盘事件
- [ ] EDIT控件，树控件, webview
- [ ] 移植原版demo
- [ ] ...
