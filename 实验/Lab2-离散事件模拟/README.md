# README

## 实验说明

**我们会针对大家的问题修订README。这只是为了更好地强调实验需求，解答实验中可能会遇到的问题，不会更改实验内容。**

### 难度说明

**部分代码助教已经帮忙填充**

**simulation.cc 需要填写的代码只有12行！**

**queue.h 和 priority_queue.h 需要填写的代码大约60行！**

**本次实验只需填写 queue.h，priority_queue.h 和 simulation.cc 的 TODO 部分。填写所需要的 C++ 特性只有：**

1. 课本上所讲过的传引用。

2. simulation.cc 中调用数据结构函数时，加上类型以区别，如 `Queue<Customer>::empty` 。

**希望大家跳出舒适圈，不要过于依赖工具。**

### 实验设计思想

#### 1. 隔离上层应用和数据结构

我们在检查多项式实验时，发现很多同学在写如多项式加法这样的应用函数时，直接操控链表，没有把数据结构和上层应用进行隔离。

为什么应用和数据结构要隔离？这是一个很好的问题。

首先，数据结构就能重复地被应用程序调用，不用再重复造轮子。

第二，当程序出现bug时，我们先对底层测试发现无误后就可以假定上层应用出现问题。否则，调试多项式加法时你还要关心指针是不是NULL，非常不合理。

第三，发现这个数据结构不适合这个应用时，可以通过改变数据结构很快就能对应用修改。如果不隔离，比如多项式链表换成顺序表，程序基本要从头开始写。如果能有效隔离，比如这次的优先队列，想把时间复杂度从 O(n) 提高到 O(logn), 只需要修改相关函数，完全不需要修改上层应用的代码。

#### 2. 单元测试

大家可以写完一个文件就能进行测试，及时找到程序中的bug。**强烈推荐大家写完一个模块就运行测试一下，不要拖到最后！**

## 环境配置

### Linux

可使用git下载本仓库。

```bash
git clone https://git.ustc.edu.cn/ds-fa20/queue.git
```

本实验需要在Linux操作系统下完成。

需要的安装的工具有 git，CMake，Valgrind，编译器GCC或Clang。

可以在命令行界面下输入如下命令安装。

```bash
sudo apt-get install cmake
```

在实验主目录下，输入以下命令即可对项目进行编译并运行。

```
./build.sh
./run.sh
```

编译完成后，你可以进入build目录下，对某一个测试单独进行运行。

```bash
cd build
./test_queue # 写好 queue.h 即可测试
./test_priority_queue # 写好 priority_queue.h 即可测试
valgrind ./test_memory
./test_simulation
```

## 测试说明

```bash
# 使用提供的测试文件进行测试，在实验主目录下输入
./run.sh
```

如果正确的话，则catch测试都会绿色通过，valgrind测试 0 error。

通过普通队列、优先队列、排队模拟分别得3分，通过内存测试得1分。

对于排队模拟，可以看一下理论值和模拟值差距，如果不大可以再运行一次。因为设定的是95%的置信区间，哪怕模拟没问题，偶尔也会出现fail。

内存测试不通过的原因可能有：使用未初始化的元素；分配在堆内存上的空间没有回收。

## 文件说明

queue.h中包含了普通队列的相关操作，需要补充todo部分。

priority_queue.h包含了优先队列的相关操作，需要补充todo部分。

simulation.cc中包含了模拟的主要过程，需要补充todo部分。

**请不要修改其他部分！**

CMakeList.txt是cmake的配置文件。

