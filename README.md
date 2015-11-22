# gtkduilib

duilib向gtk的移植

##编译

windows:
1,install [msys2] (https://msys2.github.io/)
2,then install gtk3.0.x from msys2 shell
3,install codeblocks
4,open the workspace and build

linux (ubuntu):
1,sudo apt-get install codebloks libgtk3.0xxxx
2,open the workspace and build

##Demo
以前弄好的Demo找不到了但是有截图.

##和原版的duilib有啥不同
基本上兼容原版duilib的格式，主要有下面几种不同
1, 全部使用unicode, 便于移植, windows A/W很是闹心呀
2, 绘制字体使用pango的接口，没有保留原版的绘制接口
3, 好多LPCTSTR类型的参数使用const CUIString& 传递
4，使用pugixml作为xml解析

##备注
1, 做这个的目的其实是为了学习下gtk.duilib的框架很好，把windows消息机制移植到gtk的机制，控件代码基本上都不用改。
2, gtk for windows的bug多多，建议还是在linux下弄
3, 作者心灵很脆弱,不喜勿喷, 谢谢 ^_^

##TODO:
太多了。
-[] 上传demo
-[] 键盘事件
-[] EDIT控件，树控件, webview
-[] 移植原版demo
