# cpp-gobang-ai-learning

注意在mainwindow.cpp里的mousePressEvent函数中

构造p的时候不同的系统传入的参数不一样：

for windows：

coordinate p((x-12)/24, (y)/24);

for mac os：

coordinate p((x+12)/24, (y)/24);

感谢网友们编写的精美教程。

编写过程：

4月30日 评估函数完成

5月2日 极大极小值搜索出现问题 选择不了最优解

5月3日 减枝算法完成 

5月4日 用zobrist算法进行了进一步的优化

5月17日 添加了多线程

5月21日 使用qt完成图形界面

鄙人大作业，禁止完全抄袭
