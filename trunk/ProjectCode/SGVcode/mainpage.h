/** @mainpage 
*
* @authors 
*
* @section  聚类方法简介
* -聚类步骤
*-# 
*<pre> 
*将人类感知的颜色分成了两类，一种是：赤、橙、黄、绿、青、蓝、紫等基于不同可见光波长的色彩子空间；
*另一种是由多种波长组合成的灰白色，而灰白色在Lab色彩模式中可表现为亮度子空间，因此可以将白色表示为
*不同亮度条件下的灰色。
*</pre> 
*\image html ColorM.png "色彩模型"
*-# 
*<pre> 
*采用直方图方法可以将超像素图块归入互不相干的区域进行相对独立的聚类操作，因此可以保证并行计算的
*原子化操作，这有利于并行计算的实现。
*</pre> 
*\image html flowerHist.png    "彩色图片的直方图"
*\image html mountainHist.png  "灰度图片的直方图"
*-# 
*<pre> 
*该方法可以实现图像的大区域图块聚类与图像细节区域图块聚类的同步进行，并保证图像中的细节图块不会
*被大区域图块所吞噬。这一特点可以为图像中细节目标如：场景中的人体、汽车、动物等对象的目标捕捉与追
*踪提供有效的图块信息提取。
*</pre>  
*
* @section 聚类函数入口
*@see UINT CSLICSuperpixelsDlg::THreadSuperPixel(LPVOID lpParam)
*
*/