# cpp-gobang-ai-learning

<br>

## Introduction:


![](https://img.shields.io/badge/language-c++-orange.svg) ![](https://img.shields.io/badge/language-Qt-orange.svg) ![](https://img.shields.io/github/license/Fizgrad/cpp-gobang-ai-learning) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/Fizgrad/cpp-gobang-ai-learning)
![GitHub All Releases](https://img.shields.io/github/downloads/Fizgrad/cpp-gobang-ai-learning/total)  ![GitHub stars](https://img.shields.io/github/stars/Fizgrad/cpp-gobang-ai-learning?style=social) ![GitHub forks](https://img.shields.io/github/forks/Fizgrad/cpp-gobang-ai-learning?style=social) 

<br>
注意在mainwindow.cpp里的mousePressEvent函数中

构造p的时候不同的系统传入的参数不一样：

for windows：

coordinate p((x-12)/24, (y)/24);

for mac os：

coordinate p((x+12)/24, (y)/24);
<br>

### 感谢网友们编写的精美教程。

## 编写过程：

4月30日 评估函数完成

5月2日 极大极小值搜索出现问题 选择不了最优解

5月3日 减枝算法完成 

5月4日 用zobrist算法进行了进一步的优化

5月17日 添加了多线程

5月21日 使用qt完成图形界面

鄙人大作业，禁止完全抄袭
