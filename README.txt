Hi thanks for looking at our Make OHIO Repo.

Team Members:
Nick Wells, Anuj Lutel, Kelly Finitsas, Joey Onderko

What was the goal?
We tried to automate the process of movning around your mirror when you are on the road. By using a webcam and a laptop as our processing power for OpenCV we were able to get XY cordinates from the camera. 

What about Z?
The team ran out of time but in order to get Z we would need a side camera or a sonar sensor points at the headrest of the seat. Also we thought in a car we could get the driver seat angles and position and estimate the headrest position.

Did it work?
We were able to get the rear view mirror to move in the correct directions. As we were limited on space we needed more time to adjust parameters so the mirror would move the correct distance. However, we were able to keep the mirror alawys points at a fixed point behind the driver which remained on the mirror.

What are the photo resistors for?
We used the photo resistors to estimate the suns position. If the sun was in your eyes and you made a button input the sun visor would flip down. More logic would need to be added on if the user would want this feature but the team thought it was an interseting idea to prototype.

Why a button?
The team found that having OpenCV run constantly used a lot of resources. The short cut was using a button to get one XYZ cordinate with only a few OpenCV frames needed. This worked but the team would like to spend more time investigating making OpenCV sleep and run once every minute or so to check if the driver needs the mirror adjusted. 

When is the mirror commonly adjusted on drives?
We found that the mirror is normally adjusted either when the driver enters the car, when the driver is in the middle of the driver, or when the driver seat is adjusted. If the team was able to change the mirror on entry then check when the driver seat is adjusted this would be a decent strategy.

