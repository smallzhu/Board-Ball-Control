我的板球控制采用的飞思卡尔的MK66 

库采用采用的山外的库

摄像头采用的是OV7725


寻找圆心的方法为，找打二值图像内的变化，变化部分即为球的位置，因为OV7725内自带硬件二值化，所以不用去二值化，它二值化的图像没有噪点，这才是最舒服的。

找到球的上下左右四个点后就能确定圆心坐标，但是我发现我寻找球左边最左边的点会有问题，找不到，于是就只有用三个点来寻找圆心坐标，效果依然很好。

因为我们是比赛当天才开始调，所以PID部分没有加进去，还有一些代码没有完善.

板球结构

![image](https://github.com/smallzhu/Board-Ball-Control/blob/master/images/board.jpg)

![image](https://github.com/smallzhu/Board-Ball-Control/blob/master/images/structure.jpg)

LCD显示效果

![image](https://github.com/smallzhu/Board-Ball-Control/blob/master/images/LCD.jpg)

主板

![image](https://github.com/smallzhu/Board-Ball-Control/blob/master/images/general.jpg)