test/*.cc是4个测试文件，分别会测试普通队列、优先队列、模拟结果的正确性。

test/catch.hpp是C++单元测试框架。

util中是与排队模拟相关的工具函数。


## 实验要求

填写所有的todo部分，**请不要修改其他部分！**

### 数据结构部分

#### 实现操作

在**queue.h** 和**priority_queue.h**中需要填写这两个数据结构的操作。

1. **init**函数对数据结构进行初始化。

2. **destroy**函数负责清除分配堆内存的空间。

3. **enqueue**函数会将一个元素加入到队列中。如果队列已满，返回false；否则，返回true。

4. **dequeue**函数将队头元素从队列中取出。如果队列中没有数据，返回false；否则，返回true。

5. **top**函数会访问队列头部中的元素，但不会改变队列。如果队列中没有数据，返回false；否则，返回true。

6. **length**函数返回队列中元素的个数。

7. **full**和**empty**函数分别判断队列是否为满或者为空。

#### 实现细节

1. queue 和 priority queue 最大的不同点在于：queue 要求先进入队列的元素最先出来，而 priority queue 要求把在队列数据中最小的元素放入队头，让其成为最先出来的元素。
2. 两个数据结构中的 data 域是存放数据的数组， capacity 域表示队列能容纳的最大元素个数。由于队空和队满之间存在 capacity + 1 种状态，data 也需要分配 capacity + 1 的容量以区分。
3. queue 的 front 指向队头元素，而 rear 指向下一个元素插入的位置。
4. priority queue 的实现：数据只能存放在 data[1] 和 data[rear] 之间，data[1] 一定是优先队列中元素最小的，且满足 data[i/2] <= data[i] (i > 1)。元素入队时，放入队列末尾，判断是否满足 data[i/2] <= data[i]。若满足可返回；若不满足则交换 data[i/2] 和 data[i] 的位置，继续判断交换直到满足条件。例如，在原来有4个元素的优先队列中，插入比原队列元素都要小的元素，则会发生交换 data[5] <-> data[2], data[2] <-> data[1]。元素出队时，取出对头元素，将队尾元素放入队头位置 data[1]，从 data[1] 处开始判断 data[i/2] <= data[i] 这一条件。（注意：满足 i/2 = j 的 i 有 2j 和 2j+1， 并判断是否越界，操作的时间复杂度应该为 O(lgn) ）
5. 觉得上面实现有困难的。可以用遍历找出最小元素放入队头。（时间复杂度是线性的，但测试不卡时间）
6. 为了通用性，priority queue 需要比较元素大小的函数指针 cmp。cmp(a, b) >= 0 表示 a >= b。

### 队列模拟部分

#### 总体需求

银行共有 num_servers 个柜台。顾客达到银行后，如果柜台都被占满，就会按它们的进入时间排成一个队列，先到先服务。如果排队中的人数超过 queue_capacity，后来的顾客就不进入银行办理业务了。

我们已随机生成了达到银行的顾客的到达时间和业务办理时间。你需要在**simulation.cc**中填写相关操作。最终得到**办理过业务总人数 （不包括没有进入银行的顾客）**，和他们在**排队等待的总时间 （不包括办理业务的时间）**。返回排队等待的平均时间。

#### 实现细节

Customer中分别有3个数据，分别是：arrive_time，service_time, leave_time。它们分别表示客户到达银行的时间点，客户办理业务需要的时间，客户最终离开的时间点。

simulate函数中有3个队列数据结构，都存储Customer类型，分别是：arrive_flow、queue、leave_flow。

arrive_flow已生成好并作为simulate函数的参数，已经**根据进入银行的时间做了从小到大的排序**，且arrive_time，service_time已经根据指数分布随机生成。

queue是客户在银行内排队等待的一个队列。如果所有的服务台都被占用，就会有客户在等待。如果等待人数超过队列容量queue_capacity，新来的客户无法排队，直接离开。

leave_flow是所有正在办理业务的客户的优先队列，根据客户离开时间进行排序。容量大小就是服务台数量num_servers。你需要自己**设定好leave_time和Customer在优先队列中的比较函数**。

从客户视角上看。客户在arrive_time进入银行（arrive_flow 出队）。如果银行排队的人太多了，就直接走了；否则加入排队（queue 入队）。如果有一个服务台没有人办业务，自己在队头，就能结束排队（queue 出队），用service_time的时间办业务（leave_flow 入队）。到了leave_time后就离开银行 （leave_flow 出队）。

从上帝视角上看。只会发生3种事件，其中只有2种事件会改变系统，一个是**客户进入银行**，另一个是**客户离开银行**。也只有在这两个事件发生后，**排队等待中的客户开始办理业务**的事情才会发生。因此<del><html>作为膜法师</del></html>，只需要<del><html>操控时间</del></html>，选择做最近的这两类事件，然后判断能不能做第三种事件。

## 问题与回答

问：什么是 .cc 文件格式？

答 ：Linux/Unix 环境下的 C++ 程序通常使用 .cc 结尾。

问：为什么按照这样的方式组织文件而不是把所有的代码写在一个文件中？

答：把所有代码写到一个文件下是工程上的灾难！另外，每个测试文件都是可执行文件，而main函数只有1个，为了避免冲突也只能拆分。

问：我是 C++ 大佬，为什么要用 struct 而不是 class？我是 C++ 小白，每个函数里的::是什么意思？

答：因为我们既要保证易用性 （数据结构既能装int数据，又能装Customer），又要保证所填代码 C++ 特性极可能的少，使得不会 C++ 的同学也能填写。不会 C++ 的同学只需要知道教程上的引用`&`，以及在 simulation.cc 中调用数据结构函数时在前面加上`类型::`即可。

问：为什么在 simulation.cc 里用 queue.data 会报错？

答：排队模拟是应用层的，而 queue.data 是抽象的数据结构层的，已被设置为private。它们之间的交互请使用我们提供的函数。计算机科学很重要的一个思想就是抽象。不然，程序员就该操作物理层电子的流动了。

问：cmake 版本太低了，怎么办？

答：可以修改 CMakeList.txt 的第一行。目前已调整到已知可以编译的最低版本。

问：在做内存测试时 valgrind 报了 Unrecognised instruction 的错误，怎么办？

答：因为随机数生成使用的 random_device 使用了特殊的 CPU 指令，旧版本的 valgrind 无法识别。请升级 valgrind 版本。

问：最后的模拟测试有时候会不通过，为什么？

答：我们要求理论值要在模拟测试结果的95%置信区间内。偶尔出现一次不通过是正常现象。

问：程序调试困难，怎么办？

答：目前版本已经加入了支持 gdb 的选项，当然用 print 大法也是可以的。

## 附录

### Linux环境配置

- 可以下载虚拟机与镜像进行安装

  - virtualbox 下载链接: https://rec.ustc.edu.cn/share/49792820-0967-11eb-8e75-2f662354f3be

  - ubuntu 20.04.1 下载链接：http://mirrors.ustc.edu.cn/ubuntu-releases/20.04.1/ubuntu-20.04.1-desktop-amd64.iso

  - manjaro 20.1 下载链接：http://mirrors.ustc.edu.cn/manjaro-cd/kde/20.1/manjaro-kde-20.1-200911-linux58.iso

- 也可以使用学校提供的vlab
  - https://vlab.ustc.edu.cn/

### 实验中用到的c++特性

- 类模板 可以理解成对书本上使用elemtype作为元素类型的ADT的一种具体实现，利用类模板生成类的代码为` Queue<Customer> queue`也即`模板名<元素类型名> 实例化得到的对象名 `
- 访问控制 可以简单地理解为private成员只能在该类的成员函数中使用，public可以在其它类的成员函数中使用。（关于友元函数和protect此处并未做更进一步地描述）
- 引用 引用是一个别名`int & a= b`, 则 a是b的一个别名，对`a`的修改会直接作用在b上。

### 概率统计的背景

可以了解，对于写实验并没有太大帮助。

蒙特卡洛模拟

https://en.wikipedia.org/wiki/Monte_Carlo_method

置信区间<html><del>（你们做大物实验应该知道）</del></html>

https://en.wikipedia.org/wiki/Confidence_interval

排队论 （我们模拟的是M/M/c/N模型）

https://www2.isye.gatech.edu/~sman/courses/6644/Module05Q-QueueingSlides_171025.pdf



