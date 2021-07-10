
 MyEeprom 模块概述
==
<br>
    TM52Fxx系列芯片 内部EEPROM擦写次数为最小大于5万次,即单个地址的擦写次数是有限的<br>
 随着擦写次数的增加每次擦写的时间会有所延长，虽然对于一般不需要经常操作EEPROM的应<br>
 用5-10万的擦写次数完全可以满足整个芯片使用周期，但有些应用需要更加频繁的保存/读取<br>
 数据则需要芯片能够提供足够多的擦写次数。本DEMO主要为了有以上需要的朋友们提供一个很<br>
 好的解决方案.<br>
    DEMO的大致原理就是在芯片提供的整个EEPROM空间内，根据用户需要保存数据的大小,自动<br>
 分配N个区域,根据数据写入的次数动态地把用户数据保存在这些区域中，在芯片使用周期内确<br>
 保整个EEPROM空间使用率得到很好的平衡。理论上可以就达到了延长芯片EEPROM的使用寿命的<br>
 目的，使用寿命将延长至原来的xN(区域)倍。经压力测试每秒擦写20-30次保存15字节数据擦写<br>
 次数已经超过18万次多，数据没有发现异常.<br>
 
 ____
### 模块配置使用步骤----MyEeprom.h头文件<br>
>* `1:`  根据提示修改以下宏定义<br>
>>>>#define ChipEepromRamCP  256	   //芯片EepromRam的容量大小<br>
>>>>#define EepromStartAddr  0XEE00	  //这个芯片EEPROM物理内存中起始地址<br>
>>>>#define TM52FE8276_TotalEeprom  128  //这个芯片总共可以使用的EEPROM字节，TM52f82xx芯片偶地址有效<br>


>* `2：` 在枚举项@AllEepromAddr加入你需要保存数据的地址名称<br>
>>>>  `注意`枚举里的前后两个项不能更改<br>


>* `3：` 如果需要调试或其他测试，设置宏定义 TestEeprom  为TRUE <br> 
     
### 模块配置使用步骤----MyEeprom.c文件
>* `1：` 在工程初始化调用函数-->MyEepromInit()<br>


>* `2：` 根据提示修改此接口函数内部-->MyEepromSaveAllDataUserHandle ()<br>


>* `3：`单个数据写调用-->MyEepromWrite ( unsigned int,unsigned char ,unsigned char )
>>>>单个数据读调用-->MyEepromRead(unsigned char)<br>
>>>>测试时接口--    >MyEepromTestHandle ( char* from,unsigned char flag )<br>


 `其他：`暂时只测试了十速TM52FE8273芯片，根据数据手册其他TM52FEXX系列应该也是可以通用的<br>
         移植到到其他只需需该修改部分代码，后续有时间继续更新<br>
 
 ___       
`History     :` <br>
`Date        :` 2021/7/9<br> 
`Author      :` Dragon8814<br> 
 ____   

