Read me
======================

+ common 保存与编译整个工程有关的文件

+ AllocPM 和 Predict 分别保存分配函数和预测函数(函数原型已经写好,在predict文件
  中)该工程通过配置只在上述每个文件夹中引用一个文件,详见 CmakeConfig.txt

+ 预测和分配过程中如果需要多个文件请放入 othersrc 文件夹

+ 注意：由于最后所有文件放到同一个目录中，所以其中的 #include 直接加文件名,即不
  要写成 #include "../predict.h" "common/predict.h"  "../othersrc/predict.h" 等
  ，GCC 默认的 include 搜索路径是  common lib othersrc
