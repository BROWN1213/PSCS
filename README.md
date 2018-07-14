# Arduino-based autonomous navigation Cansat recovery system

본 시스템은 대기권에 진입해 낙하하는 위성이 자율 항법 알고리즘을 통해 안전한 목적지에 정확히 도달할수 있도록 아두이노를 이용하여 개발한 시스템입니다. 

![](https://github.com/BROWN1213/PSCS/blob/master/image/PSCS.png)

위성이 낙하하면서 자신의 위치정보를 GPS로 측정하고 목적지 까지의 거리와 방향각을 계산하여 자율항법알고리즘을 통해 스스로 낙하산을 조종합니다.  

![](https://github.com/BROWN1213/PSCS/blob/master/image/pscs1.png)
![](https://github.com/BROWN1213/PSCS/blob/master/image/pscs2.png)

본 시스템은 크게 HW구조와 SW 구조로 구성됩니다.  
첫째, HW구조는 본 시스템의 임무를 위한 낙하산, 낙하산 harness, winch 서보모터, 낙하 서보모터와 캔위성의 기본 구조인 CPU, BlueTooth, GPS, CAMERA, Scared,  IMU센서, LED로 구성됩니다.  

![](https://github.com/BROWN1213/PSCS/blob/master/image/PSCS4.png)

둘째, SW 구조는 낙하산을 펼치는 서모모터를 구동하는 FreeFall task, GPS의 위치 정보를 처리하는 GPS task, 현재 위치와 목표 지점까지의 거리와 방향각을 계산하는 Location task, 자율항법 알고리즘을 통해 winch 서보모터를 구동하여 원하는 지점까지 낙하산의 방향을 제어하는 Navigation task, 비행중 자세와 방위 정보를 측정하는 AHRS task 그리고 모든 센서값과 계산값을 저장하는 SD task, 각각의 task를 조절하는 task scheduler, 기지국과의 통신을 위한 BlueTooth 통신, 기지국의 명령을 수행하기 위한 BaseStation command 로 구성됩니다. 각 task는 캔위성의 SW플랫폼을 직접 제어하는  manager를 통해 임수를 수행합니다. 


![](https://github.com/BROWN1213/PSCS/blob/master/image/PSCS3.png)
